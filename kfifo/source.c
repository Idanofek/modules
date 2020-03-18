#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/kfifo.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SpiderPig");
MODULE_DESCRIPTION("Module for using kfifo");
MODULE_VERSION("0.0.1");

#define FIFO_SIZE (1024)
#define EXAMPLE_MESSAGE "Spider pig, spider pig!"

static struct kfifo fifo;

static void __print_kfifo_details(struct kfifo *fifo)
{
	printk(KERN_INFO "kfifo size: %d\n", kfifo_size(fifo));
	printk(KERN_INFO "kfifo length: %d\n", kfifo_len(fifo));
	printk(KERN_INFO "kfifo available size: %d\n", kfifo_avail(fifo));
}

static int __init mod_init(void) {
	printk(KERN_INFO "Initializing mod\n");

	const char* buffer = {EXAMPLE_MESSAGE};
	int buffer_size = strlen(buffer);

	// Initialize the kfifo
	int kfifo_alloc_ret = kfifo_alloc(&fifo, FIFO_SIZE, GFP_KERNEL);
	if (kfifo_alloc_ret) {
		printk(KERN_ALERT "Failed to allocate fifo (%d)\n", kfifo_alloc_ret);
		return -EINVAL;
	}

	printk(KERN_INFO "kfifo details BEFORE enqueuing...\n");
	__print_kfifo_details(&fifo);

	int kfifo_in_ret = kfifo_in(&fifo, buffer, buffer_size);
	if (buffer_size != kfifo_in_ret) {
		printk(KERN_ALERT "Failed to enqueue to fifo (%d)\n", kfifo_in_ret);
		return -EINVAL;
	}

	printk(KERN_INFO "kfifo details AFTER enqueuing...\n");
	__print_kfifo_details(&fifo);

	return 0;
}

static void __exit mod_exit(void) {
	char* buffer[FIFO_SIZE] = { 0x0 };

	int kfifo_out_ret = kfifo_out(&fifo, buffer, FIFO_SIZE);
	if (!kfifo_out_ret) {
		printk(KERN_ALERT "Failed to dequeue to fifo (%d)\n", kfifo_out_ret);
		return -EINVAL;
	}

	printk(KERN_INFO "kfifo details AFTER dequeuing...\n");
	__print_kfifo_details(&fifo);

	printk(KERN_INFO "Bb madafaka\n");
}

module_init(mod_init);
module_exit(mod_exit);
