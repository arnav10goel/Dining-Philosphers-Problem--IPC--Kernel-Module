#include <linux/module.h>     
#include <linux/kernel.h>     
#include <linux/init.h>
#include <linux/sched.h>   
#include <linux/stat.h>   
#include <linux/moduleparam.h> 
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/unistd.h>
#include <linux/cred.h>
#include <linux/fs.h>
#include <linux/path.h>
#include <linux/dcache.h>


MODULE_LICENSE("GPL");       
MODULE_AUTHOR("Arnav Goel");
MODULE_DESCRIPTION("Linux Process Task Struct Reading Module"); // The Description of the module.

int pid_input = 0; // Variable to store Process ID as a command line input from the user

//This is used to take input from the user in the form of a command line argument
module_param(pid_input, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(pid_input, "Process ID of the process whose information is to be printed");

// Function to print the process information
void print_process_info(void)
{
    pid_t pid = (pid_t)pid_input;

    // Get the task_struct of the process
    struct task_struct *task = pid_task(find_vpid(pid), PIDTYPE_PID);

    //Error handling
    if (task == -1) {
        printk(KERN_INFO "Process ID not found: %d\n", pid);
        return;
    }

    // Print the pid, ppid, gpid, and uid of the process
    printk(KERN_INFO "PID of the given process is: %d\n", task->pid);
    printk(KERN_INFO "PPID of the given process is: %d\n", task->real_parent->pid);
    printk(KERN_INFO "GPID of the given process is: %d\n", task->group_leader->pid);
    printk(KERN_INFO "UID of the given process is: %d\n", task->cred->uid);

    // Get the path of the process
    char* path = NULL;
    path = kmalloc(256 * sizeof(char), GFP_KERNEL);
    if (!path) {
        printk(KERN_INFO "Error allocating memory for path\n");
        return;
    }

    // Get the path of the process
    int len = 0;
    len = sprintf(path, "/proc/%d/exe", task->pid);
    path[len] = '\0';

    //Print the path of the process
    printk(KERN_INFO "path: %s\n", path);

    //Free the allocated memory for the path
    kfree(path);
}       


//When you run insmod command, this function is called.
static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world! Entering the Module.\n");
    print_process_info();
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

// When you run rmmod command, this function is called.
static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);      // Registers the __init function for the module.
module_exit(hello_cleanup);   // Registers the __exit function for the module.
