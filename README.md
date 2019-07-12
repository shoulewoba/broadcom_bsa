echo 0 > /sys/class/rfkill/rfkill0/state

sleep 2

echo 1 > /sys/class/rfkill/rfkill0/state

sleep 2

mkdir -p /data/bsa/config

cd /data/bsa/config

bsa_server -r 12 -p /system/etc/firmware/BCM4343B0.hcd -d /dev/ttyS4 -b /data/btsnoop.log > /data/bsa_log &

app_manager &

-------------------------------------------------attach

echo 1 > /sys/class/rfkill/rfkill0/state

brcm_patchram_plus1 -d --enable_hci --no2bytes --tosleep 200000 --baudrate 1500000 --patchram /system/etc/firmware/BCM4343B0.hcd 
/dev/ttyS4 & //初始化

/usr/libexec/bluetooth/bluetoothd --compat -n & 

hciconfig hci0 up

hciconfig hci0 piscan 



--------------------------ble_by_open_shell

# bsa_ble_wifi_introducer.sh start

hcd_file = /system/etc/firmware/BCM4345C0.hcd

[   17.211945] [BT_RFKILL]: bt shut off power

[   20.218469] [BT_RFKILL]: rfkill_rk_set_power: set bt wake_host pin output high!

[   20.281006] [BT_RFKILL]: rfkill_rk_set_power: set bt wake_host pin input!

[   20.281072] [BT_RFKILL]: ENABLE UART_RTS

[   20.382050] [BT_RFKILL]: DISABLE UART_RTS

[   20.382149] [BT_RFKILL]: bt turn on power

start broadcom bluetooth server bsa_sever

killall: bsa_server: no process killed

BSA_trace 1@ 12/31 19h:00m:23s:565ms: bsa_sv_dm_set_3d_type type=chip_id (not safe)

BSA_trace 2@ 12/31 19h:00m:23s:566ms: Command [all] with arg [0]

BSA_trace 3@ 12/31 19h:00m:23s:567ms: [ all ] trace level = 0

BSA_trace 4@ 12/31 19h:00m:23s:567ms: BTM_SetTraceLevel

start broadcom bluetooth app_manager

start broadcom bluetooth wifi introducer

Created thread with thread_id=  b6dab460

Created thread with thread_id=  b6cd9460

|----- bluetooth ble wifi config ------|




---------------------------------------------------ble by no open deviceio_test simple app

# deviceio_test blewifi

version:V1.2.0

===== rk_ble_wifi_init =====

hcd_file = /system/etc/firmware/BCM4345C0.hcd

killall: bsa_server: no process killed

bsa_server died.

[ 17.895028] [BT_RFKILL]: rfkill_rk_set_power: set bt wake_host pin output high!

[ 17.957863] [BT_RFKILL]: rfkill_rk_set_power: set bt wake_host pin input!

[ 17.957924] [BT_RFKILL]: ENABLE UART_RTS

[ 18.058869] [BT_RFKILL]: DISABLE UART_RTS

[ 18.058967] [BT_RFKILL]: bt turn on power

start broadcom bluetooth server bsa_sever

|----- bluetooth bsa server is open ------|

BSA_trace 1@ 12/31 19h:00m:18s:235ms: bsa_sv_dm_set_3d_type type=chip_id (not safe)

BSA_trace 2@ 12/31 19h:00m:18s:236ms: Command [all] with arg [0]

BSA_trace 3@ 12/31 19h:00m:18s:236ms: [ all ] trace level = 0

BSA_trace 4@ 12/31 19h:00m:18s:237ms: BTM_SetTraceLevel

DEBUG: check_bsa_server: wait bsa_server open.

