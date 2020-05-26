#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/acpi.h>

/* #include <input/sparse-keymap.h> */

MODULE_AUTHOR("Mitchell Johnson <ehntoo@ehntoo.org>");
MODULE_DESCRIPTION("GK5CN6Z WMI Driver");
MODULE_LICENSE("Dual MIT/GPL");

#define GK5CN6Z_ECRAM_GUID      "ABBC0F6F-8EA1-11D1-00A0-C90629100000"
#define GK5CN6Z_ECRAM_METHOD_ID 4

#define GK5CN6Z_EVENT_GUID      "ABBC0F72-8EA1-11D1-00A0-C90629100000"
#define GK5CN6Z_EVENT_NUMBER    210

static void gk5cn6z_wmi_notify(u32 value, void *context)
{
  struct acpi_buffer response = { ACPI_ALLOCATE_BUFFER, NULL };
  union acpi_object *obj;
  acpi_status status;
  int code;

  status = wmi_get_event_data(value, &response);
  if (status != AE_OK) {
    pr_err("GK5CN6Z WMI: bad event status 0x%x\n", status);
    return;
  }

  obj = (union acpi_object *)response.pointer;

  if (obj && obj->type == ACPI_TYPE_INTEGER) {
    code = obj->integer.value;
    pr_info("Got event %d\n", code);

    /* if (code >= NOTIFY_BRNUP_MIN && code <= NOTIFY_BRNUP_MAX) */
    /*   code = NOTIFY_BRNUP_MIN; */
    /* else if (code >= NOTIFY_BRNDOWN_MIN && code <= NOTIFY_BRNDOWN_MAX) */
    /*   code = NOTIFY_BRNDOWN_MIN; */

    /* if (!sparse_keymap_report_event(eeepc_wmi_input_dev, */
    /*         code, 1, true)) */
    /*   pr_info("EEEPC WMI: Unknown key %x pressed\n", code); */
  }

  kfree(obj);
}

static int __init gk5cn6z_wmi_init(void)
{
  acpi_status status;

  pr_info("Loading GK5CN6Z wmi driver\n");
  if (!wmi_has_guid(GK5CN6Z_EVENT_GUID)) {
      pr_warn("No known WMI event GUID found\n");
      return -ENODEV;
  }

  if (!wmi_has_guid(GK5CN6Z_ECRAM_GUID)) {
      pr_warn("No known WMI ECRAM access GUID found\n");
      return -ENODEV;
  }

  status = wmi_install_notify_handler(GK5CN6Z_EVENT_GUID,
      gk5cn6z_wmi_notify, NULL);
  if (ACPI_FAILURE(status)) {
    /* sparse_keymap_free(gk5cn6z_wmi_input_dev); */
    /* input_unregister_device(eeepc_wmi_input_dev); */
    /* pr_err("EEEPC WMI: Unable to register notify handler - %d\n", */
    /*   status); */
    return -ENODEV;
  }
  return 0;
}

static void __exit gk5cn6z_wmi_cleanup(void)
{
  pr_info("Unloading GK5CN6Z wmi driver\n");
  wmi_remove_notify_handler(GK5CN6Z_EVENT_GUID);
}

module_init(gk5cn6z_wmi_init);
module_exit(gk5cn6z_wmi_cleanup);
