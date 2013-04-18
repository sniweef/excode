#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/audit.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>

#define FILE_MODE   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
#define BUFFER_SIZE 1024

asmlinkage int sys_mycp(const char *source, const char *dest)
{
    int source_fd, dest_fd;
    char buffer[BUFFER_SIZE];
    int n;
    mm_segment_t new_fs, old_fs;

    new_fs = get_ds();
    old_fs = get_fs();
    set_fs(new_fs);

    if ((source_fd = sys_open(source, O_RDONLY, 0)) < 0) {
        printk("open %s failed.\n", source);
        return -1;
    }
    if ((dest_fd = sys_open(dest, O_WRONLY|O_CREAT|O_TRUNC, FILE_MODE)) < 0) {
        printk("open or create %s failed.\n", dest);
        return -1;
    }

    printk("mycp: begin copy %s to %s\n", source, dest);
    while ((n = sys_read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        printk("mycp: begin write.\n");
        sys_write(dest_fd, buffer, n);
    }

    sys_close(source_fd);
    sys_close(dest_fd);
    printk("mycp: finish.\n");
    set_fs(old_fs);
    return 0;
}
