#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static dev_t deviceNum;

static int __init device_reg(void){
    int ret = alloc_chrdev_region(&deviceNum, 0, 1, "chardev");
    if (ret < 0){
        printk(KERN_INFO "Failed to register char device.\n");
        return ret;
    }
    printk(KERN_INFO "devnum: major = %d minor = %d\n");
    return 0;
}

static void __exit device_unreg(void){
    unregister_chrdev_region(deviceNum, 1);
    printk(KERN_INFO "devnum: device number unregistered.\n");
}

module_init(device_reg);
module_exit(device_unreg);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RAIN");
MODULE_DESCRIPTION("Charactor device number example;");