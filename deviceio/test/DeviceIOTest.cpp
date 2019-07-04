/*
 * Copyright (c) 2017 Rockchip, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <string.h>
#include <pthread.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/rtc.h>
#include <math.h>
#include<iostream>

#include <DeviceIo/DeviceIo.h>
#include <DeviceIo/Properties.h>
#include <DeviceIo/ScanResult.h>
#include <DeviceIo/WifiInfo.h>
#include <DeviceIo/WifiManager.h>
#include <DeviceIo/RK_mediaplayer.h>
#include <DeviceIo/BtsrcParameter.h>
#include <DeviceIo/bt_hal.h>

using DeviceIOFramework::DeviceIo;
using DeviceIOFramework::DeviceInput;
using DeviceIOFramework::LedState;
using DeviceIOFramework::BtControl;
using DeviceIOFramework::WifiControl;
using DeviceIOFramework::DeviceRTC;
using DeviceIOFramework::DevicePowerSupply;
using DeviceIOFramework::NetLinkNetworkStatus;
using DeviceIOFramework::wifi_config;

//ble_content_t ble_content;

static int g_player0_flag_eos = 0;
static int g_player0_flag_seekable = 0;
static int g_player0_flag_playing = 0;
static int g_player1_flag_eos = 0;
static int g_player1_flag_seekable = 0;
static int g_player1_flag_playing = 0;

int gst_multi_player_test(char *uri0, char *uri1);
int gst_single_player_with_multi_urls(char *uri0, char *uri1);
int gst_single_player_test(char *uri0);

class DeviceInputWrapper: public DeviceIOFramework::DeviceInNotify{
public:
	void networkReady() {
		printf("net ready\n");
	}
	void netlinkNetworkOnlineStatus(bool status) {
		if (status)
			printf("net changed to online\n");
		else
			printf("net changed to offline\n");
	}
	void netlinkNetworkStatusChanged(NetLinkNetworkStatus networkStatus) {
		printf("%s : %d\n", __func__, networkStatus);
		switch (DeviceIo::getInstance()->getNetworkStatus()) {
			case DeviceIOFramework::NETLINK_NETWORK_CONFIG_STARTED:
				DeviceIo::getInstance()->controlLed(LedState::LED_NET_WAIT_CONNECT);
				break;
			case DeviceIOFramework::NETLINK_NETWORK_CONFIGING:
				DeviceIo::getInstance()->controlLed(LedState::LED_NET_DO_CONNECT);
				break;
			case DeviceIOFramework::NETLINK_NETWORK_CONFIG_SUCCEEDED:
				DeviceIo::getInstance()->controlLed(LedState::LED_NET_CONNECT_SUCCESS);
				break;
			case DeviceIOFramework::NETLINK_NETWORK_CONFIG_FAILED:
				DeviceIo::getInstance()->controlLed(LedState::LED_NET_CONNECT_FAILED);
				DeviceIo::getInstance()->controlWifi(WifiControl::WIFI_RECOVERY);
				break;
			case DeviceIOFramework::NETLINK_NETWORK_SUCCEEDED:
				DeviceIo::getInstance()->controlLed(LedState::LED_NET_CONNECT_SUCCESS);
				break;
			case DeviceIOFramework::NETLINK_NETWORK_FAILED:
				DeviceIo::getInstance()->controlLed(LedState::LED_NET_CONNECT_FAILED);
				break;
			case DeviceIOFramework::NETLINK_NETWORK_RECOVERY_START:
				DeviceIo::getInstance()->controlLed(LedState::LED_NET_RECOVERY);
				break;
			case DeviceIOFramework::NETLINK_NETWORK_RECOVERY_SUCCEEDED:
				DeviceIo::getInstance()->controlLed(LedState::LED_NET_CONNECT_SUCCESS);
				break;
			case DeviceIOFramework::NETLINK_NETWORK_RECOVERY_FAILED:
				DeviceIo::getInstance()->controlLed(LedState::LED_NET_CONNECT_FAILED);
				break;
			default:
				break;
		}
	}
	void callback(DeviceInput event, void *data, int len) {
		//printf("hardware  event:%d \n", static_cast<int>(event));

		switch (event) {
		case DeviceInput::KEY_RAW_INPUT_EVENT: {
			struct input_event *ev = (struct input_event *)data;
			int key_code = ev->code;
			bool key_pressed = ev->value;

			if (key_pressed) {
				printf("KEY_RAW_INPUT_EVENT: key %d pressed\n", key_code);
				switch (key_code) {
					case KEY_MICMUTE:
						break;
					case KEY_MODE:
						break;
					case KEY_PLAY:
						break;
					case KEY_VOLUMEDOWN:
						break;
					case KEY_VOLUMEUP:
						break;
					default:
						break;
				}
			} else {
				if (key_code != 0)
					printf("KEY_RAW_INPUT_EVENT: key %d released\n", key_code);
			}
			break;
		}
		case DeviceInput::KEY_VOLUME_DOWN: {
			int vol = *((int *)data);
			int vol_cur = DeviceIo::getInstance()->getVolume();
			DeviceIo::getInstance()->setVolume(vol_cur - vol);
			break;
		}
		case DeviceInput::KEY_VOLUME_UP: {
			int vol = *((int *)data);
			int vol_cur = DeviceIo::getInstance()->getVolume();
			DeviceIo::getInstance()->setVolume(vol_cur + vol);
			break;
		}
		case DeviceInput::KEY_MIC_MUTE: {
			printf("key mic mute\n");
			static bool micmute = false;
			if (!micmute) {
				DeviceIo::getInstance()->controlLed(LedState::LED_MICMUTE);
			} else {
				LedState layer = LedState::LED_MICMUTE;
				DeviceIo::getInstance()->controlLed(LedState::LED_CLOSE_A_LAYER, &layer, sizeof(int));
			}

			micmute = !micmute;
			break;
		}
		case DeviceInput::KEY_ENTER_AP: {
			printf("key enter ap to config network\n");
			switch (DeviceIo::getInstance()->getNetworkStatus()) {
				case DeviceIOFramework::NETLINK_NETWORK_CONFIG_STARTED:
				case DeviceIOFramework::NETLINK_NETWORK_CONFIGING:
						DeviceIo::getInstance()->stopNetworkConfig();
						break;
				case DeviceIOFramework::NETLINK_NETWORK_RECOVERY_START:
						DeviceIo::getInstance()->stopNetworkRecovery();
						break;
				default:
						DeviceIo::getInstance()->startNetworkConfig(90);
						break;
			}
			break;
		}
		case DeviceInput::KEY_HEADPHONE_INSERT: {
			if ((data != NULL) && *((int *)data) == 1)
				printf("headphone inserted\n");
			else
				printf("headphone plug out\n");
			break;
		}
		case DeviceInput::KEY_RK816_POWER: { //xiaojv
			printf("xiaojv key pressed\n");
			break;
		}
		case DeviceInput::KEY_SHUT_DOWN: { //shutdown
			printf("key shut down key pressed\n");
			break;
		}
		case DeviceInput::KEY_FACTORY_RESET: {
			printf("key factory reset\n");
			DeviceIo::getInstance()->factoryReset();
			break;
		}
		case DeviceInput::BT_SRC_ENV_CONNECT: {
			printf("=== BT_SRC_ENV_CONNECT ===\n");
			break;
		}
		case DeviceInput::BT_SRC_ENV_DISCONNECT: {
			printf("=== BT_SRC_ENV_DISCONNECT ===\n");
			break;
		}
		case DeviceInput::BT_SINK_ENV_CONNECT: {
			printf("=== BT_SINK_ENV_CONNECT ===\n");
			break;
		}
		case DeviceInput::BT_SINK_ENV_DISCONNECT: {
			printf("=== BT_SINK_ENV_DISCONNECT ===\n");
			break;
		}
		case DeviceInput::BT_BLE_ENV_CONNECT: {
			printf("=== BT_BLE_ENV_CONNECT ===\n");
			break;
		}
		case DeviceInput::BT_BLE_ENV_DISCONNECT: {
			printf("=== BT_BLE_ENV_DISCONNECT ===\n");
			break;
		}
		case DeviceInput::BT_START_PLAY: {
			printf("=== BT_START_PLAY ===\n");
			break;
		}
		case DeviceInput::BT_PAUSE_PLAY: {
			printf("=== BT_PAUSE_PLAY ===\n");
			break;
		}
		case DeviceInput::BT_STOP_PLAY: {
			printf("=== BT_STOP_PLAY ===\n");
			break;
		}
		default:
			break;
		}
	}
};

static void wifi_status_callback(int status)
{
	printf("%s: status: %d.\n", __func__, status);
}

typedef struct {
	char *cmd;
	void (*action)(void *userdata);
} test_command_t;

static void suspend_test(void *data) {
	DeviceIo::getInstance()->suspend();
}

static void factoryReset_test(void *data) {
	DeviceIo::getInstance()->factoryReset();
}

static void ota_test(void *data) {
	DeviceIo::getInstance()->OTAUpdate("");
}

static void leds_test(void *data) {
	extern int led_test(int argc, char* argv[]);
	led_test(0, NULL);
}

static void bt_sink_test(void *data) {
	printf("---------------bt sink ----------------\n");
	DeviceIo::getInstance()->controlBt(BtControl::BT_SOURCE_CLOSE);
	DeviceIo::getInstance()->controlBt(BtControl::BT_SINK_OPEN);
}

static void bt_auto_source_test(void *data) {
	char address[17] = {0};
	printf("--------------- ble wifi ----------------\n");
	DeviceIo::getInstance()->a2dpSourceAutoConnect(address, 10000);
}

static void ble_wifi_manual_test(void *data) {
	printf("--------------- ble wifi ----------------\n");
	DeviceIo::getInstance()->startNetworkConfig(600);
}

static void ble_wifi_test(void *data) {
	printf("--------------- ble wifi ----------------\n");
	DeviceIo::getInstance()->startNetworkRecovery();
}
static void ble_wifi_close(void *data) {
	printf("---------------ble_wifi_close----------------\n");
	DeviceIo::getInstance()->controlBt(BtControl::BT_BLE_COLSE);
}

static void bt_sink_close(void *data) {
	printf("---------------BT_SINK_CLOSE----------------\n");
	DeviceIo::getInstance()->controlBt(BtControl::BT_SINK_CLOSE);
}
static void bt_source_close(void *data) {
	printf("---------------BT_SINK_CLOSE----------------\n");
	DeviceIo::getInstance()->controlBt(BtControl::BT_SOURCE_CLOSE);
}

static test_command_t process_command_table[] = {
	{"suspend", suspend_test},
	{"factoryReset", factoryReset_test},
	{"ota", ota_test},
	{"leds", leds_test},
	{"bt_sink", bt_sink_test},
	{"bt_auto_source", bt_auto_source_test},
	{"ble_wifi", ble_wifi_test},
	{"ble_manual_wifi", ble_wifi_manual_test},
	{"bt_sink_close", bt_sink_close},
	{"bt_source_close", bt_source_close},
	{"bt_open", bt_init_open},
	{"bt_api2_master_start", bt_api2_master_start},
	{"bt_api2_master_status", bt_api2_master_status},
	{"bt_api2_master_stop", bt_api2_master_stop},
	{"bt_api2_sink_open", bt_api2_sink_open},
	{"bt_api2_sink_visibility00", bt_api2_sink_visibility00},
	{"bt_api2_sink_visibility01", bt_api2_sink_visibility01},
	{"bt_api2_sink_visibility10", bt_api2_sink_visibility10},
	{"bt_api2_sink_visibility11", bt_api2_sink_visibility11},
	{"bt_api2_sink_status", bt_api2_sink_status},
	{"bt_api2_sink_play", bt_api2_sink_play},
	{"bt_api2_sink_pause", bt_api2_sink_pause},
	{"bt_api2_sink_next", bt_api2_sink_next},
	{"bt_api2_sink_previous", bt_api2_sink_previous},
	{"bt_api2_sink_stop", bt_api2_sink_stop},
	{"bt_api2_sink_reconnect_en0", bt_api2_sink_reconnect_en0},
	{"bt_api2_sink_reconnect_en1", bt_api2_sink_reconnect_en1},
	{"bt_api2_sink_disconnect", bt_api2_sink_disconnect},
	{"bt_api2_sink_close", bt_api2_sink_close},
	{"RK_blewifi_start", RK_ble_test},
	{"bt_api2_spp_open", bt_api2_spp_open},
	{"bt_api2_spp_write", bt_api2_spp_write},
	{"bt_api2_spp_close", bt_api2_spp_close},
	{"bt_api2_spp_status", bt_api2_spp_status},
	{"RK_ble_audio_start", RK_ble_audio_test},
};

static void show_help() {
	int i;
	printf("#### Please Input Your Test Command Index ####\n");
	for (i = 0; i < sizeof(process_command_table) / sizeof(process_command_table[0]); i++) {
		printf("%d.  %s \n", i, process_command_table[i].cmd);
	}
	printf("Which would you like: ");
}

int main(int argc, char *argv[])
{
	char value[1024];
	bool ret;
	char sn[128] = {0};
	char hostname[64] = {0};
	#define HOST_NAME_PREFIX "小聚音箱mini-"

	DeviceIOFramework::Properties* properties;
	properties = DeviceIOFramework::Properties::getInstance();
	properties->init();

	DeviceIOFramework::WifiManager* wifiManager;
	wifiManager = DeviceIOFramework::WifiManager::getInstance();
	wifiManager->init(properties);

	class DeviceInputWrapper *input = new DeviceInputWrapper();
	DeviceIo::getInstance()->setNotify(input);

	DeviceIo::getInstance()->setVolume(30,0);

	std::cout << "version:" << DeviceIo::getInstance()->getVersion() << std::endl;
	/*
	DeviceIo::getInstance()->getSn(sn);
	std::cout << "serial number:" << sn << std::endl;

	//set hostname of speaker before wifi/bt start
	if (strlen(sn) < 4)
		sprintf(hostname, "%s%s", HOST_NAME_PREFIX, "1234");
	else
		sprintf(hostname, "%s%s", HOST_NAME_PREFIX, sn + strlen(sn) - 4);
	DeviceIo::getInstance()->setHostName(hostname, strlen(hostname));

	std::string chipid = DeviceIo::getInstance()->getChipID();
	std::cout << "Chip ID : " << chipid.c_str() << std::endl;

	unsigned int bat_temp_period = 3;
	DeviceIo::getInstance()->controlPower(DevicePowerSupply::POWER_CFG_BAT_TEMP_PERIOD_DETECT, &bat_temp_period, 0);
	std::cout << "Set Battery Temperture Period Detect: " << bat_temp_period << std::endl;

	int bat_temp_threshold_min = 0;
	int bat_temp_threshold_max = 55;
	DeviceIo::getInstance()->controlPower(DevicePowerSupply::POWER_CFG_BAT_TEMP_THRESHOLD_MIN,
			&bat_temp_threshold_min, 0);
	std::cout << "Set Battery Temperture Threshold Min: " << bat_temp_threshold_min << std::endl;

	DeviceIo::getInstance()->controlPower(DevicePowerSupply::POWER_CFG_BAT_TEMP_THRESHOLD_MAX,
			&bat_temp_threshold_max, 0);
	std::cout << "Set Battery Temperture Threshold Max: " << bat_temp_threshold_max << std::endl;

	DeviceIo::getInstance()->controlPower(DevicePowerSupply::USB_ONLINE, value, 1024);
	std::cout << "Get USB Charge Status: " << value << std::endl;

	DeviceIo::getInstance()->controlPower(DevicePowerSupply::AC_ONLINE, value, 1024);
	std::cout << "Get AC Charge Status: " << value << std::endl;

	int charge_enable = 0;
	DeviceIo::getInstance()->controlPower(DevicePowerSupply::POWER_CFG_BAT_CHARGE_DISABLE,
			&charge_enable, 0);
	std::cout << "Disable Charge" << std::endl;

	DeviceIo::getInstance()->controlPower(DevicePowerSupply::POWER_CFG_GET_CHARGE_ENABLE_STATUS, value, 1);
	std::cout << "Charging Enable Status: " << ((value[0] == '1')? "YES":"NO") <<  std::endl;

	charge_enable = 1;
	DeviceIo::getInstance()->controlPower(DevicePowerSupply::POWER_CFG_BAT_CHARGE_ENABLE,
			&charge_enable, 0);
	std::cout << "Enable Charge" << std::endl;

	DeviceIo::getInstance()->controlPower(DevicePowerSupply::POWER_CFG_GET_CHARGE_ENABLE_STATUS, value, 1);
	std::cout << "Charging Enable Status: " << ((value[0] == '1')? "YES":"NO") <<  std::endl;

	struct rtc_time tmp_rtc;
	std::cout << "Rtc Read time:" << std::endl;
	DeviceIo::getInstance()->controlRtc(DeviceRTC::DEVICE_RTC_READ_TIME, &tmp_rtc, sizeof(tmp_rtc));

	unsigned int settime = 3;
	std::cout << "Rtc Set alarm wakeup after " << settime << " seconds" << std::endl;
	DeviceIo::getInstance()->controlRtc(DeviceRTC::DEVICE_RTC_SET_TIME, &settime, 0);

	std::cout << "Rtc Read alarm setting" << std::endl;
	DeviceIo::getInstance()->controlRtc(DeviceRTC::DEVICE_RTC_READ_ALARM, &tmp_rtc, 0);

	std::cout << "Rtc Enable alarm" << std::endl;
	DeviceIo::getInstance()->controlRtc(DeviceRTC::DEVICE_RTC_ENABLE_ALARM_INTERRUPT, NULL, 0);

	std::cout << "Rtc Wait alarm wakeup" << std::endl;
	DeviceIo::getInstance()->controlRtc(DeviceRTC::DEVICE_RTC_WAIT_ALARM_RING, NULL, 0);

	DeviceIo::getInstance()->controlPower(DevicePowerSupply::BATTERY_CAPACITY, value, 18);
	std::cout << "Get Battery Capacity: " << value << std::endl;

	unsigned int det_period = 30;
	DeviceIo::getInstance()->controlPower(DevicePowerSupply::POWER_CFG_CAPACITY_DETECT_PERIOD, &det_period, 0);
	std::cout << "Set Battery Capacity Detect Period: " << det_period << " seconds." << std::endl;

	DeviceIo::getInstance()->controlWifi(WifiControl::GET_WIFI_MAC, value, 18);
	std::cout << "Wifi Mac: " << value << std::endl;

	DeviceIo::getInstance()->controlBt(BtControl::GET_BT_MAC, value, 18);
	std::cout << "Bt Mac: " << value << std::endl;

	ret = DeviceIo::getInstance()->controlWifi(WifiControl::WIFI_IS_CONNECTED);
	std::cout << "is wifi connected: " << ret << std::endl;

	int max_brightness = 255;
	int min_brightness = 0;
	DeviceIo::getInstance()->controlLed(LedState::LED_ALL_OFF);
	sleep(1);
	DeviceIo::getInstance()->controlLed(LedState::LED_PWMR_SET, &max_brightness, sizeof(int));
	sleep(1);
	DeviceIo::getInstance()->controlLed(LedState::LED_PWMR_SET, &min_brightness, sizeof(int));
	DeviceIo::getInstance()->controlLed(LedState::LED_PWMG_SET, &max_brightness, sizeof(int));
	sleep(1);
	DeviceIo::getInstance()->controlLed(LedState::LED_PWMG_SET, &min_brightness, sizeof(int));
	DeviceIo::getInstance()->controlLed(LedState::LED_PWMB_SET, &max_brightness, sizeof(int));
	sleep(1);
	DeviceIo::getInstance()->controlLed(LedState::LED_ALL_OFF);

	DeviceIo::getInstance()->setEQParameter("/data/eq_bin_new");

	DeviceIo::getInstance()->startNetworkRecovery();
	*/
	while(true) {
		if (argc > 1 && !strncmp(argv[1], "debug", 5)) {
			char szBuf[64] = {0};
			show_help();
			if(!std::cin.getline(szBuf,64)) {
				std::cout << "error" << std::endl;
				continue;
			}
			int i;
			//匹配数字
			if (szBuf[0] >= '0' && szBuf[0] <= '9') {
				i = atoi(szBuf);
				if (i >=0 && i < sizeof(process_command_table) / sizeof(process_command_table[0]))
					process_command_table[i].action(NULL);
			}
			//匹配字符串
			for (i = 0; i < sizeof(process_command_table) / sizeof(process_command_table[0]); i++) {
				if (!strcmp(szBuf, process_command_table[i].cmd)) {
					process_command_table[i].action(NULL);
					break;
				}
			}
		} else {
			sleep(10);
		}
	}

	if (NULL != wifiManager)
		delete wifiManager;

	if (NULL != properties)
		delete properties;

	return 0;
}

