#ifndef __BLUETOOTH_SOURCE_H__
#define __BLUETOOTH_SOURCE_H__

#include <DeviceIo/RkBtBase.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BT_SOURCE_SCAN_DEVICES_CNT	30
typedef struct _bt_device_info {
    char name[128]; // bt name
    char address[17]; // bt address
    bool rssi_valid;
    int rssi;
    char playrole[12]; // Audio Sink? Audio Source? Unknown?
} BtDeviceInfo;

/*
 * Specify Bluetooth scan parameters.
 * mseconds: How long is the scan, in milliseconds.
 * item_cnt: Number of devices that have been scanned.
 * device_list: Save scan results.
 */
typedef struct _bt_scan_parameter {
    unsigned short mseconds;
    unsigned char item_cnt;
    BtDeviceInfo devices[BT_SOURCE_SCAN_DEVICES_CNT];
} BtScanParam;

typedef enum {
	BT_SOURCE_EVENT_CONNECT_FAILED,
	BT_SOURCE_EVENT_CONNECTED,
	BT_SOURCE_EVENT_DISCONNECTED,
} RK_BT_SOURCE_EVENT;

typedef enum {
	BT_SOURCE_STATUS_CONNECTED,
	BT_SOURCE_STATUS_DISCONNECTED,
} RK_BT_SOURCE_STATUS;

typedef void (*RK_BT_SOURCE_CALLBACK)(void *userdata, const RK_BT_SOURCE_EVENT event);

int rk_bt_source_auto_connect_start(void *userdata, RK_BT_SOURCE_CALLBACK cb);
int rk_bt_source_auto_connect_stop(void);
int rk_bt_source_open(void);
int rk_bt_source_close(void);
int rk_bt_source_get_device_name(char *name, int len);
int rk_bt_source_get_device_addr(char *addr, int len);
int rk_bt_source_get_status(RK_BT_SOURCE_STATUS *pstatus, char *name, char *addr);
int rk_bt_source_scan(BtScanParam *data);
int rk_bt_source_connect(char *address);
int rk_bt_source_disconnect(char *address);
int rk_bt_source_remove(char *address);
int rk_bt_source_register_status_cb(void *userdata, RK_BT_SOURCE_CALLBACK cb);

#ifdef __cplusplus
}
#endif

#endif /* __BLUETOOTH_SOURCE_H__ */