#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define CHAR_DEVICE_NAME "myChardev"

static dev_t deviceNum;
static struct cdev chardev;

static int dev_open(struct inode *inode, struct file *file){
    printk(KERN_INFO "Open device.\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *file){
    printk(KERN_INFO "Release device.\n");
    return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
};

static int __init dev_init(void){
    // Register for device number
    int ret = alloc_chrdev_region(&deviceNum, 0, 1, CHAR_DEVICE_NAME);
    if (ret < 0){
        printk(KERN_ERR "Failed to allocate device number.\n");
        return ret;
    }
    printk(KERN_INFO "Device major number is %d, minor number is %d.\n", MAJOR(deviceNum), MINOR(deviceNum));

    // Init cdev: connect to api
    cdev_init(&chardev, &fops);
    chardev.owner = THIS_MODULE;

    // Add cdev: register it with the allocated device number
    ret = cdev_add(&chardev, deviceNum, 1);
    if (ret < 0){
        unregister_chrdev_region(deviceNum, 1);
        printk(KERN_ERR "Failed to add cdev.\n");
        return ret;
    }
    printk(KERN_INFO "Chardev: add cdev successfully.\n");
    return 0;
}

static void __exit dev_exit(void){
    cdev_del(&chardev);
    unregister_chrdev_region(deviceNum, 1);
    printk(KERN_INFO "Chardev: removed.\n");
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rain");
MODULE_DESCRIPTION("cdev and file operation example");
