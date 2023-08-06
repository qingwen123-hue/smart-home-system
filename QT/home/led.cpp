#include "led.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

#define FILENAME "/dev/gpioled"

//void LED_Init()
//{

//}

void LED_ON()
{
    unsigned char s = LEDON;
    int fd;
    fd = open(FILENAME,O_RDWR);
    write(fd,&s,sizeof (s));
    ::close(fd);
}

void LED_OFF()
{
    unsigned char s = LEDOFF;
    int fd;
    fd = open(FILENAME,O_RDWR);
    write(fd,&s,sizeof (s));
    ::close(fd);
}

unsigned char Led_GetState()
{
    unsigned char s;
    int fd;
    fd = open(FILENAME,O_RDWR);
    read(fd, &s, sizeof(s));
    ::close(fd);
    return s;
}
