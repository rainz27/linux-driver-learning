#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define DEVICE_DRIVER "chardevDriver"
#define DEVICE_CLASS "chardevClass"
#define DEVICE_NAME "chardevInstance"

static dev_t deviceNum;
static struct cdev chardev;
static struct class *chardevClass;
static struct device *chardevDevice;

// Define open and close API functions
static int dev_open(struct inode* inode, struct file* file){
    printk(KERN_INFO "Open the device.\n");
    return 0;
}

static int dev_close(struct inode* inode, struct file* file){
    printk(KERN_INFO "Close the device.\n");
    return 0;
}

// Init file operations struct
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_close,
};

// Device init and exit
static int __init dev_init(void){
    // Register a device number
    int ret = alloc_chrdev_region(&deviceNum, 0, 1, DEVICE_DRIVER);
    if (ret < 0){
        printk(KERN_ERR "Failed to allocate a device number for chardev.\n");
        return ret;
    }

    // Init cdev with fops
    cdev_init(&chardev, &fops);
    chardev.owner = THIS_MODULE;

    // Add cdev with device number
    ret = cdev_add(&chardev, deviceNum, 1);
    if (ret < 0){
        printk(KERN_ERR "Failed to add chardev.\n");
        unregister_chrdev_region(deviceNum, 1);
        return ret;
    }

    // Create class for chardev
    chardevClass = class_create(DEVICE_CLASS);
    if(IS_ERR(chardevClass)){
        printk(KERN_ERR "Failed to create the class for chardev.\n");
        cdev_del(&chardev);
        unregister_chrdev_region(deviceNum, 1);
        return PTR_ERR(chardevClass);
    }

    // Creatae device for chardev
    chardevDevice = device_create(chardevClass, NULL, deviceNum, NULL, DEVICE_NAME);
    if (IS_ERR(chardevDevice)){
        printk(KERN_ERR "Failed to create the device instance for chardev.\n");
        class_destroy(chardevClass);
        cdev_del(&chardev);
        unregister_chrdev_region(deviceNum, 1);
        return PTR_ERR(chardevDevice);
    }

    return 0;
}

static void __exit dev_exit(void){
    device_destroy(chardevClass, deviceNum);
    class_destroy(chardevClass);
    cdev_del(&chardev);
    unregister_chrdev_region(deviceNum, 1);
    printk(KERN_INFO "Removed chardev successfully!\n");
    return;
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_AUTHOR("Rain");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("An example for class and device creation");