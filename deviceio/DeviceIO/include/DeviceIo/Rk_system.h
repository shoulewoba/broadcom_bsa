#ifndef __RK_SYSTEM_H__
#define __RK_SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 *Version 1.1.0 Release 2019/03/27
  1.rewrite the bt api include ble/a2dp/spp
 *Version 1.0.4 Release 2019/01/29
  1.ble_wifi
    1. add config timeout to discon ble
    2. fix gatt_open flag
    3. add reconfigure when last config fail
 *Version 1.0.3 Release 2019/01/24
  1.ble_wifi
    1. fix netstatus print error when NETLINK_NETWORK_CONFIG_WRONG_KEY_FAILED
    2. notify NETLINK_NETWORK_SUCCEEDED when net recovery from config failed
 *Version 1.0.2 Release 2019/01/24
  1.ble_wifi
    1. support wep key
    2. optimize wpa_supplicant.conf
    3. fix many times reports sink connected
    4. add ble_config wifi timeout value
    5. fix chinese code for hisense
  2.Rk_key: add fix register long press callback with different keyevent

 *Version 1.0.1 Release 2019/01/07
  1.ble_wifi:	fix Chinese coding problem
		fix ble report event and add wifi priority
		add wrong key event callback
		add initBTForHis interface
  2.volume:	setVolume support zero
  3.propery:	implement RK_property
  4.player:	separate mediaplayer and deviceio
		add playlist function
 *Version 1.0.0 Release 2018/12/22
 */

#define DEVICEIO_VERSION "V1.1.0"

int RK_read_chip_id(char *buffer, const int size);
int RK_read_version(char *buffer, const int size);
int RK_system_factory_reset(const int reboot = 1);


#ifdef __cplusplus
}
#endif

#endif
