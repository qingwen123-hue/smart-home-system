#include "fan.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

#define FILENAME "/dev/fan"

void FAN_ON()
{
    unsigned char s = FANON;
    int fd;
    fd = open(FILENAME,O_RDWR);
    write(fd,&s,sizeof (s));
    ::close(fd);
}

void FAN_OFF()
{
    unsigned char s = FANOFF;
    int fd;
    fd = open(FILENAME,O_RDWR);
    write(fd,&s,sizeof (s));
    ::close(fd);
}

unsigned char FAN_GetState()
{
    unsigned char s;
    int fd;
    fd = open(FILENAME,O_RDWR);
    read(fd, &s, sizeof(s));
    ::close(fd);
    return s;
}
