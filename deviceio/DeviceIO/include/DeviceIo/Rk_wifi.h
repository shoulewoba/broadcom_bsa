#ifndef __RK_WIFI_H__
#define __RK_WIFI_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
	RK_WIFI_State_IDLE = 0,
	RK_WIFI_State_CONNECTING,
	RK_WIFI_State_CONNECTFAILED,
	RK_WIFI_State_CONNECTFAILED_WRONG_KEY,
	RK_WIFI_State_CONNECTED,
	RK_WIFI_State_DISCONNECTED
} RK_WIFI_RUNNING_State_e;

typedef enum {
	NONE = 0,
	WPA,
	WEP
} RK_WIFI_CONNECTION_Encryp_e;

typedef struct {
	int id;
	char bssid[20];
	char ssid[64];
	int freq;
	char mode[20];
	char wpa_state[20];
	char ip_address[20];
	char mac_address[20];
} RK_WIFI_INFO_Connection_s;

typedef int(*RK_wifi_state_callback)(RK_WIFI_RUNNING_State_e state);

int RK_wifi_register_callback(RK_wifi_state_callback cb);
int RK_wifi_ble_register_callback(RK_wifi_state_callback cb);
int RK_wifi_running_getState(RK_WIFI_RUNNING_State_e* pState);
int RK_wifi_running_getConnectionInfo(RK_WIFI_INFO_Connection_s* pInfo);
int RK_wifi_enable_ap(const char* ssid, const char* psk, const char* ip);
int RK_wifi_disable_ap();
int RK_wifi_enable(const int enable);
int RK_wifi_scan(void);
char* RK_wifi_scan_r(void);
char* RK_wifi_scan_r_sec(const unsigned int cols);
int RK_wifi_connect(const char* ssid, const char* psk);
int RK_wifi_connect1(const char* ssid, const char* psk, const RK_WIFI_CONNECTION_Encryp_e encryp, const int hide);
int RK_wifi_disconnect_network(void);
int RK_wifi_restart_network(void);
int RK_wifi_set_hostname(const char* name);
int RK_wifi_get_hostname(char* name, int len);
int RK_wifi_get_mac(char *wifi_mac);
int RK_wifi_has_config(void);
int RK_wifi_ping(void);

#ifdef __cplusplus
}
#endif

#endif
