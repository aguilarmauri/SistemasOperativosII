#include <linux/module.h>   // Needed by all modules
#include <linux/kernel.h>   // Needed for KERN_ALERT
#include <linux/init.h>     // Needed for the macros


static int hello_2_init(void)
{
   printk(KERN_ALERT "Hello, world 2\n");
   return 0;
}


static void hello_2_exit(void)
{
   printk(KERN_ALERT "Goodbye, world 2\n");
}


module_init(hello_2_init);
module_exit(hello_2_exit);

MODULE_LICENSE("GPL");

