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
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: imx6uirqApp.c
作者	  	: 左忠凯
版本	   	: V1.0
描述	   	: 定时器测试应用程序
其他	   	: 无
使用方法	：./imx6uirqApp /dev/imx6uirq 打开测试App
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/7/26 左忠凯创建
***************************************************************/
enum IR_KEY 
{
	IR_0 = 0x98,
	IR_1 = 0xA2,
	IR_2 = 0x62,
	IR_3 = 0xE2,
	IR_4 = 0x22,
	IR_5 = 0x02,
	IR_6 = 0xC2,
	IR_7 = 0xE0,
	IR_8 = 0xA8,
	IR_9 = 0x90,
	IR_up =  0x18,
	IR_down	 = 0x4A,
	IR_left = 0x10,
	IR_right = 0x5A,
	IR_ok = 0x38,
	IR_xi = 0x68,
	IR_ji = 0xB0
};

#define IS_RI_KEY(PERIPH) (((PERIPH) == IR_0) || \
							((PERIPH) == IR_1) || \
							((PERIPH) == IR_2) || \
							((PERIPH) == IR_3) || \
							((PERIPH) == IR_4) || \
							((PERIPH) == IR_5) || \
							((PERIPH) == IR_6) || \
							((PERIPH) == IR_7) || \
							((PERIPH) == IR_8) || \
							((PERIPH) == IR_9) || \
							((PERIPH) == IR_up) || \
							((PERIPH) == IR_down) || \
							((PERIPH) == IR_left) || \
							((PERIPH) == IR_right) || \
							((PERIPH) == IR_ok) || \
							((PERIPH) == IR_xi) || \
							((PERIPH) == IR_ji) )
/*
 * @description		: main主程序
 * @param - argc 	: argv数组元素个数
 * @param - argv 	: 具体参数
 * @return 			: 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
	int fd;
	int ret = 0;
	//int data = 0;
	unsigned char data;
	char *filename;
	//unsigned char data;
	
	if (argc != 2) {
		printf("Error Usage!\r\n");
		return -1;
	}

	filename = argv[1];
	fd = open(filename, O_RDWR);
	if (fd < 0) {
		printf("Can't open file %s\r\n", filename);
		return -1;
	}

	while (1) 
	{
		printf("A:OK\n");
		ret = read(fd, &data, sizeof(data));
		printf("A:%X\r\n", data);
		if (ret < 0) {  /* 数据读取错误或者无效 */
			
		} 
		else {		/* 数据读取正确 */
			// if (data)	/* 读取到数据 */
			
			switch (data)
			{
				case IR_0:
					printf("IR_0 dowm\n");
					break;
				case IR_1:
					printf("IR_1 dowm\n");
					break;
				case IR_2:
					printf("IR_2 dowm\n");
					break;
				case IR_3:
					printf("IR_3 dowm\n");
					break;
				case IR_4:
					printf("IR_4 dowm\n");
					break;
				case IR_5:
					printf("IR_5 dowm\n");
					break;
				case IR_6:
					printf("IR_6 dowm\n");
					break;
				case IR_7:
					printf("IR_7 dowm\n");
					break;
				case IR_8:
					printf("IR_8 dowm\n");
					break;
				case IR_9:
					printf("IR_9 dowm\n");
					break;
				case IR_up:
					printf("IR_up dowm\n");
					break;
				case IR_down:
					printf("IR_down dowm\n");
					break;
				case IR_left:
					printf("IR_left dowm\n");
					break;
				case IR_right:
					printf("IR_right dowm\n");
					break;
				case IR_ok:
					printf("IR_ok dowm\n");
					break;
				case IR_xi:
					printf("IR_xi dowm\n");
					break;
				case IR_ji:
					printf("IR_ji dowm\n");
					break;
				default:
					break;
			}
		}
		sleep(1);
		
	}
	close(fd);
	return ret;
}
