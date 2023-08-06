#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

/*
 * @description		: main主程序
 * @param - argc 	: argv数组元素个数
 * @param - argv 	: 具体参数
 * @return 			: 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
	int fd, retvalue;
	char *filename;
	unsigned char databuf[2];
	unsigned char aa;
	// if(argc != 3){
	// 	printf("Error Usage!\r\n");
	// 	return -1;
	// }

	filename = argv[1];

	/* 打开led驱动 */
	fd = open(filename, O_RDWR);
	if(fd < 0){
		printf("file %s open failed!\r\n", argv[1]);
		return -1;
	}

    while(1)
    {
        read(fd, &databuf, sizeof(databuf));
        printf("databuf[0]: %d databuf[1]: %d \n", databuf[0],databuf[1]);
		//read(fd, &aa, sizeof(aa));
        //printf("aa: %d\n", aa);
    }
	//databuf[0] = atoi(argv[2]);	/* 要执行的操作：打开或关闭 */
	//printf("argv[2]:%d\n",databuf[0]);
	// if(databuf[0] == 0)
	// {
	// 	databuf[0] = 48;
	// 	write(fd, databuf, sizeof(databuf));
	// }
	// else if(databuf[0] == 1)
	// {
	// 	databuf[0] = 49;
	// 	write(fd, databuf, sizeof(databuf));
	// }
	// else if(databuf[0] == 2)
	// {
	// 	read(fd, &databuf, sizeof(databuf));
	// 	printf("databuf: %d\n", databuf[0]);
	// }

	/* 向/dev/led文件写入数据 */
	// retvalue = write(fd, databuf, sizeof(databuf));
	// if(retvalue < 0){
	// 	printf("LED Control Failed!\r\n");
	// 	close(fd);
	// 	return -1;
	// }

	retvalue = close(fd); /* 关闭文件 */
	if(retvalue < 0){
		printf("file %s close failed!\r\n", argv[1]);
		return -1;
	}
	return 0;
}