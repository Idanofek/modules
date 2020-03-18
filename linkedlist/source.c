#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SpiderPig");
MODULE_DESCRIPTION("Simple Module");
MODULE_VERSION("0.0.1");

#define ITERATION_NUMBER (10)

static struct data {
	unsigned int value;
	struct list_head list;
};

static LIST_HEAD(data_list);

static int __init mod_init(void) {
	printk(KERN_INFO "Initializing mod\n");
	int i = 0;
	for (; i < ITERATION_NUMBER; ++i) {
		struct data *new_data;
		new_data = (struct data*)kmalloc(sizeof(struct data), GFP_KERNEL);
		new_data->value = i;
		INIT_LIST_HEAD(&new_data->list);

		printk(KERN_INFO "Adding node number %d\n", i);
		list_add(&new_data->list, &data_list);
	}

	return 0;
}

static void __exit mod_exit(void) {
	struct list_head *pos, *n;
	struct data *d;

	list_for_each_safe(p, n, &data_list) {
		d = list_entry(p, struct data, list);
		printk(KERN_INFO "Value: %d\n", d->value);
		list_del(p);
		kfree((const void*)d);
	}

	printk(KERN_INFO "Bb madafaka\n");
}

module_init(mod_init);
module_exit(mod_exit);
