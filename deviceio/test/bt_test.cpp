#include <string>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <csignal>
#include <errno.h>
#include <paths.h>
#include <sys/wait.h>

#include "DeviceIo/DeviceIo.h"
#include <DeviceIo/bt_hal.h>

/* Immediate wifi Service UUID */
#define BLE_UUID_SERVICE	"0000180A-0000-1000-8000-00805F9B34FB"
#define BLE_UUID_WIFI_CHAR	"00009999-0000-1000-8000-00805F9B34FB"
#define BLE_UUID_AUDIO_CHAR	"00006666-0000-1000-8000-00805F9B34FB"

/* have to initialize */
static Bt_Content_t bt_content;

void bt_init_open(void *data)
{
	printf("---------------BT_OPEN----------------\n");
	bt_content.bt_name = "KUGOU_AUDIO";
	bt_content.ble_content.ble_name = "KUGOU_BLE_123";
	bt_content.ble_content.server_uuid = BLE_UUID_SERVICE;
	bt_content.ble_content.chr_uuid[0] = BLE_UUID_WIFI_CHAR;
	bt_content.ble_content.chr_uuid[1] = BLE_UUID_AUDIO_CHAR;
	bt_content.ble_content.chr_cnt = 2;

	RK_bt_init(&bt_content);
}

/******************************************/
/*               A2DP SINK                */
/******************************************/
int bt_sink_callback(RK_BTA2DP_State_e state)
{
	switch(state) {
		case RK_BTA2DP_State_IDLE:
			printf("++++++++++++ BT SINK EVENT: idle ++++++++++\n");
			break;
		case RK_BTA2DP_State_CONNECT:
			printf("++++++++++++ BT SINK EVENT: connect sucess ++++++++++\n");
			break;
		case RK_BTA2DP_State_PLAY:
			printf("++++++++++++ BT SINK EVENT: playing ++++++++++\n");
			break;
		case RK_BTA2DP_State_PAUSE:
			printf("++++++++++++ BT SINK EVENT: paused ++++++++++\n");
			break;
		case RK_BTA2DP_State_STOP:
			printf("++++++++++++ BT SINK EVENT: stoped ++++++++++\n");
			break;
		case RK_BTA2DP_State_DISCONNECT:
			printf("++++++++++++ BT SINK EVENT: disconnected ++++++++++\n");
			break;
	}
}

void bt_api2_sink_open(void *data)
{
	RK_bta2dp_register_callback(bt_sink_callback);
	RK_bta2dp_open((char *)bt_content.bt_name);
}

void bt_api2_sink_visibility00(void *data)
{
	RK_bta2dp_setVisibility(0, 0);
}

void bt_api2_sink_visibility01(void *data)
{
	RK_bta2dp_setVisibility(0, 1);
}

void bt_api2_sink_visibility10(void *data)
{
	RK_bta2dp_setVisibility(1, 0);
}

void bt_api2_sink_visibility11(void *data)
{
	RK_bta2dp_setVisibility(1, 1);
}

void bt_api2_sink_status(void *data)
{
	RK_BTA2DP_State_e pState;

	RK_bta2dp_getState(&pState);
	switch(pState) {
		case RK_BTA2DP_State_IDLE:
			printf("++++++++++++ BT MASTER EVENT: idle ++++++++++\n");
			break;
		case RK_BTA2DP_State_CONNECT:
			printf("++++++++++++ BT MASTER EVENT: connect sucess ++++++++++\n");
			break;
		case RK_BTA2DP_State_PLAY:
			printf("++++++++++++ BT MASTER EVENT: playing ++++++++++\n");
			break;
		case RK_BTA2DP_State_PAUSE:
			printf("++++++++++++ BT MASTER EVENT: paused ++++++++++\n");
			break;
		case RK_BTA2DP_State_STOP:
			printf("++++++++++++ BT MASTER EVENT: stoped ++++++++++\n");
			break;
		case RK_BTA2DP_State_DISCONNECT:
			printf("++++++++++++ BT MASTER EVENT: disconnected ++++++++++\n");
			break;
	}
}

void bt_api2_sink_play(void *data)
{
	RK_bta2dp_play();
}

void bt_api2_sink_pause(void *data)
{
	RK_bta2dp_pause();
}

void bt_api2_sink_next(void *data)
{
	RK_bta2dp_next();
}

void bt_api2_sink_previous(void *data)
{
	RK_bta2dp_prev();
}

void bt_api2_sink_stop(void *data)
{
	RK_bta2dp_stop();
}

void bt_api2_sink_reconnect_en0(void *data)
{
	RK_bta2dp_set_auto_reconnect(0);
}

void bt_api2_sink_reconnect_en1(void *data)
{
	RK_bta2dp_set_auto_reconnect(1);
}

void bt_api2_sink_disconnect(void *data)
{
	RK_bta2dp_disconnect();
}

void bt_api2_sink_close(void *data)
{
	RK_bta2dp_close();
}

/******************************************/
/*              A2DP SOURCE               */
/******************************************/
void bt_master_callback(void *userdata, const RK_BtMasterEvent_e enEvent)
{
	char address[17], name[100];

	switch(enEvent)
	{
		case RK_BtMasterEvent_Connect_Failed:
			printf("++++++++++++ BT MASTER EVENT: connect failed ++++++++++\n");
			RK_btmaster_getDeviceName(name, 100);
			RK_btmaster_getDeviceAddr(address, 17);
			printf("DeviceName:%s. Address:%s\n", name, address);
			break;
		case RK_BtMasterEvent_Connected:
			printf("++++++++++++ BT MASTER EVENT: connect sucess ++++++++++\n");
			break;
		case RK_BtMasterEvent_Disconnected:
			printf("++++++++++++ BT MASTER EVENT: disconnect ++++++++++\n");
			break;
	}
}

