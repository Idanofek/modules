#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SpiderPig");
MODULE_DESCRIPTION("Simulates a character device");
MODULE_VERSION("0.0.1a");

#define DEVICE_NAME "devm"
#define MESSAGE_BUFFER_LENGTH (56)
#define EXAMPLE_MSG "Spider pig, spider pig, does whatever spider pig does.\n"

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static int major_num;
static int device_open_count = 0;
static char msg_buffer[MESSAGE_BUFFER_LENGTH];
static char* msg_ptr;

static struct file_operations file_ops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

static ssize_t device_read(struct file *fs, char *buffer, size_t len, loff_t *offset)
{
	int bytes_read = 0;
	if (0 == *msg_ptr) {
		msg_ptr = msg_buffer;
	}

	while (len && *msg_ptr) {
		put_user(*(msg_ptr++), buffer++);
		len = len - 1;
		bytes_read++;
	}

	return bytes_read;
}

static ssize_t device_write(struct file *fs, const char *buffer, size_t len, loff_t *offset) 
{
	printk(KERN_ALERT "This operation is not supported.\n");
	return -EINVAL;
}

static int device_open(struct inode *inode, struct file *file) 
{
	if (device_open_count) {
		return -EBUSY;
	}

	device_open_count++;
	try_module_get(THIS_MODULE);
	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	device_open_count--;
	module_put(THIS_MODULE);
	return 0;
}

static int __init mod_init(void) {
	printk(KERN_INFO "Initializing mod\n");
	strncpy(msg_buffer, EXAMPLE_MSG, MESSAGE_BUFFER_LENGTH);
	msg_ptr = msg_buffer;
	major_num = register_chrdev(0, DEVICE_NAME, &file_ops);
	if (major_num < 0) {
		printk(KERN_ALERT "failed to load devm device [%d]\n", major_num);
		return major_num;
	} else {
		printk(KERN_INFO "devm module loaded with device maj. num %d\n", major_num);
		return 0;
	}
}

static void __exit mod_exit(void) {
	unregister_chrdev(major_num, DEVICE_NAME);
	printk(KERN_INFO "Bb madafaka\n");
}

module_init(mod_init);
module_exit(mod_exit);