void gst_callback(void *userdata, RK_MediaEvent_e enEvent)
{
	char *data = (char *)userdata;
	char buff[100] = {0};

	if (data)
		memcpy(buff, data, strlen(data));

	switch (enEvent) {
		case RK_MediaEvent_URLInvalid:
			printf("+++++ <%s>: URL Invalid event +++++\n", buff);
			break;
		case RK_MediaEvent_BufferStart:
			printf("+++++ <%s>: Buffer Start event +++++\n", buff);
			break;
		case RK_MediaEvent_BufferEnd:
			printf("+++++ <%s>: Buffer End event +++++\n", buff);
			break;
		case RK_MediaEvent_Play:
			printf("+++++ <%s>: Stream Play event +++++\n", buff);
			if (strstr(buff, "Player-0"))
				g_player0_flag_playing = 1;
			else if (strstr(buff, "Player-1"))
				g_player1_flag_playing = 1;
			break;
		case RK_MediaEvent_Pause:
			printf("+++++ <%s>: Stream Pause event +++++\n", buff);
			if (strstr(buff, "Player-0"))
				g_player0_flag_playing = 0;
			else if (strstr(buff, "Player-1"))
				g_player1_flag_playing = 0;
			break;
		case RK_MediaEvent_Stop:
			printf("+++++ <%s>: Stream Stop event +++++\n", buff);
			if (strstr(buff, "Player-0"))
				g_player0_flag_playing = 0;
			else if (strstr(buff, "Player-1"))
				g_player1_flag_playing = 0;
			break;
		case RK_MediaEvent_End:
			printf("+++++ <%s>: Stream End event +++++\n", buff);
			if (strstr(buff, "Player-0"))
				g_player0_flag_eos = 1;
			else if (strstr(buff, "Player-1"))
				g_player1_flag_eos = 1;

			break;
		case RK_MediaEvent_Ready:
			printf("+++++ <%s>: Stream Ready event +++++\n", buff);
			break;
		case RK_MediaEvent_Error:
			printf("+++++ <%s>: Stream Error event +++++\n", buff);
			break;
		case RK_MediaEvent_Duration:
			printf("+++++ <%s>: Stream Duration event +++++\n", buff);
			break;
		case RK_MediaEvent_SeekEnable:
			printf("+++++ <%s>: Stream SeekEnable event +++++\n", buff);
			if (strstr(buff, "Player-0"))
				g_player0_flag_seekable = 1;
			else if (strstr(buff, "Player-0"))
				g_player1_flag_seekable = 1;

			break;
		default:
			break;
	}
}

