#ifndef __RK_SOFTAP_H__
#define __RK_SOFTAP_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
	RK_softAP_State_IDLE=0,
	RK_softAP_State_CONNECTTING,
	RK_softAP_State_SUCCESS,
	RK_softAP_State_FAIL,
	RK_softAP_State_DISCONNECT,
} RK_softAP_State_e;

typedef int (*RK_softap_state_callback)(RK_softAP_State_e state);

int RK_softap_register_callback(RK_softap_state_callback cb);
int RK_softap_start(char* name);
int RK_softap_stop(void);
int RK_softap_getState(RK_softAP_State_e* pState);
int RK_softap_get_exdata(char* buffer, int* length);


#ifdef __cplusplus
}
#endif

#endif
