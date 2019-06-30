#ifndef __MY_AUDIO_H__
#define __MY_AUDIO_H__

/*
#define	WF_STARTUP				"/system/sound/startup.wav"		//ϵͳ�����ɹ�����ӭʹ�á�//Welcome to smarthome
#define	WF_CONFIG				"/system/sound/config.wav"		//ϵͳ���л�Ϊ��������ģʽ������5����֮������������á�
#define	WF_REBOOT				"/system/sound/reboot.wav"		//���óɹ���ϵͳ���������У����Ժ�
#define	WF_CONFIGOK				"/system/sound/configok.wav"	//���óɹ�
#define	WF_OPENLOCK				"/system/sound/open.wav"		//�����ѿ�
#define	WF_WELCOME				"/system/sound/welcome.wav"		//��ӭ���١������У����Ժ�
#define	WF_CALL_TIMEOUT			"/system/sound/call-timeout.wav"//��ʱ���˽��������Ժ��ٲ���
#define	WF_CALLING				"/system/sound/calling.wav"		//��������˹
#define	WF_TALK_TIMEOUT			"/system/sound/talk-timeout.wav"//ͨ����ʱ
#define	WF_TALK_OVER			"/system/sound/talk-over.wav"	//�Է��ѹһ����ٴθ�л���Ĺ��١�
#define	WF_CONFIG_WIFI_OK		"/system/sound/reboot.wav"
#define	WF_RESET_OK				"/system/sound/reset-ok.wav"	//��λ�����ɹ���ϵͳ���������У����Ժ�
#define	WF_ALARM_IN				"/system/sound/alarm.wav"		//Alarm Sound
#define	WF_SCARE				"/system/sound/alarm.wav"		//Alarm Sound
#define	WF_MUTE         		"/system/sound/mute.wav"
#define	WF_PIR_ALARM			"/system/sound/pir-alarm.wav"	//CTD, PIR Alarm Sound //��ӭ���٣����Ѿ����������밴���壩
#define	WF_DHCP_SUCC    		"/system/sound/dhcp-success.wav"//DHCP SUCCESS
#define	WF_DHCP_FAIL    		"/system/sound/dhcp-fail.wav"	//DHCP FAIL
*/

#define	WF_STARTUP				0		//ϵͳ�����ɹ�
#define	WF_CONFIGING			1		//����ģʽ
#define WF_SMARTCONFIGOK		2		//һ������OK
#define WF_SMARTCONFIGNG		3		//һ������ʧ��
#define	WF_CONFIGOK				4		//���óɹ�
#define WF_CONFIGNG				5		//����ʧ��
#define	WF_REBOOT				6		//����
#define WF_RESET				7		//��λ
#define	WF_OPENLOCK				8		//����
#define WF_CALLING				9		//����
#define WF_CALL_AGREE			10		//���н�ͨ
#define WF_TALK_OVER			11		//ͨ������---��Ժ���ͨ��
#define WF_ALARM				12		//����
#define WF_MUTE					13		//����

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
	char codec[16];					//codec�ͺ�
}FACAUDIOCONFIG, *PFACAUDIOCONFIG;

typedef struct __CUSAUDIOCONFIG
{
	char enable;			//�Ƿ�ʹ��������ʾ
	int volOut;				//��������ٷֱ�
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
	char duplex;		//�Ƿ�ʹ��˫������
	char hard;			//Ӳ�����������
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
