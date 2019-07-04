#ifndef __BT_BASE_H__
#define __BT_BASE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char	  uint8_t;
typedef unsigned short uint16_t;

typedef struct {
#define UUID_16     2
#define UUID_32     4
#define UUID_128    16

	uint16_t len; //byte
	const char *uuid;
} Ble_Uuid_Type_t;

enum {
	BLE_ADVDATA_TYPE_USER = 0,
	BLE_ADVDATA_TYPE_SYSTEM
};

typedef struct {
	Ble_Uuid_Type_t server_uuid;
	Ble_Uuid_Type_t chr_uuid[12];
	uint8_t chr_cnt;
	const char *ble_name;
	uint8_t advData[256];
	uint8_t advDataLen;
	uint8_t respData[256];
	uint8_t respDataLen;
	uint8_t advDataType;
	//AdvDataKgContent adv_kg;
	char le_random_addr[6];
	/* recevice data */
	void (*cb_ble_recv_fun)(const char *uuid, char *data, int len);
	/* full data */
	void (*cb_ble_request_data)(const char *uuid);
} RkBleContent;

typedef struct {
	RkBleContent ble_content;
	const char *bt_name;
} RkBtContent;

int rk_bt_init(RkBtContent *p_bt_content);
int rk_bt_deinit(void);
int rk_bt_is_connected(void);

#ifdef __cplusplus
}
#endif

#endif /* __BT_BASE_H__ */
