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




