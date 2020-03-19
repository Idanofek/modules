#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SpiderPig");
MODULE_DESCRIPTION("Module for using irq")
MODULE_VERSION("0.0.1");

#define IRQN (1)
#define DEVICE_NAME "spiderpig"

struct my_struct {
	long value1,
	long value2,
};

static struct my_struct *struct = NULL;

static irqreturn_t intr_handler(int irq, void *dev) 
{
	if (IRQN != irq) {
		return IRQ_NONE;
	}

	struct given_struct = *((struct my_struct*)dev);
	printk(KERN_INFO "value1: %d\n", given_struct.value1);
	printk(KERN_INFO "value2: %d\n", given_struct.value2);

	return IRQN_HANDLED;
}

static int __init mod_init(void) 
{
	printk(KERN_INFO "Initializing mod\n");

	my_struct = (struct my_struct*)kmalloc(sizeof(struct my_struct));

	if (request_irq(IRQN, inter_handler, IRQF_SHARED, DEVICE_NAME, (void*)my_struct)) {
		printk(KERN_ERR DEVICE_NAME#": cannot register IRQ :%d\n", IRQN);
		return -EIO;
	}


	return 0;
}

static void __exit mod_exit(void) 
{
	free_irq(IRQN, (void*)my_struct);
	kfree(my_struct);
	printk(KERN_INFO "Bb madafaka\n");
}

module_init(mod_init);
module_exit(mod_exit);
