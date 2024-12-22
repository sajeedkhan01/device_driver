/***************************************************************************//**
 *  \file       First_driver.c
 *
 *  \details    Simple hello world driver with module parameters, callback, 
 *              and explanation of available parameter types and permissions.
 *
 *  \author     Sajeedkhan
 *
 *******************************************************************************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

int notify_param(const char *val, const struct kernel_param *kp);

int valueETX, arr_valueETX[4];
char *nameETX;
int cb_valueETX = 0;

// Declare module parameters with read and write access (S_IRUSR|S_IWUSR).
module_param(valueETX, int, S_IRUSR|S_IWUSR);           // Integer parameter
module_param(nameETX, charp, S_IRUSR|S_IWUSR);          // String parameter
module_param_array(arr_valueETX, int, NULL, S_IRUSR|S_IWUSR); // Array of integers

/*----------------------Module_param_cb()--------------------------------*/
/**
 * @brief A callback function to handle changes to the cb_valueETX parameter.
 *
 * This function is called whenever the cb_valueETX parameter is modified.
 * It prints a message to the kernel log and shows the new value.
 *
 * @param val Value passed to the callback function
 * @param kp The kernel parameter associated with cb_valueETX
 * @return 0 on success, -1 on failure
 */
int notify_param(const char *val, const struct kernel_param *kp)
{
    int res = param_set_int(val, kp); // Use the helper function for writing an integer value

    if (res == 0) {
        printk(KERN_INFO "Call back function called...\n");
        printk(KERN_INFO "New value of cb_valueETX = %d\n", cb_valueETX);
        return 0; // Successfully handled the parameter change
    }
    return -1; // Failure
}

/**
 * @brief Define custom kernel parameter operations for cb_valueETX.
 *
 * The custom operations specify a setter function (notify_param) and a getter function (param_get_int).
 */
const struct kernel_param_ops my_param_ops = {
    .set = &notify_param,   // Use our custom setter
    .get = &param_get_int,  // Standard getter function
};

// Register the callback parameter cb_valueETX with custom operations
module_param_cb(cb_valueETX, &my_param_ops, &cb_valueETX, S_IRUGO|S_IWUSR);
/*-------------------------------------------------------------------------*/

/**
 * @brief Module initialization function.
 *
 * This function is executed when the kernel module is loaded into the kernel.
 * It prints the values of the parameters and arrays to the kernel log.
 *
 * @return 0 on success, non-zero error code on failure
 */
static int __init hello_world_init(void)
{
    int i;

    // Print the value of the integer and string parameters
    printk(KERN_INFO "ValueETX = %d  \n", valueETX);
    printk(KERN_INFO "cb_valueETX = %d  \n", cb_valueETX);
    printk(KERN_INFO "NameETX = %s \n", nameETX);

    // Loop through the array and print each value
    for (i = 0; i < (sizeof arr_valueETX / sizeof (int)); i++) {
        printk(KERN_INFO "Arr_value[%d] = %d\n", i, arr_valueETX[i]);
    }

    printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
    return 0;
}

/**
 * @brief Module cleanup function.
 *
 * This function is executed when the kernel module is removed from the kernel.
 */
static void __exit hello_world_exit(void)
{
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}

// Register the module initialization and cleanup functions
module_init(hello_world_init);
module_exit(hello_world_exit);

// Module metadata
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sajeedkhan");
MODULE_DESCRIPTION("A simple hello world driver with parameters and callback");
MODULE_VERSION("1.0");



