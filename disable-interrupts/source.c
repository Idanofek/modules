#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SpiderPig");
MODULE_DESCRIPTION("Disables interrupts on cpu");
MODULE_VERSION("0.0.1");

static int __init mod_init(void) 
{
	printk(KERN_INFO "initializing module\n");

	printk(KERN_INFO "Disabling cpu interrupts. WAHAHAH!\n");

	unsigned long flags;
	local_irq_save(flags);
	msleep(3000);
	local_irq_restore(flags);

	return 0;
}

static void __exit mod_exit(void)
{
	printk(KERN_INFO "Bb madafaka\n");
}

module_init(mod_init);
module_exit(mod_exit);
