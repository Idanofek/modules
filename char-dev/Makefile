obj-m += source.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
test:
	sudo insmod source.ko
	sudo rmmod source.ko
	dmesg
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(pwd) clean
