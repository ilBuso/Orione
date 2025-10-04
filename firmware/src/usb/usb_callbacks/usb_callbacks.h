#ifndef USB_CALLBACKS_H
#define USB_CALLBACKS_H

    #include "bsp/board_api.h"
    #include "tusb.h"

    #include <class/hid/hid.h>
    
    #include "../usb_descriptors/usb_descriptors.h"
    #include "src/global.h"
    #include "src/matrix/scan_rows/scan_rows.h"
    
    void tud_mount_cb(void);
    void tud_umount_cb(void);
    void tud_suspend_cb(bool remote_wakeup_en);
    void tud_resume_cb(void);

    void send_hid_report(uint8_t report_id, uint16_t consumer_code);
    void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len);
    uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen);
    void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize);

#endif /* USB_CALLBACKS_H */