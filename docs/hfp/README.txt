// This document provides a brief introduction for the BSA HFP.
1. v11：default 0,1channel map loopback，should set the adc-channel map in dts; as follows:
   &acodec {
	status = "okay";
	rockchip,adc-grps-route = <1 2 3 0>;
   };

2. start hfp: bsa_bt_hfp.sh start

3. stop hfp: bsa_bt_hfp.sh stop
