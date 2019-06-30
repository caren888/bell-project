#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h> 
#include <fcntl.h>

#include <common.h>
#include <middleware.h>
#include "my_audio.h"

MYAUDIOCONFIG myAudioConfig;

pthread_mutex_t mutexPlayFile = PTHREAD_MUTEX_INITIALIZER;

static sem_t myAudioSendSem;
static sem_t myAudioRecvSem;
static sem_t myAudioManageSem;


void set_audio_config(MYAUDIOCONFIG imyAudioConfig)
{
	memcpy(&myAudioConfig, &imyAudioConfig, sizeof(MYAUDIOCONFIG));
}

void get_audio_config(MYAUDIOCONFIG *imyAudioConfig)
{
	memcpy(imyAudioConfig, &myAudioConfig, sizeof(MYAUDIOCONFIG));
}

int StartPlayAlarmAudio(int type, void (*call_back)(void))
{
	int iRet;
	int cnt, interval;
	struct media_a *audio = NULL;
	static int curPlayType = -1;
	char filePath[128];

	audio = get_media_audio();
	if (audio == NULL) {
		return ERROR_FALSE;
	}

	printf("play music===========================\n");
	pthread_mutex_lock(&mutexPlayFile);

	if (curPlayType == WF_ALARM) {
		pthread_mutex_unlock(&mutexPlayFile);
		return ERROR_SUCCESS;
	}

	switch (type) {
		case WF_STARTUP:							//开机
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVEBOOT);			
			cnt = 1;
			interval = 0;
			break;
		case WF_CONFIGING:							//配置中
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVECONFIG);
			cnt = 0;
			interval = 1000;			
			break;
		case WF_SMARTCONFIGOK:						//一键配置成功
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVESMARTCONFIGOK);
			cnt = 1;
			interval = 0;						
			break;
		case WF_SMARTCONFIGNG:						//一键配置失败
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVESMARTCONFIGNG);
			cnt = 1;
			interval = 0;						
			break;
		case WF_CONFIGOK:							//配置成功
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVECONFIGOK);
			cnt = 1;
			interval = 0;						
			break;
		case WF_CONFIGNG:							//配置失败
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVECONFIGNG);
			cnt = 1;
			interval = 0;									
			break;
		case WF_REBOOT:								//重启
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVEDING);
			cnt = 1;
			interval = 1000;									
			break;
		case WF_RESET:								//复位
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVEDING);
			cnt = 3;
			interval = 1000;									
			break;
		case WF_OPENLOCK:							//开锁
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVEOPENDOOR);
			cnt = 1;
			interval = 0;									
			break;
		case WF_CALLING:
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVECALLING);			//呼叫
			cnt = 0;
			interval = 2000;									
			break;
		case WF_CALL_AGREE:
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVEMUTE);			//呼叫接听
			cnt = 1;
			interval = 0;									
			break;
		case WF_TALK_OVER:
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVETALKOVER);			//通话结束
			cnt = 1;
			interval = 0;									
			break;
		case WF_ALARM:
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVEALARM);			//报警
			cnt = 0;
			interval = 0;									
			break;
		case WF_MUTE:
			curPlayType = type;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVEMUTE);				//静音			
			cnt = 1;
			interval = 0;									
			break;
		default:
			curPlayType = WF_MUTE;
			memset(filePath, 0, sizeof(filePath));
			strcpy(filePath, WAVEMUTE);
			cnt = 1;
			interval = 0;									
			break;			
	}

	iRet = audio->play_file(audio, filePath, cnt, interval, call_back);
	DEBUG_IMPORTANT_INFO("---main---%s,%d,%s---play_file result is %d, cnt = %d, interval = %d\n", __FILE__, __LINE__ , __func__, iRet, cnt, interval);
	
	pthread_mutex_unlock(&mutexPlayFile);
	
	return iRet;
}