int gst_multi_player_test(char *uri0, char *uri1)
{
	int ret = 0;
	int pHandle0;
	int pHandle1;
	char data0[100];
	char data1[100];
	int position0 = 0;
	int position1 = 0;
	int duration0 = 0;
	int duration1 = 0;

	/* Init global stream flag. */
	g_player0_flag_playing = 0;
	g_player0_flag_eos = 0;
	g_player0_flag_seekable = 0;
	g_player1_flag_playing = 0;
	g_player1_flag_eos = 0;
	g_player1_flag_seekable = 0;

	/* Set player private data */
	memset(data0, 0, sizeof(data0));
	memset(data1, 0, sizeof(data1));
	memcpy(data0, "Player-0", strlen("Player-0"));
	memcpy(data1, "Player-1", strlen("Player-1"));

	/* Create player */
	ret = RK_mediaplayer_create(&pHandle0);
	if (ret < 0) {
		printf("create mideaplayer-0 failed!\n");
		return -1;
	}
	ret = RK_mediaplayer_create(&pHandle1);
	if (ret < 0) {
		printf("create mideaplayer-1 failed!\n");
		RK_mediaplayer_destroy(pHandle0);
		return -1;
	}

	/* Set callback for player */
	ret = RK_mediaplayer_register_callback(pHandle0, (void *) data0, gst_callback);
	if (ret < 0) {
		printf("Registe mideaplayer-0 callback failed!\n");
		RK_mediaplayer_destroy(pHandle0);
		RK_mediaplayer_destroy(pHandle1);
	}

	ret = RK_mediaplayer_register_callback(pHandle1, (void *) data1, gst_callback);
	if (ret < 0) {
		printf("Registe mideaplayer-1 callback failed!\n");
		RK_mediaplayer_destroy(pHandle0);
		RK_mediaplayer_destroy(pHandle1);
	}

	ret = RK_mediaplayer_play(pHandle0, uri0);
	if (ret < 0) {
		printf("play %s failed!\n", uri0);
		RK_mediaplayer_destroy(pHandle0);
		RK_mediaplayer_destroy(pHandle1);
		return -1;
	}

	ret = RK_mediaplayer_play(pHandle1, uri1);
	if (ret < 0) {
		printf("play %s failed!\n", uri1);
		RK_mediaplayer_destroy(pHandle0);
		RK_mediaplayer_destroy(pHandle1);
		return -1;
	}

	while (!g_player0_flag_eos || !g_player1_flag_eos) {
		if (!g_player0_flag_playing && !g_player1_flag_playing) {
			usleep(100000); // 100ms
			continue;
		}

		/* Get position operation test */
		if (!g_player0_flag_eos) {
			ret = RK_mediaplayer_get_position(pHandle0);
			if (ret > 0)
				position0 = ret;
			else
				printf("Get position from %s failed!\n", uri0);
		}
		if (!g_player1_flag_eos) {
			ret = RK_mediaplayer_get_position(pHandle1);
			if (ret > 0)
				position1 = ret;
			else
				printf("Get position from %s failed!\n", uri1);
		}

		/* Get position operation test */
		if (!g_player0_flag_eos && !duration0) {
			ret = RK_mediaplayer_get_duration(pHandle0);
			if (ret > 0)
				duration0 = ret;
			else
				printf("Get duration from %s failed! ret = %d\n", uri0, ret);
		}
		/* Get position operation test */
		if (!g_player1_flag_eos && !duration1) {
			ret = RK_mediaplayer_get_duration(pHandle1);
			if (ret > 0)
				duration1 = ret;
			else
				printf("Get duration from %s failed! ret = %d\n", uri1, ret);
		}

		if (!g_player0_flag_eos && position0 && duration0)
			printf("MediaPlayer-0:%dms/%dms", position0, duration0);
		if (!g_player1_flag_eos && position1 && duration1)
			printf("; MediaPlayer-1:%dms/%dms \r", position1, duration1);
		else
			printf("\r");

		fflush(stdout);
		usleep(100000);
	}

	printf(">>>>> All stream reached the end.\n");
	RK_mediaplayer_destroy(pHandle0);
	RK_mediaplayer_destroy(pHandle1);
	return 0;
}

