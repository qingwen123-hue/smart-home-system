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
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/delay.h>
#define DHT11_CNT			1		  	/* 设备号个数 */
#define DHT11_NAME		"dht11"	/* 名字 */
#define u8 unsigned char
void DHT11_Rst(int gpio);
int DHT11_Check(int gpio);
int DHT11_Read_Bit(int gpio);
u8 DHT11_Read_Byte(int gpio);
u8 DHT11_Read_Data(u8 *temp,u8 *humi,int gpio);

void DHT11_Rst(int gpio)
{
	gpio_direction_output(gpio,0);
	gpio_set_value(gpio,0);
	mdelay(20);
	gpio_set_value(gpio,1);
	udelay(30);
}

int DHT11_Check(int gpio)
{
	int retry = 0;
	gpio_direction_input(gpio);
	while(gpio_get_value(gpio)&&retry<100)
	{
		retry++;
		udelay(1);
	};
	if(retry>=100)return 1;
	else retry=0;
	while (!gpio_get_value(gpio)&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		udelay(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

int DHT11_Read_Bit(int gpio)
{
	int retry=0;
	while(gpio_get_value(gpio)&&retry<100)//等待变为低电平
	{
		retry++;
		udelay(1);
	}
	retry=0;
	while(!gpio_get_value(gpio)&&retry<100)//等待变高电平
	{
		retry++;
		udelay(1);
	}
	udelay(40);//等待40us
	if(gpio_get_value(gpio))return 1;
	else return 0;
}

u8 DHT11_Read_Byte(int gpio)
{
	u8 i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit(gpio);
    }						    
    return dat;
}


u8 DHT11_Read_Data(u8 *temp,u8 *humi,int gpio)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst(gpio);
	if(DHT11_Check(gpio)==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte(gpio);
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
		else
		{
			return 2;
		}
	}
	else
		return 1;
	return 0;	    
}

/* gpioled设备结构体 */
struct dht11_dev{
	dev_t devid;			/* 设备号 	 */
	struct cdev cdev;		/* cdev 	*/
	struct class *class;	/* 类 		*/
	struct device *device;	/* 设备 	 */
	int major;				/* 主设备号	  */
	int minor;				/* 次设备号   */
	struct device_node	*nd; /* 设备节点 */
	int gpio;			/* led所使用的GPIO编号		*/
};

struct dht11_dev dht11;

static int dht11_open(struct inode *inode, struct file *filp)
{
	//int ret = 0;
	filp->private_data = &dht11; /* 设置私有数据 */
	
	return 0;
}

static ssize_t dht11_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	int ret = 0;
	unsigned char value[2];
	int aa;
	struct dht11_dev *dev = filp->private_data;

	value[0] = 0;
	value[1] = 0;
	while(value[0] == 0 || value[1] == 0)
	{
		ret = DHT11_Read_Data(&value[0],&value[1],dev->gpio);
	}
	
	
	//value = atomic_read(&dev->keyvalue); /* 保存按键值 */
	ret = copy_to_user(buf, &value, sizeof(value));
	return ret;
}

/* 设备操作函数 */
static struct file_operations dht11_fops = {
	.owner = THIS_MODULE,
	.open = dht11_open,
	.read = dht11_read,
};

static int __init dht11_init(void)
{
    int ret = 0;
    dht11.nd = of_find_node_by_path("/mydht11");
    if(dht11.nd == NULL)
    {
        printk("dht11 node not find!\r\n");
        return -EINVAL;
    }
    else
    {
        printk("dht11 node find!\r\n");
    }

	dht11.gpio = of_get_named_gpio(dht11.nd,"led-gpio",0);
	if(dht11.gpio < 0)
	{
		printk("can't get gpio");
		return -EINVAL;
	}
	printk("gpio num = %d\r\n",dht11.gpio);

	gpio_request(dht11.gpio,"dht11gpio");
	DHT11_Rst(dht11.gpio);
	while(DHT11_Check(dht11.gpio))
	{
	 	mdelay(400);
	 	printk("check!\r\n");
	 	DHT11_Rst(dht11.gpio);
	}
	/* 注册字符设备驱动 */
	/* 1、创建设备号 */
	if (dht11.major) {		/*  定义了设备号 */
		dht11.devid = MKDEV(dht11.major, 0);
		register_chrdev_region(dht11.devid, DHT11_CNT, DHT11_NAME);
	} else {						/* 没有定义设备号 */
		alloc_chrdev_region(&dht11.devid, 0, DHT11_CNT, DHT11_NAME);	/* 申请设备号 */
		dht11.major = MAJOR(dht11.devid);	/* 获取分配号的主设备号 */
		dht11.minor = MINOR(dht11.devid);	/* 获取分配号的次设备号 */
	}
	printk("dht11 major=%d,minor=%d\r\n",dht11.major, dht11.minor);	

    dht11.cdev.owner = THIS_MODULE;
	cdev_init(&dht11.cdev,&dht11_fops);
	cdev_add(&dht11.cdev,dht11.devid,DHT11_CNT);

	dht11.class = class_create(THIS_MODULE,DHT11_NAME);
	if (IS_ERR(dht11.class)) {
		return PTR_ERR(dht11.class);
	}
/* 5、创建设备 */
	dht11.device = device_create(dht11.class, NULL, dht11.devid, NULL, DHT11_NAME);
	if (IS_ERR(dht11.device)) {
		return PTR_ERR(dht11.device);
	}

	dht11.nd = of_find_node_by_path("/mydht11");
    if(dht11.nd == NULL)
    {
        printk("dht11 node not find!\r\n");
        return -EINVAL;
    }
    else
    {
        printk("dht11 node find!\r\n");
    }

	dht11.gpio = of_get_named_gpio(dht11.nd,"led-gpio",0);
	if(dht11.gpio < 0)
	{
		printk("can't get gpio");
		return -EINVAL;
	}
	printk("gpio num = %d\r\n",dht11.gpio);

	gpio_request(dht11.gpio,"dht11gpio");

	return 0;


}


static void __exit dht11_exit(void)
{
	/* 注销字符设备驱动 */
	cdev_del(&dht11.cdev);/*  删除cdev */
	unregister_chrdev_region(dht11.devid, DHT11_CNT); /* 注销设备号 */

	device_destroy(dht11.class, dht11.devid);
	class_destroy(dht11.class);
}

module_init(dht11_init);
module_exit(dht11_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ykl");
