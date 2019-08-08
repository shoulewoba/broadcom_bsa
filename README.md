博通的bsa协议中其实很简单的，因为是个黑盒子，不提供代码，所以只能调用接口，另一方面如果自己需要一些特殊功能，也需要博通修改重新编译提供后台应用，博通不提供技术支持，只能让正基去改。

是什么后台应用？ 就是broadcom_bsa/server这个目录下的bsa_server

bsa_server这个大小大概是4M，这个后台是一定要跑的，不管前端写的是应用还是脚本还是自己敲得命令，这个应用必须跑起来

看一下手动的命令

-------------

echo 0 > /sys/class/rfkill/rfkill0/state

sleep 2

echo 1 > /sys/class/rfkill/rfkill0/state

sleep 2

mkdir -p /data/bsa/config

cd /data/bsa/config

bsa_server -r 12 -p /system/etc/firmware/BCM4343B0.hcd -d /dev/ttyS4 -b /data/btsnoop.log > /data/bsa_log &

app_manager &

-------------

有跑bsa_server的后台，sniff log也可以输出到data/btsnoop.log，前端只跑了app_manager

还有哪些前端呢，在broadcom_bsa/3rdparty里面，这个目录下就是编译测试程序，

库在
./bsa_examples/linux/libbsa/build/arm64/sharedlib/libbsa.so

./bsa_examples/linux/libbsa/build/arm/sharedlib/libbsa.so

./bsa_examples/linux/libbsa/build/x86_64/sharedlib/libbsa.so

一些测试程序



博通在linux上面，除了可以跑自己的协议栈，当然也可以跑开源的bluez，看一下手动的命令，主要就是有个hciattach加载hci0的


--------


echo 0 > /sys/class/rfkill/rfkill0/state

sleep 2

echo 1 > /sys/class/rfkill/rfkill0/state

sleep 2

brcm_patchram_plus1 -d --enable_hci --no2bytes --tosleep 200000 --baudrate 1500000 --patchram /system/etc/firmware/BCM4343B0.hcd 
/dev/ttyS4 & //初始化

/usr/libexec/bluetooth/bluetoothd --compat -n & 

hciconfig hci0 up

hciconfig hci0 piscan 


------------

这个就是跑BLUEZ的，IPC机制是D-BUS，一样是后台前端的模式，跑起bluetoothd可以运行一些工具或者bluez-alsa




然后再看一下，运行博通协议站的时候，一些配网的例子，这个脚本是我们sdk自带的，我们自己写的


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



---------
  325 root      3380 S    wpa_supplicant -B -i wlan0 -c /data/cfg/wpa_supplica
  
  355 root     20064 S    bsa_server -r 12 -p /system/etc/firmware/BCM4345C0.h
  
  366 root      3352 S    app_manager -s
  
  367 root      3932 S    app_ble_wifi_introducer




在这个脚本运行完后，我们看到了，主要跑了bsa_server后台和app_ble_wifi_introducer前端

这个前端在3rdparty/embedded/bsa_examples/linux/app_ble_wifi_introducer
这个就是博通协议中带的测试程序








还有一个东西叫做deviceio，这个是我们自己开发的上层，主要就是把那些测试程序和脚本的命令都再封装一次，留下一个更简单的接口。

这个再封装一次的上层可以支持正基博通，海华博通，还有bluez

external/deviceio/DeviceIO/lib32$ ls

libDeviceIo_bluez.so  libDeviceIo_broadcom.so  libDeviceIo_cypress.so

主要的库在这里，编译的话要在CMAKE上选好，CMakeLists.txt，可以选择哪个库。



再来看看我们封装一次的上层，配网的例子

直接运行就可以了

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


-------
  326 root      3380 S    wpa_supplicant -B -i wlan0 -c /data/cfg/wpa_supplica

  398 root      7012 S    deviceio_test blewifi
  
  408 root     20064 S    bsa_server -r 12 -p /system/etc/firmware/BCM4345C0.h


我们有看到，这个配网过程中有运行bsa_server -r 12 -p /system/etc/firmware/BCM4345C0.h后台程序，还有deviceio_test blewifi的应用程序
所以和手动运行差不多的，只是封装一下，让客户更好用


