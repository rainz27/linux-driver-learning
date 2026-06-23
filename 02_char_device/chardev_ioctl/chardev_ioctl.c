#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include "chardev_ioctl.h"

#define BUFFER_SIZE 128
static size_t bufferLen;
static char deviceBuf[BUFFER_SIZE];

static dev_t deviceNum;
static struct cdev charDev;
static struct class *chardevClass;
static struct device *chardevDevice;

static int dev_open(struct inode *inode, struct file *file){
    printk(KERN_INFO "Open the device.\n");
    return 0;
}

static int dev_close(struct inode *inode, struct file *file){
    printk(KERN_INFO "Close the device.\n");
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *userBuf, size_t len, loff_t *offset){
    size_t bytes_to_read;
    if (*offset >= bufferLen) return 0; //EOF
    bytes_to_read = min(len, (size_t)(bufferLen - *offset));
    if (copy_to_user(userBuf, deviceBuf + *offset, bytes_to_read)) return -EFAULT;
    *offset += bytes_to_read;
    return bytes_to_read;
}

static ssize_t dev_write(struct file *file, const char __user *userBuf, size_t len, loff_t *offset){
    size_t bytes_to_write;
    bytes_to_write = min(len, (size_t)(BUFFER_SIZE - 1));
    if (copy_from_user(deviceBuf, userBuf, bytes_to_write)) return -EFAULT;
    bufferLen = bytes_to_write;
    deviceBuf[bytes_to_write] = '\0';
    return bytes_to_write;
}

static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
    int len;

    switch(cmd){
        case CHARDEV_IOCTL_CLEAR:
            memset(deviceBuf, 0, BUFFER_SIZE);
            bufferLen = 0;
            printk(KERN_INFO "Cleared buffer.\n");
            return 0;
        case CHARDEV_IOCTL_GETLEN:
            len = (int) bufferLen;
            if (copy_to_user((int __user *)arg, &len, sizeof(len))) return -EFAULT;
            printk(KERN_INFO "Returned buffer length.\n");
            return 0;
        default:
            printk(KERN_ERR "Invalid ioctl command.\n");
            return -ENOTTY;
    }

    return 0;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_close,
    .read = dev_read,
    .write = dev_write,
    .unlocked_ioctl = dev_ioctl,
};

static int __init dev_init(void){
    int ret;
    // Major / Minor
    ret = alloc_chrdev_region(&deviceNum, 0, 1, "chardev"); // proc/dev
    if (ret < 0){
        printk(KERN_ERR "Failed to register the device number.\n");
        return ret;
    }

    // Fops
    cdev_init(&charDev, &fops);
    charDev.owner = THIS_MODULE;

    // Create instance
    ret = cdev_add(&charDev, deviceNum, 1);
    if (ret < 0){
        printk(KERN_ERR "Failed to create device instance.\n");
        unregister_chrdev_region(deviceNum, 1);
        return ret;
    }

    // Create class
    chardevClass = class_create("chardev"); // sys/class
    if (IS_ERR(chardevClass)){
        printk(KERN_ERR "Failed to create device class.\n");
        cdev_del(&charDev);
        unregister_chrdev_region(deviceNum, 1);
        return PTR_ERR(chardevClass);
    }

    // Create device
    chardevDevice = device_create(chardevClass, NULL, deviceNum, NULL, "dev1"); // expose the instance to userspace
    if (IS_ERR(chardevDevice)){
        printk(KERN_ERR "Failed to create device instance.\n");
        class_destroy(chardevClass);
        cdev_del(&charDev);
        unregister_chrdev_region(deviceNum, 1);
        return PTR_ERR(chardevDevice);
    }

    return 0;
}

static void __exit dev_exit(void){
    printk(KERN_INFO "Exit.\n");
    device_destroy(chardevClass, deviceNum);
    class_destroy(chardevClass);
    cdev_del(&charDev);
    unregister_chrdev_region(deviceNum, 1);
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rain");
MODULE_DESCRIPTION("Chardevice");