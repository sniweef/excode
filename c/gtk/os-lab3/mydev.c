#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");


static int init_mydev(void);
static void exit_mydev(void);
static int my_open(struct inode *inode, struct file *filp);
static int my_release(struct inode *inode, struct file *filp);
static ssize_t my_read(struct file *file, char *buf, size_t
            len, loff_t *off); 
static ssize_t my_write(struct file *file, const char *buf,
          size_t len, loff_t *off);

struct file_operations mydev_op = {
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write
};

module_init(init_mydev);
module_exit(exit_mydev);

#define MODULE_NAME "mydev"
#define BUFFER_SIZE 8

int mydev_major = 0;
char *buffer;

static int init_mydev(void) 
{
    int result = register_chrdev(mydev_major, MODULE_NAME, &mydev_op);

    if (result < 0) {
        printk("mydev: cannot obtain major number %d.\n", mydev_major);
        return result;
    }

    buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (buffer == NULL) {
        printk("mydev: kmalloc failed.\n");
        result = -ENOMEM;
        exit_mydev();
        return result;
    }

    memset(buffer, 0, BUFFER_SIZE);
    printk("mydev: init mydev successfully.\n");
    return 0;
}
static void exit_mydev(void)
{
    unregister_chrdev(mydev_major, MODULE_NAME);
    if (buffer != NULL)
        kfree(buffer);
    printk("mydev: remove mydev module\n");
}
static int my_open(struct inode *inode, struct file *filp)
{
    return 0;
}
static int my_release(struct inode *inode, struct file *filp)
{
    return 0;
}
static ssize_t my_read(struct file *f, char *user_buf, size_t
            len, loff_t *off) 
{
    int n = (BUFFER_SIZE > len) ? len : BUFFER_SIZE;
    printk("mydev: begin read %d bytes.\n", n);
    if (copy_to_user(user_buf, buffer, n) != 0)
        return -EFAULT;
    if (*off == 0) {
        *off += n;
        return n;
    } else {
        return 0;
    }
}
static ssize_t my_write(struct file *f, const char *user_buf,
          size_t len, loff_t *off)
{
    int n = (BUFFER_SIZE > len) ? len : BUFFER_SIZE;
    printk("mydev: begin write.\n");
    
    if (copy_from_user(buffer, user_buf, n) != 0)
        return -EFAULT;
    return n;
}