void bt_api2_master_start(void *data)
{
	RK_btmaster_connect_start(NULL, bt_master_callback);
}

void bt_api2_master_stop(void *data)
{
	RK_btmaster_stop();
}

void bt_api2_master_status(void *data)
{
	RK_BtMasterStatus status;

	RK_btmaster_getStatus(&status);
	if (status == RK_BtMasterStatus_Connected)
		printf("++++++++++++ BT MASTER STATUS: connected ++++++++++++\n");
	else
		printf("++++++++++++ BT MASTER STATUS: disconnected ++++++++++++\n");
}

/******************************************/
/*                  BLE                   */
/******************************************/
static int Setup_wifi_status_callback(RK_BLEWIFI_State_e state)
{
	printf("%s: status: %d.\n", __func__, state);

	switch (state) {
	case RK_BLEWIFI_State_IDLE:
		printf("RK_BLEWIFI_State_IDLE.\n");
	break;
	case RK_BLEWIFI_State_CONNECTTING:
		printf("RK_BLEWIFI_State_CONNECTTING.\n");
	break;
	case RK_BLEWIFI_State_SUCCESS:
		printf("RK_BLEWIFI_State_SUCCESS.\n");
	break;
	case RK_BLEWIFI_State_FAIL:
		printf("RK_BLEWIFI_State_FAIL.\n");
	break;
	case RK_BLEWIFI_State_DISCONNECT:
		printf("RK_BLEWIFI_State_DISCONNECT.\n");
	break;
	}
}

static int RK_ble_recv_data_test(const char *uuid, unsigned char *data, int len)
{
	char data_t[512];

	printf("=== %s uuid: %s===\n", __func__, uuid);
	memcpy(data_t, data, len);

	return 1;
}

static int Setup_ble_audio_status_callback(RK_BLE_State_e state)
{
	printf("%s: status: %d.\n", __func__, state);

	switch (state) {
	case RK_BLE_State_IDLE:
		printf("RK_BLE_State_IDLE.\n");
	break;
	case RK_BLE_State_CONNECTTING:
		printf("RK_BLE_State_CONNECTTING.\n");
	break;
	case RK_BLE_State_SUCCESS:
		printf("RK_BLE_State_SUCCESS.\n");
	break;
	case RK_BLE_State_FAIL:
		printf("RK_BLE_State_FAIL.\n");
	break;
	case RK_BLE_State_DISCONNECT:
		printf("RK_BLE_State_DISCONNECT.\n");
	break;
	}
}

static int RK_ble_audio_recv_data_test(const char *uuid, unsigned char *data, int len)
{
	char data_t[512];

	printf("=== %s uuid: %s, data: %s ===\n", __func__, uuid, data);
	memcpy(data_t, data, len);

	return 1;
}

void RK_ble_audio_test(void *data)
{
	RK_ble_audio_register_callback(Setup_ble_audio_status_callback);
	RK_ble_audio_recv_data_callback(RK_ble_audio_recv_data_test);
	RK_bleaudio_start(NULL);
}

void RK_ble_test(void *data) {
	printf("---------------kg_ble_open----------------\n");
	RK_blewifi_register_callback(Setup_wifi_status_callback);
	RK_ble_recv_data_callback(RK_ble_recv_data_test);
	RK_blewifi_start((char *)bt_content.bt_name);
}

/* SPP */
void _btspp_callback(int type, char *data, int len)
{
	switch(type) {
		case RK_BTSPP_Event_DATA:
			printf("+++++++ RK_BTSPP_Event_DATA +++++\n");
			printf("\tRECVED(%d):%s\n", len, data);
			break;
		case RK_BTSPP_Event_CONNECT:
			printf("+++++++ RK_BTSPP_Event_CONNECT +++++\n");
			break;
		case RK_BTSPP_Event_DISCONNECT:
			printf("+++++++ RK_BTSPP_Event_DISCONNECT +++++\n");
			break;
		default:
			printf("+++++++ BTSPP NO TYPE SUPPORT! +++++\n");
			break;
	}
}

void bt_api2_spp_open(void *data)
{
	RK_btspp_open(_btspp_callback);
}

void bt_api2_spp_write(void *data)
{
	int ret = 0;
	char buff[100] = {"This is a message from rk3308 board!"};

	ret = RK_btspp_write(buff, strlen(buff));
	if (ret != strlen(buff)) {
		printf("%s failed, ret<%d> != strlen(buff)<%d>\n",
				__func__, ret, strlen(buff));
	}
}

void bt_api2_spp_close(void *data)
{
	RK_bta2dp_close();
}

void bt_api2_spp_status(void *data)
{
	RK_BTSPP_State status;

	RK_btspp_getState(&status);
	switch(status) {
		case RK_BTSPP_State_IDLE:
			printf("+++++++ RK_BTSPP_State_IDLE +++++\n");
			break;
		case RK_BTSPP_State_CONNECT:
			printf("+++++++ RK_BTSPP_State_CONNECT +++++\n");
			break;
		case RK_BTSPP_State_DISCONNECT:
			printf("+++++++ RK_BTSPP_State_DISCONNECT +++++\n");
			break;
		default:
			printf("+++++++ BTSPP NO STATUS SUPPORT! +++++\n");
			break;
	}
}

