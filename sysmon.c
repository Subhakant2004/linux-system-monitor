#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/sysinfo.h>
#include <linux/timekeeping.h>
#include <linux/cpu.h>

#define DEVICE_NAME "sysmon"

static dev_t dev_number;
static struct cdev sysmon_cdev;
static struct class *sysmon_class;
static struct device *sysmon_device;

static int sysmon_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "SYSMON: device opened\n");
    return 0;
}

static int sysmon_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "SYSMON: device closed\n");
    return 0;
}

static ssize_t sysmon_read(struct file *file, char __user *buffer,
                           size_t length, loff_t *offset)
{
    char message[512];
    int message_length;
    struct sysinfo info;
    unsigned long uptime;
    unsigned int cpu_count;

    if (*offset > 0)
        return 0;

    si_meminfo(&info);

    uptime = ktime_get_boottime_seconds();
    cpu_count = num_online_cpus();

    message_length = snprintf(
        message,
        sizeof(message),
        "===== KERNEL SYSMON =====\n"
        "Online CPUs    : %u\n"
        "Kernel Uptime  : %lu seconds\n"
        "Total Memory   : %lu MB\n"
        "Free Memory    : %lu MB\n"
        "=========================\n",
        cpu_count,
        uptime,
        (info.totalram * (unsigned long)PAGE_SIZE) / (1024 * 1024),
        (info.freeram * (unsigned long)PAGE_SIZE) / (1024 * 1024)
    );

    if (length < message_length)
        message_length = length;

    if (copy_to_user(buffer, message, message_length))
        return -EFAULT;

    *offset += message_length;

    return message_length;
}

static struct file_operations sysmon_fops = {
    .owner = THIS_MODULE,
    .open = sysmon_open,
    .read = sysmon_read,
    .release = sysmon_release,
};

static int __init sysmon_init(void)
{
    int result;

    result = alloc_chrdev_region(&dev_number, 0, 1, DEVICE_NAME);
    if (result < 0)
        return result;

    cdev_init(&sysmon_cdev, &sysmon_fops);

    result = cdev_add(&sysmon_cdev, dev_number, 1);
    if (result < 0) {
        unregister_chrdev_region(dev_number, 1);
        return result;
    }

    sysmon_class = class_create(DEVICE_NAME);

    if (IS_ERR(sysmon_class)) {
        cdev_del(&sysmon_cdev);
        unregister_chrdev_region(dev_number, 1);
        return PTR_ERR(sysmon_class);
    }

    sysmon_device = device_create(
        sysmon_class,
        NULL,
        dev_number,
        NULL,
        DEVICE_NAME
    );

    if (IS_ERR(sysmon_device)) {
        class_destroy(sysmon_class);
        cdev_del(&sysmon_cdev);
        unregister_chrdev_region(dev_number, 1);
        return PTR_ERR(sysmon_device);
    }

    printk(KERN_INFO "SYSMON: module loaded\n");
    printk(KERN_INFO "SYSMON: /dev/sysmon created\n");

    return 0;
}

static void __exit sysmon_exit(void)
{
    device_destroy(sysmon_class, dev_number);
    class_destroy(sysmon_class);
    cdev_del(&sysmon_cdev);
    unregister_chrdev_region(dev_number, 1);

    printk(KERN_INFO "SYSMON: module unloaded\n");
}

module_init(sysmon_init);
module_exit(sysmon_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Subhakant");
MODULE_DESCRIPTION("Linux System Monitor Character Device");
