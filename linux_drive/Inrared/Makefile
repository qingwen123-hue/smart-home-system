KERNELDIR := /home/ykl/linux/ykl_linux
CURRENT_PATH := $(shell pwd)

obj-m := Inrared.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
