#ifndef __GST_PLAYER_H__
#define __GST_PLAYER_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	RK_MIDEA_MODE_SEQ, //Play in sequence, stop at the end of the playlist
	RK_MIDEA_MODE_RAND, //Shuffle Playback
	RK_MIDEA_MODE_SINGLE, //Single cycle
	RK_MIDEA_MODE_LOOP //List loop
} RK_MIDEA_MODE;

typedef enum {
	RK_MediaPlayer_State_IDLE = 0,
	RK_MediaPlayer_State_PLAY ,
	RK_MediaPlayer_State_PAUSE,
	RK_MediaPlayer_State_STOP ,
	RK_MediaPlayer_State_TRICKMODE,
	RK_MediaPlayer_State_End,
	RK_MediaPlayer_State_Error,
} RK_MediaPlayer_State_e;

typedef enum {
	RK_MediaEvent_URLInvalid,
	RK_MediaEvent_BufferStart,
	RK_MediaEvent_BufferEnd,
	RK_MediaEvent_Play,
	RK_MediaEvent_Pause,
	RK_MediaEvent_Stop,
	RK_MediaEvent_End,
	RK_MediaEvent_Ready,
	RK_MediaEvent_Duration,
	RK_MediaEvent_SeekEnable,
	RK_MediaEvent_Error
} RK_MediaEvent_e;

typedef void (*RK_media_event_callback)(void *userdata, RK_MediaEvent_e enEvent);

int RK_mediaplayer_create(int *pHandle);
int RK_mediaplayer_destroy(int iHandle);
int RK_mediaplayer_play(int iHandle, const char *url);
int RK_mediaplayer_pause(int iHandle);
int RK_mediaplayer_resume(int iHandle);
int RK_mediaplayer_get_position(int iHandle); // 获取播放进度
int RK_mediaplayer_get_duration(int iHandle); // 获取歌曲总时长
int RK_mediaplayer_seek(int iHandle, int iMs);
int RK_mediaplayer_stop(int iHandle);
int RK_mediaplayer_register_callback(int iHandle, void *userdata, RK_media_event_callback cb);
int RK_mediaplayer_set_mode(int iHandle, RK_MIDEA_MODE mode);
int RK_mediaplayer_prev(int iHandle);
int RK_mediaplayer_next(int iHandle);
int RK_mediaplayer_add_music(int iHandle, char *title, char *url);
int RK_mediaplayer_clear_playlist(int iHandle);
int RK_mediaplayer_show_list(int iHandle);
char* RK_mediaplayer_get_title(int iHandle);
int RK_mediaplayer_start_playlist(int iHandle);


#ifdef __cplusplus
}
#endif

#endif /* __GST_PLAYER_H__ */
