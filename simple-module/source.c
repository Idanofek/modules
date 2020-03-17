#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.hi>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Idan Ofek");
MODULE_DESCRIPTION("Simple Module");
MODULE_VERSION(“0.0.1”);

static int __init mod_init(void) {
	printk(KERN_INFO "Sup madafaka\n");
	return 0;
}

static void __exit mod_exit(void) {
	printk(KERN_INFO "Bb madafaka\n");
}

module_init(mod_init);
module_exit(mod_exit);
