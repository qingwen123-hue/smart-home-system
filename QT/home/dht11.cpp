#include "dht11.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

#define FILENAME "/dev/dht11"

void DHT11_getValue(int &temp,int &humi)
{
    int fd;
    unsigned char databuf[2];
    fd = open(FILENAME,O_RDWR);
    read(fd, &databuf, sizeof(databuf));
    //printf("databuf[0]: %d databuf[1]: %d \n", databuf[0],databuf[1]);
    temp = databuf[0];
    humi = databuf[1];
}
