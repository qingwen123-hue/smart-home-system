#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/of_irq.h>
#include <linux/irq.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

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

unsigned char InfraredDataRecv(void);
unsigned int InfraredRecvLowTime(void);
unsigned int InfraredRecvHighTime(void);

#define IMX6UIRQ_CNT		1			/* 设备号个数 	*/
#define IMX6UIRQ_NAME		"Inrared"	/* 名字 		*/
//#define KEY0VALUE			0X01		/* KEY0按键值 	*/
//#define INVAKEY				0XFF		/* 无效的按键值 */
//#define KEY_NUM				1			/* 按键数量 	*/




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

/* 中断IO描述结构体 */
struct irq_Inrareddesc {
	int gpio;								/* gpio */
	int irqnum;								/* 中断号     */
	//unsigned char value;					/* 按键对应的键值 */
	char name[10];							/* 名字 */
	irqreturn_t (*handler)(int, void *);	/* 中断服务函数 */
};

/* imx6uirq设备结构体 */
struct imx6uirq_dev{
	dev_t devid;			/* 设备号 	 */
	struct cdev cdev;		/* cdev 	*/
	struct class *class;	/* 类 		*/
	struct device *device;	/* 设备 	 */
	int major;				/* 主设备号	  */
	int minor;				/* 次设备号   */
	struct device_node	*nd; /* 设备节点 */
	struct irq_Inrareddesc irqdesc;
	// atomic_t keyvalue;		/* 有效的按键键值 */
	// atomic_t releasekey;	/* 标记是否完成一次完成的按键，包括按下和释放 */
	// struct timer_list timer;/* 定义一个定时器*/
	// struct irq_Inrareddesc irqdesc[KEY_NUM];	/* 按键描述数组 */
	// unsigned char curkeynum;				/* 当前的按键号 */
};

struct imx6uirq_dev imx6uirq;	/* irq设备 */
unsigned char curRecData,preRecData;

int flag;
unsigned char recData;
static irqreturn_t Inraed_irq_handler(int irq, void *dev_id)
{
	unsigned char data;
	struct imx6uirq_dev *dev = (struct imx6uirq_dev *)dev_id;
	disable_irq_nosync(dev->irqdesc.irqnum);
	// mdelay(50);
	//printk("ok\n");
	
	data = InfraredDataRecv();
	//if(data != 0)printk("n:%X\n",data);
	if(IS_RI_KEY(data))
	{
		recData = data;
		flag = 0;
	}
	mdelay(10);
	enable_irq(dev->irqdesc.irqnum);
	
	return IRQ_RETVAL(IRQ_HANDLED);
}

/*
 * @description		: 打开设备
 * @param - inode 	: 传递给驱动的inode
 * @param - filp 	: 设备文件，file结构体有个叫做private_data的成员变量
 * 					  一般在open的时候将private_data指向设备结构体。
 * @return 			: 0 成功;其他 失败
 */
static int imx6uirq_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &imx6uirq;	/* 设置私有数据 */
	flag = 1;
	return 0;
}

 /*
  * @description     : 从设备读取数据 
  * @param - filp    : 要打开的设备文件(文件描述符)
  * @param - buf     : 返回给用户空间的数据缓冲区
  * @param - cnt     : 要读取的数据长度
  * @param - offt    : 相对于文件首地址的偏移
  * @return          : 读取的字节数，如果为负值，表示读取失败
  */
static ssize_t imx6uirq_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	int ret = 0;
	// unsigned char keyvalue = 0;
	// unsigned char releasekey = 0;
	// struct imx6uirq_dev *dev = (struct imx6uirq_dev *)filp->private_data;

	if(flag == 0)
	{
		ret = copy_to_user(buf, &recData, sizeof(recData));
		flag = 1;
	}
	else
	{
		unsigned char tem = 0xff;
		ret = copy_to_user(buf, &tem, sizeof(tem));
	}
	
// 	keyvalue = atomic_read(&dev->keyvalue);
// 	releasekey = atomic_read(&dev->releasekey);

// 	if (releasekey) { /* 有按键按下 */	
// 		if (keyvalue & 0x80) {
// 			keyvalue &= ~0x80;
// 			ret = copy_to_user(buf, &keyvalue, sizeof(keyvalue));
// 		} else {
// 			goto data_error;
// 		}
// 		atomic_set(&dev->releasekey, 0);/* 按下标志清零 */
// 	} else {
// 		goto data_error;
// 	}
	return ret;

}

/* 设备操作函数 */
static struct file_operations imx6uirq_fops = {
	.owner = THIS_MODULE,
	.open = imx6uirq_open,
	.read = imx6uirq_read,
};

/*
 * @description	: 驱动入口函数
 * @param 		: 无
 * @return 		: 无
 */
