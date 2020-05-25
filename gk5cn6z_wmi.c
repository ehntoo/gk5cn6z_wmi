#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/acpi.h>

MODULE_AUTHOR("Mitchell Johnson <ehntoo@ehntoo.org>");
MODULE_DESCRIPTION("GK5CN6Z WMI Driver");
MODULE_LICENSE("Dual MIT/GPL");

#define GK5CN6Z_ECRAM_GUID      "ABBC0F6F-8EA1-11D1-00A0-C90629100000"
#define GK5CN6Z_ECRAM_METHOD_ID 4

#define GK5CN6Z_EVENT_GUID      "ABBC0F72-8EA1-11D1-00A0-C90629100000"

static int __init init_hello_4(void)
{
	printk(KERN_INFO "Hello, world 4\n");
  if (!wmi_has_guid(GK5CN6Z_EVENT_GUID)) {
	    pr_warn("No known WMI event GUID found\n");
	    return -ENODEV;
	}
	return 0;
}

static void __exit cleanup_hello_4(void)
{
	printk(KERN_INFO "Goodbye, world 4\n");
}

module_init(init_hello_4);
module_exit(cleanup_hello_4);
