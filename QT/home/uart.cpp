#include "uart.h"
#include "mainwindow.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <QString>
#include <QDebug>
#define FILENAME "/dev/ttymxc2"

int set_opt(int,int,int,char,int);
int fd1;

uart::uart()
{

}

uart::~uart()
{
    ::close(fd1);
}

void uart::run()
{
    int nByte;
    char recbuffer[20],sedbuffer[5];
    memset(recbuffer, 0, sizeof(recbuffer));
    memset(sedbuffer, 0, sizeof(sedbuffer));

    while(1)
    {
        fd1 = open(FILENAME, O_RDWR|O_NONBLOCK);
        set_opt(fd1, 115200, 8, 'N', 1);
        nByte = read(fd1, recbuffer, 512);
        recbuffer[nByte+1] = '\0';

        //write(fd,buffer,strlen(buffer));
        //qDebug() << "ok";
        if(recbuffer[0] == 0xff && recbuffer[4] == 0x55)
        {
            qDebug() << recbuffer;
            if(recbuffer[1] == 0x01)
            {
                if(recbuffer[2] == 0x01)
                {
                    emit SendReceive(0x00);
                }
                else if(recbuffer[2] == 0x02)
                {
                    emit SendReceive(0x01);
                }

            }
            else if(recbuffer[1] == 0x02)
            {
                if(recbuffer[2] == 0x01)
                {
                    emit SendReceive(0x02);
                }
                else if(recbuffer[2] == 0x02)
                {
                    emit SendReceive(0x03);
                }
            }
            else if(recbuffer[1] == 0x03)
            {
                sedbuffer[0] = 0xff;
                sedbuffer[4] = 0x55;
                sedbuffer[1] = 0x03;
                sedbuffer[2] = (unsigned char)gTemp;
                sedbuffer[3] = (unsigned char)gHumi;
                write(fd1,sedbuffer,strlen(sedbuffer));
            }
            else if(recbuffer[1] == 0x04)
            {
                sedbuffer[0] = 0xff;
                sedbuffer[4] = 0x55;
                sedbuffer[1] = 0x04;
                sedbuffer[2] = (unsigned char)gHour;
                sedbuffer[3] = (unsigned char)gMin;
                write(fd1,sedbuffer,strlen(sedbuffer));
            }
        }
        memset(recbuffer, 0, sizeof(recbuffer));
        memset(sedbuffer, 0, sizeof(sedbuffer));
        nByte = 0;
        ::close(fd1);
        QThread::msleep(100);

    }
}



int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if  ( tcgetattr( fd,&oldtio)  !=  0) {
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch( nBits )
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }

    switch( nEvent )
    {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch( nSpeed )
    {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        case 460800:
            cfsetispeed(&newtio, B460800);
            cfsetospeed(&newtio, B460800);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }
    if( nStop == 1 )
        newtio.c_cflag &=  ~CSTOPB;
    else if ( nStop == 2 )
        newtio.c_cflag |=  CSTOPB;
        newtio.c_cc[VTIME]  = 100;///* 设置超时10 seconds*/
        newtio.c_cc[VMIN] = 0;
        tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }

    //	printf("set done!\n\r");
    return 0;
}
