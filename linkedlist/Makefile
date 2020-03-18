obj-m += source.o
MY_CFLAGS += -g -DDEBUG
ccflags-y += ${MY_CFLAGS}
CC += ${MY_CFLAGS}

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

debug:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules EXTRA_CFLAGS="$(MY_CFLAGS)"

test:
	sudo insmod source.ko
	sudo rmmod source.ko
	dmesg
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(pwd) clean
