#ifndef __MY_AUDIO_H__
#define __MY_AUDIO_H__

/*
#define	WF_STARTUP				"/system/sound/startup.wav"		//系统启动成功，欢迎使用。//Welcome to smarthome
#define	WF_CONFIG				"/system/sound/config.wav"		//系统已切换为网络配置模式，请在5分钟之内完成网络配置。
#define	WF_REBOOT				"/system/sound/reboot.wav"		//配置成功，系统重新启动中，请稍候。
#define	WF_CONFIGOK				"/system/sound/configok.wav"	//配置成功
#define	WF_OPENLOCK				"/system/sound/open.wav"		//门锁已开
#define	WF_WELCOME				"/system/sound/welcome.wav"		//欢迎光临。呼叫中，请稍候。
#define	WF_CALL_TIMEOUT			"/system/sound/call-timeout.wav"//超时无人接听，请稍候再拨。
#define	WF_CALLING				"/system/sound/calling.wav"		//爱的罗曼斯
#define	WF_TALK_TIMEOUT			"/system/sound/talk-timeout.wav"//通话超时
#define	WF_TALK_OVER			"/system/sound/talk-over.wav"	//对方已挂机，再次感谢您的光临。
#define	WF_CONFIG_WIFI_OK		"/system/sound/reboot.wav"
#define	WF_RESET_OK				"/system/sound/reset-ok.wav"	//复位操作成功，系统重新启动中，请稍候
#define	WF_ALARM_IN				"/system/sound/alarm.wav"		//Alarm Sound
#define	WF_SCARE				"/system/sound/alarm.wav"		//Alarm Sound
#define	WF_MUTE         		"/system/sound/mute.wav"
#define	WF_PIR_ALARM			"/system/sound/pir-alarm.wav"	//CTD, PIR Alarm Sound //欢迎光临，您已经入监控区域，请按门铃）
#define	WF_DHCP_SUCC    		"/system/sound/dhcp-success.wav"//DHCP SUCCESS
#define	WF_DHCP_FAIL    		"/system/sound/dhcp-fail.wav"	//DHCP FAIL
*/

#define	WF_STARTUP				0		//系统启动成功
#define	WF_CONFIGING			1		//配置模式
#define WF_SMARTCONFIGOK		2		//一键配置OK
#define WF_SMARTCONFIGNG		3		//一键配置失败
#define	WF_CONFIGOK				4		//配置成功
#define WF_CONFIGNG				5		//配置失败
#define	WF_REBOOT				6		//重启
#define WF_RESET				7		//复位
#define	WF_OPENLOCK				8		//开锁
#define WF_CALLING				9		//呼叫
#define WF_CALL_AGREE			10		//呼叫接通
#define WF_TALK_OVER			11		//通话结束---针对呼叫通话
#define WF_ALARM				12		//报警
#define WF_MUTE					13		//静音

#define WAVEALARM				"/system/sound/alarm.wav"
#define WAVEBOOT				"/system/sound/boot.wav"
#define WAVECALLING				"/system/sound/calling.wav"
#define WAVECONFIG				"/system/sound/config.wav"
#define WAVESMARTCONFIGOK		"/system/sound/smartConfigOK.wav"
#define WAVESMARTCONFIGNG		"/system/sound/smartConfigNG.wav"
#define WAVECONFIGOK			"/system/sound/config_ok.wav"
#define WAVECONFIGNG			"/system/sound/config_ng.wav"
#define WAVEDING				"/system/sound/ding.wav"
#define WAVEMUTE				"/system/sound/mute.wav"
#define WAVEOPENDOOR			"/system/sound/open_door.wav"
#define WAVETALKOVER			"/system/sound/talk_over.wav"


typedef struct __FACAUDIOCONFIG
{
	char codec[16];					//codec型号
}FACAUDIOCONFIG, *PFACAUDIOCONFIG;

typedef struct __CUSAUDIOCONFIG
{
	char enable;			//是否使能语言提示
	int volOut;				//输出音量百分比
}CUSAUDIOCONFIG, *PCUSAUDIOCONFIG;

typedef struct __SYSAUDIOCONFIG
{
	int sampleIn;
	int depthIn;
	int channelIn;
	int volIn;
	int sampleOut;
	int depthOut;
	int channelOut;
	int volOut;
	char encoder[16];
	char decoder[16];
	char duplex;		//是否使能双向语言
	char hard;			//硬，软回声抑制
}SYSAUDIOCONFIG, *PSYSAUDIOCONFIG;

typedef struct __MYAUDIOCONFIG {
	FACAUDIOCONFIG facAudioConfig;
	CUSAUDIOCONFIG cusAudioConfig;
	SYSAUDIOCONFIG sysAudioConfig;
}MYAUDIOCONFIG, *PMYAUDIOCONFIG;

#define MAXAUDIOFRAMESIZE	2 * 1024

int my_audio_init(void);
int StartPlayAlarmAudio(int type, void (*call_back)(void));
void get_audio_config(MYAUDIOCONFIG *imyAudioConfig);
void set_audio_config(MYAUDIOCONFIG imyAudioConfig);
void my_audio_notify_manage_refresh(void);
int my_audio_set_voice(int voice);

#endif
