#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SpiderPig");
MODULE_DESCRIPTION("Module for using irq");
MODULE_VERSION("0.0.1");

#define IRQN (1)
#define DEVICE_NAME "spiderpig"

struct my_struct {
	long value1;
	long value2;
};

static struct my_struct *my_struct = NULL;

static irqreturn_t intr_handler(int irq, void *dev) 
{
	if (IRQN != irq) {
		return IRQ_NONE;
	}

	const struct my_struct *given_struct = (const struct my_struct*)dev;
	printk(KERN_INFO "value1: %d\n", given_struct->value1);
	printk(KERN_INFO "value2: %d\n", given_struct->value2);

	return IRQ_HANDLED;
}

static int __init mod_init(void) 
{
	printk(KERN_INFO "Initializing mod\n");

	// Free the existing keyboard handler since it cannot co-exist with
	// our module
	free_irq(IRQN, NULL);

	my_struct = (struct my_struct*)kmalloc(sizeof(struct my_struct), GFP_KERNEL);

	if (request_irq(IRQN, intr_handler, IRQF_SHARED, DEVICE_NAME, (void*)my_struct)) {
		printk(KERN_ERR "spiderpig: cannot register IRQ :%d\n", IRQN);
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
