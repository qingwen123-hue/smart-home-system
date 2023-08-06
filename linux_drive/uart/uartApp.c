#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
 
int set_opt(int,int,int,char,int);
void main()
{
	int fd,nByte,flag=1;
	char *uart3 = "/dev/ttymxc2";  //"/dev/ttymxc2"是COM3
	char buffer[512];
	char *uart_out = "Please input,waiting....\r\n";
        char *uart_demo = "Linux uart demo\r\n";
	memset(buffer, 0, sizeof(buffer));
	//if((fd = open(uart3, O_RDWR|O_NOCTTY))<0)//默认为阻塞读方式
          if((fd = open(uart3, O_RDWR|O_NONBLOCK))<0)//非阻塞读方式
 
		printf("open %s is failed",uart3);
	else{
		set_opt(fd, 115200, 8, 'N', 1);
		write(fd,uart_demo, strlen(uart_demo));
                write(fd,uart_out, strlen(uart_out));
		while(1){
			nByte = read(fd, buffer, 512);
			printf("%d\n",nByte);
			while(nByte>0)
			{
				buffer[nByte+1] = '\0';			
				write(fd,buffer,strlen(buffer));
				
				//printf("%X\n",buffer[1]);
				memset(buffer, 0, strlen(buffer));
				nByte = 0;
			}
		}
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
	
	// newtio.c_cflag &= ~HUPCL;
    // newtio.c_iflag &= ~INPCK;
    // newtio.c_iflag |= IGNBRK;
    // newtio.c_iflag &= ~ICRNL;
    // newtio.c_iflag &= ~IXON;
    // newtio.c_lflag &= ~IEXTEN;
    // newtio.c_lflag &= ~ECHOK;
    // newtio.c_lflag &= ~ECHOCTL;
    // newtio.c_lflag &= ~ECHOKE;
    // newtio.c_oflag &= ~ONLCR;

		tcflush(fd,TCIFLUSH);
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
	
	//	printf("set done!\n\r");
	return 0;
}
 
