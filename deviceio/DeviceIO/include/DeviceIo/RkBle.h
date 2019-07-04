#ifndef __BLUETOOTH_BLE_H__
#define __BLUETOOTH_BLE_H__

#include <DeviceIo/RkBtBase.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	RK_BLE_STATE_IDLE = 0,
	RK_BLE_STATE_CONNECTTING,
	RK_BLE_STATE_SUCCESS,
	RK_BLE_STATE_FAIL,
	RK_BLE_STATE_DISCONNECT
} RK_BLE_STATE;

typedef struct {
	char uuid[38];
	char data[134];
	int len;
} RkBleConfig;

typedef void (*RK_BLE_STATE_CALLBACK)(RK_BLE_STATE state);
typedef void (*RK_BLE_RECV_CALLBACK)(const char *uuid, char *data, int len);

int rk_ble_register_status_callback(RK_BLE_STATE_CALLBACK cb);
int rk_ble_register_recv_callback(RK_BLE_RECV_CALLBACK cb);
int rk_ble_start(RkBleContent *ble_content);
int rk_ble_stop(void);
int rk_ble_get_state(RK_BLE_STATE *p_state);
int rk_ble_get_exdata(char *buffer, int *length);
int rk_ble_write(const char *uuid, char *data, int len);

#ifdef __cplusplus
}
#endif

#endif /* __BLUETOOTH_BLE_H__ */