//百分比0~100,实际音量-30~120;;;;;;;因为实际情况需要，把音量去掉两头, 10 ~ 110
int my_audio_set_voice(int voice)
{
	struct media_a *audio = NULL;
	int realVoice;
	int iRet;

	audio = get_media_audio();
	if (audio == NULL) {
		return ERROR_FALSE;
	}

	if (voice<0 || voice>100) {
		return ERROR_FALSE;
	}

	if (voice % 2) {
		voice += 1;
	}

	realVoice = 100 * voice / 100;

	realVoice = realVoice + 10;

	if (realVoice < 10) {
		realVoice = 10;		
	}

	if (realVoice > 110) {
		realVoice = 110;
	}

	iRet = audio->dac_vol_set(audio, realVoice);

	return iRet;	
}

int my_audio_get_audio(char *buffer, int *len)
{
	int iRet;
	struct media_a *laudio;

	if (buffer==NULL || len==NULL) {
		return ERR_PARAMETER;
	}

	laudio = get_media_audio();
	if (laudio == NULL) {
		return ERROR_FALSE;
	}

	iRet = laudio->get_audio(laudio, buffer, len);
	if ( iRet != ERROR_SUCCESS ) {
//		DEBUG_ERR("get audio error, iRet = %d, len = %d\n", iRet, len);
		return ERROR_FALSE;
	}
	
	if ( ( *len >= MAXAUDIOFRAMESIZE ) || ( *len <= 0x00 ) ) {
		DEBUG_ERR("---main---%15s,%5d,%15s---audio Size = %d, TOO LARGE---",__FILE__, __LINE__, __func__,  *len);
		return ERROR_FALSE;
	}	

	return ERROR_SUCCESS;	
}


int my_audio_uninit(void)
{
	return ERROR_SUCCESS;
}

int my_audio_init(void)
{
	int iRet;

	sem_init(&myAudioManageSem, 0, 0);
	sem_init(&myAudioSendSem, 0, 0);
	sem_init(&myAudioRecvSem, 0, 0);

	struct media_a *audio;

	audio = get_media_audio();
	if (audio == NULL) {
		return ERROR_FALSE;
	}

	myAudioConfig.sysAudioConfig.sampleIn = 8000;
	myAudioConfig.sysAudioConfig.channelIn = 1;
	myAudioConfig.sysAudioConfig.depthIn = 16;	
	iRet = audio->adc_param_set(audio, myAudioConfig.sysAudioConfig.sampleIn, myAudioConfig.sysAudioConfig.channelIn, myAudioConfig.sysAudioConfig.depthIn);
	if (iRet != ERROR_SUCCESS) {
		return iRet;
	}

	myAudioConfig.sysAudioConfig.sampleOut = 8000;
	myAudioConfig.sysAudioConfig.channelOut= 1; 
	myAudioConfig.sysAudioConfig.depthOut = 16;
	iRet = audio->dac_param_set(audio, myAudioConfig.sysAudioConfig.sampleOut, myAudioConfig.sysAudioConfig.channelOut, myAudioConfig.sysAudioConfig.depthOut);
	if (iRet != ERROR_SUCCESS) {
		return iRet;
	}

	iRet = audio->adc_vol_set(audio, myAudioConfig.sysAudioConfig.volIn);
	if (iRet !=ERROR_SUCCESS) {
		return iRet;
	}

	
//	iRet = audio->dac_vol_set(audio, myAudioConfig.sysAudioConfig.volOut);

//	iRet = audio->dac_vol_set(audio, myAudioConfig.cusAudioConfig.volOut);
	myAudioConfig.cusAudioConfig.volOut = 50;
	if (myAudioConfig.cusAudioConfig.volOut < 0  || myAudioConfig.cusAudioConfig.volOut > 100) {
		myAudioConfig.cusAudioConfig.volOut = 50;		
	}
	iRet = my_audio_set_voice(myAudioConfig.cusAudioConfig.volOut);
	if (iRet != ERROR_SUCCESS) {
		return iRet;
	}	

	return ERROR_SUCCESS;
}


