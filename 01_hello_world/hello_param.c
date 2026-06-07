#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

static char* name = "world";
static int count = 1;

module_param(name, charp, 0644);
MODULE_PARM_DESC(name, "Name to print");

module_param(count, int, 0644);
MODULE_PARM_DESC(count, "Number of times to print");

static int __init hello_init(void)
{
    for (int i = 0; i < count; i++){
        printk(KERN_INFO "hello: Hello, %s\n", name);
    }
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "hello: module unloaded.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rain");
MODULE_DESCRIPTION("Module parameter example");