static int __init imx6uirq_init(void)
{
	int ret;
	/* 1、构建设备号 */
	if (imx6uirq.major) {
		imx6uirq.devid = MKDEV(imx6uirq.major, 0);
		register_chrdev_region(imx6uirq.devid, IMX6UIRQ_CNT, IMX6UIRQ_NAME);
	} else {
		alloc_chrdev_region(&imx6uirq.devid, 0, IMX6UIRQ_CNT, IMX6UIRQ_NAME);
		imx6uirq.major = MAJOR(imx6uirq.devid);
		imx6uirq.minor = MINOR(imx6uirq.devid);
	}

	/* 2、注册字符设备 */
	cdev_init(&imx6uirq.cdev, &imx6uirq_fops);
	cdev_add(&imx6uirq.cdev, imx6uirq.devid, IMX6UIRQ_CNT);

	/* 3、创建类 */
	imx6uirq.class = class_create(THIS_MODULE, IMX6UIRQ_NAME);
	if (IS_ERR(imx6uirq.class)) {
		return PTR_ERR(imx6uirq.class);
	}

	/* 4、创建设备 */
	imx6uirq.device = device_create(imx6uirq.class, NULL, imx6uirq.devid, NULL, IMX6UIRQ_NAME);
	if (IS_ERR(imx6uirq.device)) {
		return PTR_ERR(imx6uirq.device);
	}

	imx6uirq.nd = of_find_node_by_path("/Inrared");
	if (imx6uirq.nd== NULL){
		printk("Inrared node not find!\r\n");
		return -EINVAL;
	} 
	
	imx6uirq.irqdesc.gpio = of_get_named_gpio(imx6uirq.nd,"Inrared-gpio",0);
	if(imx6uirq.irqdesc.gpio < 0)
	{
		printk("can't get Inrared gpio\r\n");
	}

	memset(imx6uirq.irqdesc.name, 0, sizeof(imx6uirq.irqdesc.name));	/* 缓冲区清零 */
	sprintf(imx6uirq.irqdesc.name, "Inrared");		/* 组合名字 */
	gpio_request(imx6uirq.irqdesc.gpio, imx6uirq.irqdesc.name);
	gpio_direction_input(imx6uirq.irqdesc.gpio);	
	imx6uirq.irqdesc.irqnum = irq_of_parse_and_map(imx6uirq.nd, 0);
	printk("Inrared:gpio=%d, irqnum=%d\r\n", imx6uirq.irqdesc.gpio, imx6uirq.irqdesc.irqnum);

	/* 申请中断 */
	imx6uirq.irqdesc.handler = Inraed_irq_handler;
	//imx6uirq.irqdesc.value = KEY0VALUE;

	ret = request_irq(imx6uirq.irqdesc.irqnum, imx6uirq.irqdesc.handler, IRQF_TRIGGER_FALLING, imx6uirq.irqdesc.name, &imx6uirq);
	if(ret < 0){
		printk("irq %d request failed!\r\n", imx6uirq.irqdesc.irqnum);
		return -EFAULT;
	}
	// /* 5、初始化按键 */
	// atomic_set(&imx6uirq.keyvalue, INVAKEY);
	// atomic_set(&imx6uirq.releasekey, 0);
	// keyio_init();
	return 0;
}

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static void __exit imx6uirq_exit(void)
{
	free_irq(imx6uirq.irqdesc.irqnum, &imx6uirq);
	// unsigned int i = 0;
	// /* 删除定时器 */
	// del_timer_sync(&imx6uirq.timer);	/* 删除定时器 */
		
	// /* 释放中断 */
	// for (i = 0; i < KEY_NUM; i++) {
	// 	free_irq(imx6uirq.irqdesc[i].irqnum, &imx6uirq);
	// }
	cdev_del(&imx6uirq.cdev);
	unregister_chrdev_region(imx6uirq.devid, IMX6UIRQ_CNT);
	device_destroy(imx6uirq.class, imx6uirq.devid);
	class_destroy(imx6uirq.class);
}


unsigned char InfraredDataRecv(void)
{
	unsigned int time;
    unsigned char i,j;
    unsigned char data[4] = {0};
	//ktime_t start,diff;
    //获取引导 ??
    time = InfraredRecvLowTime();
    if(time < 400 || time >= 550)
	{
        return 0x00;
	}
	ndelay(1080000);
	//start = ktime_get();
	//printk("1:time:%d\n",time);
	// diff = ktime_sub(ktime_get(), start);
	// printk("run:%lld ns\n",ktime_to_ns(diff));
    time = InfraredRecvHighTime();
	ndelay(1080000);
	//printk("2:time:%d\n",time);
	// if(time < 100)
	// {
	// 	return 0xff;//长按
	// }
	// else if(time > 100)
	// {
	// 	return 0x01;
	// }
    if(time < 150 || time > 250)
        return 0x00;
    //获取数据
    for(i = 0; i < 4 ; i++)
    {
        for(j = 0 ; j < 8 ;j ++)
        {
            time = InfraredRecvLowTime();
			//printk("3:time:%d\n",time);
            // if(time < 20 || time > 35)
			// {
			// 	return 0x03;
			// }
            time = InfraredRecvHighTime();   //通过高电平的时间长短来判断数 ??0,1
			ndelay(1080000);
			//printk("3:time:%d\n",time);
			//udelay(10);
			if(time < 30)   //数据0
            {
                data[i] <<= 1;
                data[i] |=0x0;
            }
            else
            {
                data[i] <<= 1;
                data[i] |= 0x1;
            }
			
        }
    }
	//if(data[2] != ~data[3])return 0x00;
    return data[2];
}

unsigned int InfraredRecvLowTime(void)
{
	unsigned int time = 0;
    while(gpio_get_value(imx6uirq.irqdesc.gpio) == 0)
    {
        if(time >= 550)
            break;
        time +=1;
        udelay(17);
		//delay_us(17);
    }
    return time;
}

unsigned int InfraredRecvHighTime(void)
{
	unsigned int time = 0;
    while(gpio_get_value(imx6uirq.irqdesc.gpio) == 1)
    {
        if(time >= 250)
            break;
        time +=1;
		udelay(17);
        //delay_us(17);
    }
    return time;
}

module_init(imx6uirq_init);
module_exit(imx6uirq_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ykl");