int gst_single_player_test(char *uri0)
{
	int ret = 0;
	int pHandle0;
	char data0[100];
	int position0 = 0;
	int duration0 = 0;
	int player0_is_paused = 0;
	int op_seek = 0;
	int op_pause = 0;

	/* Init global stream flag. */
	g_player0_flag_playing = 0;
	g_player0_flag_eos = 0;
	g_player0_flag_seekable = 0;

	/* Set player private data */
	memset(data0, 0, sizeof(data0));
	memcpy(data0, "Player-0", strlen("Player-0"));

	/* Create player */
	ret = RK_mediaplayer_create(&pHandle0);
	if (ret < 0) {
		printf("create mideaplayer-0 failed!\n");
		return -1;
	}

	/* Set callback for player */
	ret = RK_mediaplayer_register_callback(pHandle0, (void *) data0, gst_callback);
	if (ret < 0) {
		printf("Registe mideaplayer-0 callback failed!\n");
		RK_mediaplayer_destroy(pHandle0);
	}

	ret = RK_mediaplayer_play(pHandle0, uri0);
	if (ret < 0) {
		printf("play %s failed!\n", uri0);
		RK_mediaplayer_destroy(pHandle0);
		return -1;
	}

	while (!g_player0_flag_eos) {
		if (!g_player0_flag_playing) {
			usleep(100000); // 100ms
			continue;
		}

		/* Get position operation test */
		ret = RK_mediaplayer_get_position(pHandle0);
		if (ret > 0)
			position0 = ret;
		else
			printf("Get position from %s failed!\n", uri0);

		/* Get position operation test */
		if (!duration0) {
			ret = RK_mediaplayer_get_duration(pHandle0);
			if (ret > 0)
				duration0 = ret;
			else
				printf("Get duration from %s failed! ret = %d\n", uri0, ret);
		}

		if (position0 && duration0) {
			printf("MediaPlayer-0:%dms/%dms\r", position0, duration0);
			fflush(stdout);
		}

		/* Seek operation test */
		if (!op_seek && position0 && duration0 && (position0 >= (duration0 / 3))) {
			printf(">>>>> Seek mideaplayer-0 from %dms to %dms.\n",
				   duration0 / 3, duration0 / 2);
			ret = RK_mediaplayer_seek(pHandle0, duration0 / 2);
			if (ret < 0)
				printf("Seek operation failed!\n");

			op_seek = 1;
		}

		/* Pause operation test */
		if (!op_pause && position0 && duration0 && (position0 >= (2 * duration0 / 3))) {
			ret = RK_mediaplayer_pause(pHandle0);
			if (ret)
				printf(">>>>> Pause mideaplayer-0 FAILED!\n");
			else {
				printf(">>>>> Pause mideaplayer-0 SUCESS!\n");
				player0_is_paused = 1;
			}

			printf("Paused for 3s ...\n");
			sleep(3);

			if (player0_is_paused) {
				ret = RK_mediaplayer_resume(pHandle0);
				if (ret)
					printf(">>>>> Resume mideaplayer-0 FAILED\n");
				else
					printf(">>>>> Resume mideaplayer-0 SUCESS!\n");
			}

			op_pause = 1;
		}

		usleep(100000);
	}

	printf(">>>>> All stream reached the end.\n");
	RK_mediaplayer_destroy(pHandle0);

	return 0;
}

