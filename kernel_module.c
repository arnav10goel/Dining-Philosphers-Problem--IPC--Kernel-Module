#include <linux/module.h>     
#include <linux/kernel.h>     
#include <linux/init.h>       

MODULE_LICENSE("GPL");        // The license  under which the module is distributed.
MODULE_AUTHOR("Arnav Goel");// The author of the module.
MODULE_DESCRIPTION("HelloWorld Linux Kernel Module."); // The Description of the module.


// This function defines what happens when this module is inserted into the kernel.
// ie. when you run insmod command.
static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world!\n");
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

// This function defines what happens when this module is removed from the kernel.
// ie.when you run rmmod command.
static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);      // Registers the __init function for the module.
module_exit(hello_cleanup);   // Registers the __exit function for the module.