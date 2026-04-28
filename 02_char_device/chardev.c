#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>      // file_operations structure
#include <linux/uaccess.h> // copy_to_user/copy_from_user

#define DEVICE_NAME "cahrdev"
#define BUFFER_SIZE 256

static int major_number;  // kernel equipment number
static char buffer[BUFFER_SIZE] = {0}; // kernel-side buffer
static int buffer_len = 0; // buffer area length

// Called when user opens /dev/chardev
static int dev_open(struct inode *inode, struct file *file){
	printk(KERN_INFO "chardev: device opened\n");
	return 0;
}

// Called when user closes /dev/chardev
static int dev_release(struct inode *inode, struct file *file){
	printk(KERN_INFO "chardev: device closed\n");
	return 0;
}

// Called when user calls read() -- send kernel buffer to user
static ssize_t dev_read(struct file *file, char __user *user_buf, size_t len, loff_t *offset){
	if (*offset >= buffer_len)
		return 0;

	int bytes_to_read = min((int)len,(int)(buffer_len - *offset));

	// Copy to user: safely copy from kernel space to user space
	if (copy_to_user(user_buf, buffer + *offset, bytes_to_read))
		return -EFAULT;

	*offset += bytes_to_read;

	printk(KERN_INFO "chardev: sent %d bytes to user\n", bytes_to_read);
	return bytes_to_read;
}

// Called when user calls write() -- store user data in kernel buffer
static ssize_t dev_write(struct file *file, const char __user *user_buf, size_t len, loff_t *offset){
	buffer_len = min((int)len, BUFFER_SIZE - 1);

	// Copy from user: safely copy from user space to kernel space
	if (copy_from_user(buffer, user_buf, buffer_len))
		return -EFAULT;

	buffer[buffer_len] = '\0';
	printk(KERN_INFO "chardev: received %d bytes: %s\n", buffer_len, buffer);
	return buffer_len;
}

// Register function pointers for open/release/read/write
static struct file_operations fops = {
	.open = dev_open,
	.release = dev_release,
	.read = dev_read,
	.write = dev_write,
};

static int __init chardev_init(void){
	// Request a major number from the kernel (0 = auto-assign)
	major_number = register_chrdev(0, DEVICE_NAME, &fops);
	if (major_number < 0){
		printk(KERN_ALERT "chardev: registration failed, error %d\n", major_number);
		return major_number;
	}
	printk(KERN_INFO "chardev: registered with major number %d\n", major_number);
	printk(KERN_INFO "chardev: run -> sudo mknod /dev/chardev c %d 0\n", major_number);
	return 0;
}

static void __exit chardev_exit(void){
	unregister_chrdev(major_number, DEVICE_NAME);
	printk(KERN_INFO "chardev: unregistered\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rain");