int gst_single_player_with_multi_urls(char *uri0, char *uri1)
{
	int ret = 0;
	int pHandle0;
	char data0[100];
	int position0 = 0;
	int duration0 = 0;
	int player0_is_paused = 0;
	int op_reset = 0;

	/* Init global stream flag. */
	g_player0_flag_playing = 0;
	g_player0_flag_eos = 0;
	g_player0_flag_seekable = 0;

	/* Set player private data */
	memset(data0, 0, sizeof(data0));
	memcpy(data0, "Player-0", strlen("Player-0"));

	/* Create player */
	ret = RK_mediaplayer_create(&pHandle0);
	if (ret < 0) {
		printf("create mideaplayer-0 failed!\n");
		return -1;
	}

	/* Set callback for player */
	ret = RK_mediaplayer_register_callback(pHandle0, (void *) data0, gst_callback);
	if (ret < 0) {
		printf("Registe mideaplayer-0 callback failed!\n");
		RK_mediaplayer_destroy(pHandle0);
	}

	ret = RK_mediaplayer_play(pHandle0, uri0);
	if (ret < 0) {
		printf("play %s failed!\n", uri0);
		RK_mediaplayer_destroy(pHandle0);
		return -1;
	}

	while (!g_player0_flag_eos) {
		if (!g_player0_flag_playing) {
			usleep(100000); // 100ms
			continue;
		}

		/* Get position operation test */
		ret = RK_mediaplayer_get_position(pHandle0);
		if (ret > 0)
			position0 = ret;
		else
			printf("Get position from %s failed!\n", uri0);

		/* Get position operation test */
		if (!duration0) {
			ret = RK_mediaplayer_get_duration(pHandle0);
			if (ret > 0)
				duration0 = ret;
			else
				printf("Get duration from %s failed! ret = %d\n", uri0, ret);
		}

		if (position0 && duration0) {
			printf("MediaPlayer-0:%dms/%dms\r", position0, duration0);
			fflush(stdout);
		}

		/* Stop operation test */
		if (!op_reset && position0 && duration0 && (position0 >= (duration0 / 3))) {
			ret = RK_mediaplayer_stop(pHandle0);
			if (ret)
				printf(">>>>> Stop mideaplayer-0 FAILED!\n");
			else
				printf(">>>>> Stop mideaplayer-0 SUCESS!\n");

			printf(">>>>> Reset mideaplayer-0 uri from %s to %s.\n", uri0, uri1);
			ret = RK_mediaplayer_play(pHandle0, uri1);
			if (ret < 0) {
				printf("play %s failed!\n", uri0);
				RK_mediaplayer_destroy(pHandle0);
				return -1;
			}
			duration0 = 0;
			position0 = 0;
			op_reset = 1;
		}

		usleep(100000);
	}

	printf(">>>>> All stream reached the end.\n");
	RK_mediaplayer_destroy(pHandle0);

	return 0;
}
