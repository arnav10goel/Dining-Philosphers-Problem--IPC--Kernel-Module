#include <linux/module.h>     
#include <linux/kernel.h>     
#include <linux/init.h>
#include <linux/sched.h>   // Needed for task_struct
#include <linux/stat.h>    // Needed for the macros
#include <linux/moduleparam.h> // Needed for module_param
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/unistd.h>
#include <linux/cred.h>
#include <linux/fs.h>
#include <linux/path.h>
#include <linux/dcache.h>


MODULE_LICENSE("GPL");        // The license  under which the module is distributed.
MODULE_AUTHOR("Arnav Goel");// The author of the module.
MODULE_DESCRIPTION("Linux Process Task Struct Reading Module"); // The Description of the module.

int pid_input = 0; // Variable to store Process ID

module_param(pid_input, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
// Function to print the process information
void print_process_info(void)
{
    pid_t pid = (pid_t)pid_input;
    // Get the task_struct of the process
    struct task_struct *task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (!task) {
        printk(KERN_INFO "Invalid process ID: %d\n", pid);
        return;
    }

    // Print the pid, ppid, gpid, and uid of the process
    printk(KERN_INFO "pid: %d\n", task->pid);
    printk(KERN_INFO "ppid: %d\n", task->real_parent->pid);
    printk(KERN_INFO "gpid: %d\n", task->group_leader->pid);
    printk(KERN_INFO "uid: %d\n", task->cred->uid);

    // Get the path of the process
    char* path = NULL;
    path = kmalloc(256 * sizeof(char), GFP_KERNEL);
    if (!path) {
        printk(KERN_INFO "Error allocating memory for path\n");
        return;
    }

    //Read the symbolic link to the process's executable
    int len = 0;
    len = sprintf(path, "/proc/%d/exe", task->pid);
    //len = vfs_readlink(path, path, len);
    // if (len == -1) {
    //     printk(KERN_INFO "Error reading link to process executable\n");
    //     kfree(path);
    //     return;
    // }

    path[len] = '\0';

    //Print the path of the process
    printk(KERN_INFO "path: %s\n", path);

    //Free the allocated memory
    kfree(path);
}       


// This function defines what happens when this module is inserted into the kernel.
// ie. when you run insmod command.
static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world! Entering the Module.\n");
    print_process_info();
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
