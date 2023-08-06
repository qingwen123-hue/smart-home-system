#include "inraredthread.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include "linux/ioctl.h"
#include <time.h>
#include <unistd.h>

#define FILENAME "/dev/Inrared"
int fd;
InraredThread::InraredThread()
{

}

InraredThread::~InraredThread()
{
    ::close(fd);
}

void InraredThread::run()
{
    
    unsigned char data;
    
    fd = open(FILENAME,O_RDWR);
    while (1)
    {
       read(fd, &data, sizeof(data)); 
       if(IS_RI_KEY(data))
       {
           emit SendReceive(data);
       }
       QThread::msleep(100);
    }
    
}
