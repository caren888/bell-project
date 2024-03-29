#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <imp/imp_audio.h>
#include <imp/imp_log.h>

#include "common.h"
#include "encrypt.h"
#include "middleware.h"
#include "audio.h"

#define AUDIORECORDBUFNUM	10
#define AUDIORECORDBUFLEN	2048
//#define AUDIORECORDBUFLEN	2048
#define AUDIOPLAYBUFNUM     5
#define AUDIOPLAYBUFLEN     2048			//play audio buffer len   			
//#define AUDIOPLAYBUFLEN      2 * 1024		//play audio buffer len   			

typedef struct __STREAMBUFFERINDEX
{
	int curIndex;
	int preIndex;
}STREAMBUFFERINDEX, PSTREAMBUFFERINDEX;

//RIFF Chunk
typedef struct
{
    char szRiffID[4];       // 'R','I','F','F'
    int dwRiffSize;
    char szRiffFormat[4];   // 'W','A','V','E'
} RIFF_HEADER;

//Format Chunk
typedef struct
{
    char  szFmtID[4];   // 'f','m','t',' '
    int  dwFmtSize;   //maybe 2 Bytes Extra Information!!!
}FMT_BLOCK;

typedef struct
{
    short wFormatTag;
    short wChannels;
    int dwSamplesPerSec;
    int dwAvgBytesPerSec;
    short wBlockAlign;
    short wBitsPerSample;
} WAVE_FORMAT;

//Fact Chunk(Option)
typedef struct
{
    char  szFactID[4];  // 'f','a','c','t'
    int  dwFactSize;
}FACT_BLOCK;

typedef struct
{
    short wChannels;
    int dwSamplesPerSec;
    short wBitsPerSample;
    int nWaveHeadSize;
    int nWaveDataSize;
	FILE *fp;
}WAVE_INFORMATION;


pthread_t recordThreadID;
pthread_t playFileThreadID;
pthread_t playAudioThreadID;

/*
**recordAudioFlag=1 now is recording audio
**playAudioFlag=1   now is play audio
**playMusicFlag=1   now is play music
*/

struct play_buf_attr {
	char *aPlayBuf;
	int aPlayBufLen;
	int aPlayBufFlag;	//0 empty,can write---1,full,can read and play---3 is playing, can not write
	int aPlayWritePos;
	int aPlayWriteLen;
	int aPlayReadPos;	
}aPlayBufAttr[AUDIOPLAYBUFNUM];
static int curWriteBuf;	 // 1 --write buffer 1, 2 --write buffer 2, 3 --write buffer 3,....
static int curPlayBuf;  // 1 ---buffer1;  2 ---buffer2, 3 ---buffer3

struct record_buf_attr {
	char *aRecordBuf;
	int aRecordBufLen;
	int aRecordBufFlag;	//0 empty,can write---1,full,can read and play---3 is sending, can not write
	int aRecordWritePos;
	int aRecordWriteLen;
	int aRecordReadPos;	
}aRecordBufAttr[AUDIORECORDBUFNUM];
static int curRecordWriteBuf;
static int curRecordReadBuf;



static int recordAudioFlag = 0;	    //recordAudioFlag=1(now is recording audio)
static int playAudioFlag = 0;		//playAudioFlag=1(now is palying audio)
static int playMusicFlag = 0;		//playMusicFlag=1(now is palying music)
static pthread_mutex_t mutexRecordAudioFlag = PTHREAD_MUTEX_INITIALIZER;
//static pthread_mutex_t mutexRecordAudioBuf  = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexPlayAudioFlag   = PTHREAD_MUTEX_INITIALIZER;
//static pthread_mutex_t mutexPlayAuioBuf     = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexForPlayFile     = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t playAudiocond;

static int playMusicInterrupted;

static sem_t semStartRecordAudio, semStartPlayAudio, semStartPlayMusic;
static sem_t semRecordOneAudio;

//static char playAudioBuffer[4 * AUDIO_SAMPLE_RATE_8000 * AUDIO_SOUND_MODE_MONO * 2 * 20 /1000];
//static int  playAudioLen;

char szWaveFile[128];
typedef void (*CALLBACK)(void);
int playCnt = 0;
int playInterval = 0;
CALLBACK playFunc;

/*
**return  :the length of data after changed
**function:change moto audio to stero audio(just for sampleBit=16 audio)
*/
static int mono_to_stero(char *srcBuffer, int len)
{
	int i = 0;
	short *buff = (short *)srcBuffer;
	len = len/2;
	for(i=len-1; i>=0; i--)
	{
		buff[2*i+1] = buff[i];
		buff[2*i]   = buff[i];
	}
//	DEBUG_INFO("---daemon---%15s,%5d,%15s---%s %4d %s return length=%d\n",__FILE__, __LINE__, __func__,  __FILE__, __LINE__, __FUNCTION__, 2*2*len);
	return 2*2*len;
}

/*
**change stero audio to moto audio(just for sampleBit=16 audio)
*/
static int stero_to_mono(char *srcBuffer, int len)
{
	int i = 0;
	short *buff = (short *)srcBuffer;
	len = len/2; 	//char length to short length
	len = len/2;	//stero length to mono length

	for(i=0; i<=len-1; i++)
	{
		buff[i] = buff[2*i];
	}
//	DEBUG_INFO("---daemon---%15s,%5d,%15s---%s %4d %s return length=%d\n",__FILE__, __LINE__, __func__,  __FILE__, __LINE__, __FUNCTION__, 2*len);
	return 2*len;
}

static int get_play_audio_flag(void)
{
	int bRet = 0;
//    pthread_mutex_lock(&mutexPlayAudioFlag);
    bRet = playAudioFlag;
//    pthread_mutex_unlock(&mutexPlayAudioFlag);
	return bRet;
}

static void set_play_music_flag(int value)
{
//    pthread_mutex_lock(&mutexPlayMusicFlag);
    playMusicFlag = value;
//    pthread_mutex_unlock(&mutexPlayMusicFlag);
}

static int get_play_music_flag(void)
{
	int bRet = 0;
//    pthread_mutex_lock(&mutexPlayMusicFlag);
    bRet = playMusicFlag;
//    pthread_mutex_unlock(&mutexPlayMusicFlag);
	return bRet;
}

static int get_data(char *buffer, int len)
{
	int ret = ERROR_SUCCESS;

	pthread_mutex_lock(&mutexPlayAudioFlag);	
GET_AGAIN:
//	DEBUG_INFO("curPlayBuf = %d, aPlayBufFlag = %d, aPlayReadPos = %d, aPlayWriteLen = %d", 
//		curPlayBuf, aPlayBufAttr[curPlayBuf].aPlayBufFlag, aPlayBufAttr[curPlayBuf].aPlayReadPos, aPlayBufAttr[curPlayBuf].aPlayWriteLen);
	if (aPlayBufAttr[curPlayBuf].aPlayBufFlag == 1) {
		if (aPlayBufAttr[curPlayBuf].aPlayReadPos+len > aPlayBufAttr[curPlayBuf].aPlayWriteLen) {
			aPlayBufAttr[curPlayBuf].aPlayBufFlag = 0;
			aPlayBufAttr[curPlayBuf].aPlayWritePos = 0;
			if (++curPlayBuf >= AUDIOPLAYBUFNUM)
				curPlayBuf = 0;
			goto GET_AGAIN;
		} else {
//			DEBUG_INFO("curPlayBuf = %d, aPlayReadPos = %d", curPlayBuf, aPlayBufAttr[curPlayBuf].aPlayReadPos);		
			memcpy(buffer, aPlayBufAttr[curPlayBuf].aPlayBuf+aPlayBufAttr[curPlayBuf].aPlayReadPos, len);
			aPlayBufAttr[curPlayBuf].aPlayReadPos += len;
		}
	} else {
		ret = ERR_NO_NEW_DATA;	
	}
	pthread_mutex_unlock(&mutexPlayAudioFlag);
	
	return ret;
}

static int push_data(char *buffer, int len)
{
	int ret = ERROR_SUCCESS;

	pthread_mutex_lock(&mutexPlayAudioFlag);	
PUSH_AGAIN:	
//	DEBUG_INFO("curWriteBuf = %d, aPlayBufFlag = %d, aPlayWritePos = %d, aPlayWriteLen = %d", 
//		curWriteBuf, aPlayBufAttr[curWriteBuf].aPlayBufFlag, aPlayBufAttr[curWriteBuf].aPlayWritePos, aPlayBufAttr[curWriteBuf].aPlayWriteLen);
	if (aPlayBufAttr[curWriteBuf].aPlayBufFlag == 0) {
		if (aPlayBufAttr[curWriteBuf].aPlayWritePos+len > aPlayBufAttr[curWriteBuf].aPlayBufLen) {
			aPlayBufAttr[curWriteBuf].aPlayWriteLen = aPlayBufAttr[curWriteBuf].aPlayWritePos;
			aPlayBufAttr[curWriteBuf].aPlayBufFlag = 1;
			aPlayBufAttr[curWriteBuf].aPlayReadPos = 0;
			if (++curWriteBuf >= AUDIOPLAYBUFNUM) {
				curWriteBuf = 0;				
			}
			goto PUSH_AGAIN;
		} else {
//			DEBUG_INFO("curWriteBuf = %d, aPlayWritePos = %d", curWriteBuf, aPlayBufAttr[curWriteBuf].aPlayWritePos);
			memcpy(aPlayBufAttr[curWriteBuf].aPlayBuf+aPlayBufAttr[curWriteBuf].aPlayWritePos, buffer, len);
			aPlayBufAttr[curWriteBuf].aPlayWritePos += len;
		}			
	} else {
		ret = ERR_BUFFER_FULL;
	}
	pthread_mutex_unlock(&mutexPlayAudioFlag);
	
	return ret;
}
	
static int analysis_music_file(char *fileName, WAVE_INFORMATION *waveInfor, int sampleRate, int channelMode, int sampleBit)
{
	FILE *fp = NULL;
    int bOK, iRet = ERROR_FALSE;
    int nWaveHeadSize = 0;

	DEBUG_INFO("---main---%15s,%5d,%15s---%s--%04d--%s():szWaveFile =%s\n",__FILE__, __LINE__, __func__,  __FILE__, __LINE__, __FUNCTION__, szWaveFile);
	
	fp = fopen(fileName, "rb");
	if (NULL == fp) {
		DEBUG_IMPORTANT_INFO("---main---%15s,%5d,%15s---open music file error\n",__FILE__, __LINE__, __func__ );
		return ERROR_FALSE;
    }
	
	bOK = FALSE;
	
    RIFF_HEADER oRIFF_HEADER;
    FMT_BLOCK oFMT_BLOCK;   //maybe Extra Char
    WAVE_FORMAT oWAVE_FORMAT;
    FACT_BLOCK oFACT_BLOCK; //Option

    //RIFF Chunk
    iRet = fread(&oRIFF_HEADER, 1, sizeof(RIFF_HEADER), fp);
	if ( iRet == sizeof(RIFF_HEADER)) {
		nWaveHeadSize += sizeof(RIFF_HEADER);
		//Format Chunk
		iRet = fread(&oFMT_BLOCK, 1, sizeof(FMT_BLOCK), fp);
		if ( iRet == sizeof(FMT_BLOCK)) {
		    nWaveHeadSize += sizeof(FMT_BLOCK);
		    iRet = fread(&oWAVE_FORMAT, 1, sizeof(WAVE_FORMAT), fp);
		    if ( iRet == sizeof(WAVE_FORMAT)) {				

				DEBUG_INFO("---daemon---%15s,%5d,%15s---Channels = %d, Sample = %d, Bits = %d, dwAvgBytesPerSec = %d\n",__FILE__, __LINE__, __func__,  oWAVE_FORMAT.wChannels, oWAVE_FORMAT.dwSamplesPerSec, oWAVE_FORMAT.wBitsPerSample, oWAVE_FORMAT.dwAvgBytesPerSec);
				if ( oWAVE_FORMAT.dwSamplesPerSec != sampleRate || oWAVE_FORMAT.wBitsPerSample != (short)sampleBit || (oWAVE_FORMAT.wChannels != 1 && oWAVE_FORMAT.wChannels != 2) ) {
					DEBUG_INFO("---daemon---%15s,%5d,%15s------->Audio Format ERROR",__FILE__, __LINE__, __func__ );
					fclose(fp);
					return ERROR_FALSE;
				}
			
		        nWaveHeadSize += sizeof(WAVE_FORMAT);
		        if ( oFMT_BLOCK.dwFmtSize != sizeof(WAVE_FORMAT)) {
		            nWaveHeadSize += oFMT_BLOCK.dwFmtSize - sizeof(WAVE_FORMAT);
		            fseek(fp, oFMT_BLOCK.dwFmtSize - sizeof(WAVE_FORMAT), SEEK_CUR);
		        }
		        //Option, Fact Chunk
		        while (1) {
		            iRet = fread(&oFACT_BLOCK, 1, sizeof(FACT_BLOCK), fp);
		            if ( iRet == sizeof(FACT_BLOCK) ) {
		                nWaveHeadSize += sizeof(FACT_BLOCK);
		                if ( oFACT_BLOCK.szFactID[0] == 'd' &&
		                     oFACT_BLOCK.szFactID[1] == 'a' &&
		                     oFACT_BLOCK.szFactID[2] == 't' &&
		                     oFACT_BLOCK.szFactID[3] == 'a') {
		                    //nWaveHeadSize += oFACT_BLOCK.dwFactSize;
//		                    nWaveDataSize = oFACT_BLOCK.dwFactSize;
		                    bOK = TRUE;
		                    break;
		                } else {
		                    nWaveHeadSize += oFACT_BLOCK.dwFactSize;
		                    fseek(fp, oFACT_BLOCK.dwFactSize, SEEK_CUR);
		                    continue;
		                }
		            } else {
						fclose(fp);
						return ERROR_FALSE;
					}
		        }
		    }
		}
	} else {
		fclose(fp);
		return ERROR_FALSE;
	}

	if (bOK == TRUE) {
		waveInfor->wChannels = oWAVE_FORMAT.wChannels;
		waveInfor->dwSamplesPerSec = oWAVE_FORMAT.dwSamplesPerSec;
		waveInfor->wBitsPerSample = oWAVE_FORMAT.wBitsPerSample;
		waveInfor->nWaveHeadSize = nWaveHeadSize;
		waveInfor->nWaveDataSize = oFACT_BLOCK.dwFactSize;
		waveInfor->fp = fp;
		return ERROR_SUCCESS;
	}

	return ERROR_FALSE;
}

static int push_encode_buffer(char *buffer, int len)
{
	int ret = ERROR_SUCCESS;

	pthread_mutex_lock(&mutexRecordAudioFlag);	
PUSH_AGAIN: 
//	DEBUG_INFO("curRecordWriteBuf = %d, aRecordBufFlag = %d, aRecordWritePos = %d, aRecordWriteLen = %d", 
//		curRecordWriteBuf, aRecordBufAttr[curRecordWriteBuf].aRecordBufFlag, aRecordBufAttr[curRecordWriteBuf].aRecordWritePos, 
//		aRecordBufAttr[curRecordWriteBuf].aRecordWriteLen);
	if (aRecordBufAttr[curRecordWriteBuf].aRecordBufFlag == 0) {
		if (aRecordBufAttr[curRecordWriteBuf].aRecordWritePos+len > aRecordBufAttr[curRecordWriteBuf].aRecordBufLen) {
			aRecordBufAttr[curRecordWriteBuf].aRecordWriteLen = aRecordBufAttr[curRecordWriteBuf].aRecordWritePos;
			aRecordBufAttr[curRecordWriteBuf].aRecordBufFlag = 1;
			aRecordBufAttr[curRecordWriteBuf].aRecordReadPos = 0;
			if (++curRecordWriteBuf >= AUDIORECORDBUFNUM) {
//				DEBUG_INFO("++curRecordWriteBuf >= %d", AUDIORECORDBUFNUM);				
				curRecordWriteBuf = 0;				
			}
			goto PUSH_AGAIN;
		} else {
//			DEBUG_INFO("curRecordWriteBuf = %d, aRecordReadPos = %d, aRecordWritePos = %d, len = %d", curRecordWriteBuf, aRecordBufAttr[curRecordWriteBuf].aRecordReadPos, aRecordBufAttr[curRecordWriteBuf].aRecordWritePos, len);
			memcpy(aRecordBufAttr[curRecordWriteBuf].aRecordBuf+aRecordBufAttr[curRecordWriteBuf].aRecordWritePos, buffer, len);
			aRecordBufAttr[curRecordWriteBuf].aRecordWritePos += len;
		}			
	} else {
//		DEBUG_INFO("ERR_BUFFER_FULL"); 				
		ret = ERR_BUFFER_FULL;
	}
	pthread_mutex_unlock(&mutexRecordAudioFlag);
	
	return ret;
}

static void stop_play_music(void)
{
	pthread_mutex_lock(&mutexForPlayFile);
	
	/*
	**如果执行该函数的时候，刚好开始了一个新的播放，需要做判断
	**如果是新的播放，因为已经发送了post，所有不需要担心, 如果是打断，不能让他结束，结束会丢失
	*/
	if(playMusicInterrupted==1)//说明还未处理
	{
		sem_post(&semStartPlayMusic);
	}
	else
	{
		set_play_music_flag(0);
	}
	
	pthread_mutex_unlock(&mutexForPlayFile);

}

/*
**return  :0--play a new file  1--interrupt a old file and play a new file
**function:do a detection for (play a new file or interrupt a old file)
*/
static int start_or_interrupt_play_music(char *path, int cnt, int interval, void (*call_back)(void))
{
	int result = -1;
	
	pthread_mutex_lock(&mutexForPlayFile);
	result = get_play_music_flag();
	if(result == 0) {
		set_play_music_flag(1);
		playMusicInterrupted = 0;
		memset(szWaveFile, 0, sizeof(szWaveFile));
		strcpy(szWaveFile, path);
		playCnt = cnt;
		playInterval = interval;
		playFunc = call_back;
		sem_post(&semStartPlayMusic);
		result = 0;
	} else {
		memset(szWaveFile, 0, sizeof(szWaveFile));
		strcpy(szWaveFile, path);
		playCnt = cnt;
		playInterval = interval;		
		playFunc = call_back;
		playMusicInterrupted = 1;		
		result = 1;
	}
	pthread_mutex_unlock(&mutexForPlayFile);

	return result;
}

static int mw_audio_get_audio(struct media_a *audio, char *buffer, int *len)
{
	int ret = ERROR_FALSE;
	if (audio->ready == 0) {
		return ERROR_FALSE;
	}
	if (*len < 800) {
		return ERR_BUF_INSUFFICIENT;
	} else {
		*len = 800;
	}
	
	pthread_mutex_lock(&mutexRecordAudioFlag);	
GET_AGAIN:
//	DEBUG_INFO("curRecordReadBuf = %d, aRecordBufFlag = %d, aRecordReadPos = %d, aRecordWriteLen = %d", 
//		curRecordReadBuf, aRecordBufAttr[curRecordReadBuf].aRecordBufFlag, aRecordBufAttr[curRecordReadBuf].aRecordReadPos, 
//		aRecordBufAttr[curRecordReadBuf].aRecordWriteLen);
	if (aRecordBufAttr[curRecordReadBuf].aRecordBufFlag == 1) {
		if (aRecordBufAttr[curRecordReadBuf].aRecordReadPos+*len > aRecordBufAttr[curRecordReadBuf].aRecordWriteLen) {
//			DEBUG_INFO("---main---%15s,%5d,%15s---readpos = %d, writepos = %d, *len = %d\n",__FILE__, __LINE__, __func__,  aRecordBufAttr[curRecordReadBuf].aRecordReadPos, aRecordBufAttr[curRecordReadBuf].aRecordWriteLen, *len);
			aRecordBufAttr[curRecordReadBuf].aRecordBufFlag = 0;
			aRecordBufAttr[curRecordReadBuf].aRecordWritePos = 0;
			if (++curRecordReadBuf >= AUDIORECORDBUFNUM)
				curRecordReadBuf = 0;
			goto GET_AGAIN;
		} else {
//			DEBUG_INFO("curRecordReadBuf = %d, aRecordReadPos = %d, aRecordWritePos = %d, len = %d", curRecordReadBuf, aRecordBufAttr[curRecordReadBuf].aRecordReadPos, aRecordBufAttr[curRecordReadBuf].aRecordWritePos, *len);		
			memcpy(buffer, aRecordBufAttr[curRecordReadBuf].aRecordBuf+aRecordBufAttr[curRecordReadBuf].aRecordReadPos, *len);
			aRecordBufAttr[curRecordReadBuf].aRecordReadPos += *len;
			ret = ERROR_SUCCESS;
		}
	} else {
//		DEBUG_INFO("ERR_NO_NEW_DATA");		
		ret = ERR_NO_NEW_DATA;	
	}
	pthread_mutex_unlock(&mutexRecordAudioFlag);
	
	return ret;
}


/*
**函数原型:int mw_audio_record_start(char  *recordBuffer,  int len)
**输入参数:recordBuffer--the buffer to store record audio
           len         --the length of recordBuffer
**返回值  :ERROR_SUCCESS--success,  ERROR_EXIST--the record is already started
**函数功能:start record audio
*/
static int mw_audio_record_start(struct media_a *audio)
{
	int i;
	if (audio==NULL)
		return ERR_PARAMETER;
	if (audio->ready == 0) {
		return ERROR_FALSE;
	}	
	pthread_mutex_lock(&mutexRecordAudioFlag);
	if (recordAudioFlag == 1) {
	} else {
		curRecordReadBuf = 0;
		curRecordWriteBuf = 0;
		for (i=0 ; i<AUDIORECORDBUFNUM; i++) {
			aRecordBufAttr[i].aRecordBufFlag = 0;
			aRecordBufAttr[i].aRecordWritePos = 0;
			aRecordBufAttr[i].aRecordWriteLen = 0;
			aRecordBufAttr[i].aRecordReadPos = 0;
		}
		recordAudioFlag = 1;
		sem_post(&semStartRecordAudio);
	}
	pthread_mutex_unlock(&mutexRecordAudioFlag);
	
	return ERROR_SUCCESS;
}

static int mw_audio_record_stop(struct media_a *audio)
{
	if (audio==NULL)
		return ERR_PARAMETER;
	if (audio->ready == 0) {
		return ERROR_FALSE;
	}	
	pthread_mutex_lock(&mutexRecordAudioFlag);
	if (recordAudioFlag == 0) {
	} else {
		recordAudioFlag = 0;
	}
	pthread_mutex_unlock(&mutexRecordAudioFlag);
	
	return ERROR_SUCCESS;
}

static int mw_audio_play_start(struct media_a *audio)
{
	int i;
	if (audio==NULL)
		return ERR_PARAMETER;
	if (audio->ready == 0) {
		return ERROR_FALSE;
	}
	pthread_mutex_lock(&mutexPlayAudioFlag);
	if (playAudioFlag == 1) {
	} else {
		curPlayBuf = 0;
		curWriteBuf = 0;
		for (i=0 ; i<AUDIOPLAYBUFNUM; i++) {
			aPlayBufAttr[i].aPlayBufFlag = 0;
			aPlayBufAttr[i].aPlayWritePos = 0;
			aPlayBufAttr[i].aPlayWriteLen = 0;
			aPlayBufAttr[i].aPlayReadPos = 0;
		}
		playAudioFlag = 1;		
		sem_post(&semStartPlayAudio);
	}
	pthread_mutex_unlock(&mutexPlayAudioFlag);
	
	return ERROR_SUCCESS;
}

static int mw_audio_play_stop(struct media_a *audio)
{
	if (audio==NULL)
		return ERR_PARAMETER;
	if (audio->ready == 0) {
		return ERROR_FALSE;
	}	
	pthread_mutex_lock(&mutexPlayAudioFlag);
	if (playAudioFlag == 0) {
	} else {
		playAudioFlag = 0;
	}
	pthread_mutex_unlock(&mutexPlayAudioFlag);
	
	return ERROR_SUCCESS;
}


static int mw_audio_play_voice(struct media_a *audio, char *buffer, int len)
{
	int ret;
	if (audio==NULL || buffer==NULL)
		return ERR_PARAMETER;
	if (audio->ready == 0) {
		return ERROR_FALSE;
	}	
	ret = push_data(buffer, len);	
	return ret;
}

/*
**函数原型:int mw_audio_play_file(char *fileName)
**输入参数:fileName
**返回值  :none
**函数功能:play music file
*/
static int mw_audio_play_file(struct media_a *audio, char *path, int cnt, int interval, void (*call_back)(void))
{
	if (audio==NULL)
		return ERR_PARAMETER;
	if (audio->ready == 0) {
		return ERROR_FALSE;
	}	
	if ((strlen(path) > 127) || (path ==NULL))
		return ERR_FILENAME;
	if (cnt < 0)
		return ERR_PLAYTIME;
		
	start_or_interrupt_play_music(path, cnt, interval, call_back);
	
	return ERROR_SUCCESS;
}

static int mw_audio_adc_param_get(struct media_a *audio, int *sampleRate, int *channelMode, int *sampleBit)
{
	int ret = -1;
	if (audio==NULL || sampleRate==NULL || channelMode==NULL || sampleBit==NULL)
		return ERR_PARAMETER;
	if (audio->ready == 0) {
		return ERROR_FALSE;
	}	
	ret = audio_adc_param_get(sampleRate, channelMode, sampleBit);
	if (ret != ERROR_SUCCESS) {
		return ERROR_FALSE;
	}	
	audio->adc_sam_rate = *sampleRate;
	audio->adc_ch_mode = *channelMode;
	audio->adc_sam_bit = *sampleBit;	
	return ERROR_SUCCESS;
}

static int mw_audio_dac_param_get(struct media_a *audio, int *sampleRate, int *channelMode, int *sampleBit)
{
	int ret = -1;
	if (audio==NULL || sampleRate==NULL || channelMode==NULL ||sampleBit==NULL)
		return ERR_PARAMETER;
	if (audio->ready == 0) {
		return ERROR_FALSE;
	}	
	ret = audio_dac_param_get(sampleRate, channelMode, sampleBit);
	if (ret != ERROR_SUCCESS) {
		return ERROR_FALSE;
	}	
	audio->dac_sam_rate = *sampleRate;
	audio->dac_ch_mode = *channelMode;
	audio->dac_sam_bit = *sampleBit;	
	return ERROR_SUCCESS;
}

static int mw_audio_adc_param_set(struct media_a *audio, int sampleRate, int channelMode, int sampleBit)
{
	int ret = -1;
	if (audio==NULL)
		return ERR_PARAMETER;

	if (audio->ready == 0) {
		return ERROR_FALSE;
	}
	
	ret = audio_adc_param_set(sampleRate, channelMode, sampleBit);
	return ret;
}

static int mw_audio_dac_param_set(struct media_a *audio, int sampleRate, int channelMode, int sampleBit)
{
	int ret = -1;
	if(audio==NULL)
		return ERR_PARAMETER;
	if (audio->ready == 0) {
		return ERROR_FALSE;
	}	
	ret = audio_dac_param_set(sampleRate, channelMode, sampleBit);
	return ret;
}

static int mw_audio_adc_volume_set(struct media_a *audio, int value)
{
	int ret = -1;
	if ((audio==NULL) ||(value < -30) || (value > 120))
		return ERR_SUPPORT_VOL;	
	if (audio->ready == 0) {
		return ERROR_FALSE;
	}	
	ret = audio_adc_volume_set(value);
	if (ret != ERROR_SUCCESS) {
		return ERROR_FALSE;
	}	
	return ERROR_SUCCESS;
}

static int mw_audio_adc_volume_get(struct media_a *audio, int *value)
{
	int ret = -1;
	if (audio==NULL || value==NULL)
		return ERR_PARAMETER;
	
	if (audio->ready == 0) {
		return ERROR_FALSE;
	}
	
	ret = audio_adc_volume_get(value);
	if (ret != ERROR_SUCCESS) {
		return ERROR_FALSE;
	}
	audio->adc_vol = *value;	
	return ERROR_SUCCESS;
}

static int mw_audio_dac_volume_set(struct media_a *audio, int value)
{
	int ret = -1;
	if(audio==NULL)
		return ERR_PARAMETER;
	
	if (audio->ready == 0) {
		return ERROR_FALSE;
	}
	
	if ((value < -30) || (value > 120))
		return ERR_SUPPORT_VOL;		
	ret = audio_dac_volume_set(value);
	if (ret != ERROR_SUCCESS) {
		return ERROR_FALSE;
	}	
	return ERROR_SUCCESS;
}

static int mw_audio_dac_volume_get(struct media_a *audio, int *value)
{
	int ret = -1;
	if (audio==NULL || value==NULL)
		return ERR_PARAMETER;

	if (audio->ready == 0) {
		return ERROR_FALSE;
	}
	
	ret = audio_dac_volume_get(value);
	if(ret != ERROR_SUCCESS) {
		return ERROR_FALSE;
	}
	audio->dac_vol = *value;		
	return ERROR_SUCCESS;
}

int mw_is_record_buffer_empty(void)
{
	pthread_mutex_lock(&mutexRecordAudioFlag);	
	pthread_mutex_unlock(&mutexRecordAudioFlag);
	return ERROR_SUCCESS;
}

int mw_is_play_buffer_empty(void)
{
	int i, iRet;
	
	pthread_mutex_lock(&mutexPlayAudioFlag);
	for (i=0 ; i<AUDIOPLAYBUFNUM; i++) {
		if (aPlayBufAttr[i].aPlayBufFlag == 0) {
			continue;
		} else {
			break;
		}
	}

	if (i>=AUDIOPLAYBUFNUM) {
		iRet = TRUE;
	} else {
		iRet = FALSE;
	}
	pthread_mutex_unlock(&mutexPlayAudioFlag);

	return iRet;
}

void mw_audio_clear_play(void)
{
	int i;
	
	pthread_mutex_lock(&mutexPlayAudioFlag);
	curPlayBuf = 0;
	curWriteBuf = 0;
	for (i=0 ; i<AUDIOPLAYBUFNUM; i++) {
		aPlayBufAttr[i].aPlayBufFlag = 0;
		aPlayBufAttr[i].aPlayWritePos = 0;
		aPlayBufAttr[i].aPlayWriteLen = 0;
		aPlayBufAttr[i].aPlayReadPos = 0;
	}
	pthread_mutex_unlock(&mutexPlayAudioFlag);
	
	return;
}

void mw_audio_clear_record(void)
{
	int i;
	
	pthread_mutex_lock(&mutexRecordAudioFlag);
	curRecordReadBuf = 0;
	curRecordWriteBuf = 0;
	for (i=0 ; i<AUDIORECORDBUFNUM; i++) {
		aRecordBufAttr[i].aRecordBufFlag = 0;
		aRecordBufAttr[i].aRecordWritePos = 0;
		aRecordBufAttr[i].aRecordWriteLen = 0;
		aRecordBufAttr[i].aRecordReadPos = 0;
	}
	pthread_mutex_unlock(&mutexRecordAudioFlag);

	clear_ao_chn();

	return;
}

/*
**Start audio recording thread. 
*/
static void *imp_audio_record_thread(void *p)
{	
	char buffer[4096];
//	char speexBuffer[4096];
	int len = 0, ret;

//	speex_init();
	append_pthread_pid("imp_audio_record_thread");
	
	while(1) {
		sem_wait(&semStartRecordAudio);	
		
		while (recordAudioFlag) {
			ret = get_audio_frame(buffer, &len);
			if(ret == ERROR_SUCCESS) {								
//				DEBUG_INFO("get audio frame len=%d", len);
//				speex_buffer(buffer, len, speexBuffer, sizeof(speexBuffer));
//				push_encode_buffer(speexBuffer, len);
				push_encode_buffer(buffer, len);
			} else {
//				DEBUG_ERR("---main---%15s,%5d,%15s---get audio frame failed ret=%d",__FILE__, __LINE__, __func__,  ret);
			}
			usleep(10 * 1000);
		}
	}

	pthread_exit(NULL);
}



static void *imp_audio_play_voice_thread(void *p)
{	
	char buffer[1024];
	int ret;

	append_pthread_pid("imp_audio_play_voice_thread");
	
	while (1) {
		sem_wait(&semStartPlayAudio);

		DEBUG_IMPORTANT_INFO("start play voice");
		
		while (playAudioFlag) {
			
			ret = get_data(buffer, 800);
			if (ret == ERROR_SUCCESS) {
				play_voice_buffer(buffer, 800);
			} else {
				usleep(10 * 1000);
//				DEBUG_ERR("---main---%s,%d,%s---get audio data error", __FILE__, __LINE__, __func__);
			}
			
		}
	}
	
	pthread_exit(NULL);
}

static void *imp_audio_play_file_thread(void *p)
{
    int iRet = ERROR_FALSE;
	int audioFrameReadSize = 0;
    char szBuffer[4096] = {0};
    int nReadSize = 0;
	int sampleRate = 0;
	int channelMode = 0;
	int sampleBit = 0;
	char fileName[128];
	int cnt;
	int interval;
	int playForever = 0;
	CALLBACK func; 
	
	struct media_a *audio = (struct media_a *)p;
	WAVE_INFORMATION waveInformation;

	append_pthread_pid("imp_audio_play_file_thread");
	
	while (1) {		
		sem_wait(&semStartPlayMusic);
		while (1) {

			DEBUG_INFO("---main---%s,%d,%s---playMusicInterrupted=%d, playCnt=%d, szWaveFile = %s\n", __FILE__, __LINE__, __func__, playMusicInterrupted, playCnt, szWaveFile);

			pthread_mutex_lock(&mutexForPlayFile);			
			if(playMusicInterrupted == 1)
				playMusicInterrupted = 0;

			nReadSize = 0;
			memset(fileName, 0, sizeof(fileName));
			strcpy(fileName, szWaveFile);
			cnt = playCnt;
			interval = playInterval;
			func = playFunc;
			pthread_mutex_unlock(&mutexForPlayFile);

			if (cnt == 0) {
				playForever = 1;
			} else {
				playForever = 0;
			}
							
			iRet = mw_audio_dac_param_get(audio, &sampleRate, &channelMode, &sampleBit);
			if (iRet != ERROR_SUCCESS) {
				goto PLAY_OVER;
			}
			
			iRet = analysis_music_file(fileName, &waveInformation, sampleRate, channelMode, sampleBit);
			if (iRet != ERROR_SUCCESS) {
				goto PLAY_OVER;
			}

			/*size of one frame*/
//			audioFrameReadSize = waveInformation.dwSamplesPerSec * waveInformation.wChannels * waveInformation.wBitsPerSample / 8 / 100;	//10ms
			audioFrameReadSize = waveInformation.dwSamplesPerSec * waveInformation.wChannels * waveInformation.wBitsPerSample / 8 / 100 * 5;//50ms

			while (1) {
				if (playMusicInterrupted == 1) {
					fclose(waveInformation.fp);
					break;
				}			

				/*read one frame*/
	            iRet = fread(szBuffer, 1, audioFrameReadSize, waveInformation.fp);

	            if ( iRet < 0 ) {
					if (playForever == 1) {
						usleep(interval * 1000);
						fseek(waveInformation.fp, waveInformation.nWaveHeadSize, SEEK_SET);
						continue;
					} else {
						if(--cnt > 0) {
							usleep(interval * 1000);
							fseek(waveInformation.fp, waveInformation.nWaveHeadSize, SEEK_SET);
							continue;
						} else {
							fclose(waveInformation.fp);
							if (func != NULL) {
								sleep(1);
								func();
							}							
							goto PLAY_OVER;
						}
					}
	            } else if (iRet == 0) {
	            	if (playForever == 1) {
						usleep(interval * 1000);
						fseek(waveInformation.fp, waveInformation.nWaveHeadSize, SEEK_SET);
						continue;
	            	} else {
						if(--cnt > 0) {
							usleep(interval * 1000);
							fseek(waveInformation.fp, waveInformation.nWaveHeadSize, SEEK_SET);
							continue;
						} else {
							fclose(waveInformation.fp);
							if (func != NULL) {
								sleep(1);
								func();
							}
							goto PLAY_OVER;
						}
	            	
	            	}
	            } else {
	                nReadSize += iRet;
					//DEBUG_INFO("---main---%15s,%5d,%15s---nReadSize = %d, waveInformation.nWaveDataSize = %d\n",__FILE__, __LINE__, __func__,  nReadSize, waveInformation.nWaveDataSize);					
	                if ( nReadSize >= waveInformation.nWaveDataSize ) {
	                    nReadSize = 0;
						play_music_buffer(szBuffer, iRet);
						if (playForever == 1) {
							usleep(interval * 1000);
							fseek(waveInformation.fp, waveInformation.nWaveHeadSize, SEEK_SET);
							continue;
						} else {
							if(--cnt > 0) {
								usleep(interval * 1000);
								fseek(waveInformation.fp, waveInformation.nWaveHeadSize, SEEK_SET);
								continue;
							} else {
								fclose(waveInformation.fp);
								if (func != NULL) {
									sleep(1);
									func();
								}
								goto PLAY_OVER;
							}
						
						}
	                } else {
						if((waveInformation.wChannels==1)&&(channelMode==2)) {
							iRet = mono_to_stero(szBuffer, iRet);
							play_music_buffer(szBuffer, iRet);
						} else if ((waveInformation.wChannels==2)&&(channelMode==1)) {
							iRet = stero_to_mono(szBuffer, iRet);
							play_music_buffer(szBuffer, iRet);
						} else {
							play_music_buffer(szBuffer, iRet);
						}
						continue;
	                }
	            }
	        }
		}
PLAY_OVER:
		stop_play_music();
	}

	pthread_exit(NULL);	

	return (void*)0;
}

static int init_media_a(struct media_a *audio)
{
	audio->adc_param_set = mw_audio_adc_param_set;
	audio->adc_param_get = mw_audio_adc_param_get;
	audio->dac_param_set = mw_audio_dac_param_set;
	audio->dac_param_get = mw_audio_dac_param_get;
	audio->adc_vol_set = mw_audio_adc_volume_set;
	audio->adc_vol_get = mw_audio_adc_volume_get;
	audio->dac_vol_set = mw_audio_dac_volume_set;
	audio->dac_vol_get = mw_audio_dac_volume_get;
	audio->record_start = mw_audio_record_start;
	audio->record_stop = mw_audio_record_stop;
	audio->talk_start = mw_audio_play_start;
	audio->talk_stop = mw_audio_play_stop;
	audio->play_file = mw_audio_play_file;
	audio->get_audio = mw_audio_get_audio;
	audio->play_voice = mw_audio_play_voice;

	return ERROR_SUCCESS;
}

int mw_media_audio_open(struct media_a *audio)
{
	int result = ERROR_FALSE;
	int i, j;

	audio->ready = 0;
	
	sem_init(&semStartRecordAudio, 0, 0);
	sem_init(&semRecordOneAudio, 0, 0);
	sem_init(&semStartPlayAudio, 0, 0);
	sem_init(&semStartPlayMusic, 0, 0);

	for (j=0; j<AUDIORECORDBUFNUM; j++) {
		aRecordBufAttr[j].aRecordBufLen = AUDIORECORDBUFLEN;
		aRecordBufAttr[j].aRecordBuf = (char *)malloc(aRecordBufAttr[j].aRecordBufLen);
		if (aRecordBufAttr[j].aRecordBuf == NULL)
			goto ERR_A_OPEN_7;
	}

	for (i=0; i<AUDIOPLAYBUFNUM; i++) {
		aPlayBufAttr[i].aPlayBufLen = AUDIOPLAYBUFLEN;
		aPlayBufAttr[i].aPlayBuf = (char *)malloc(aPlayBufAttr[i].aPlayBufLen);
		if (aPlayBufAttr[i].aPlayBuf == NULL)
			goto ERR_A_OPEN_6;
	}
	
	result = init_media_a(audio);
	if (result != ERROR_SUCCESS) {
		goto ERR_A_OPEN_6;
	}

	result  = audio_record_init();
	if (result != ERROR_SUCCESS) {
		goto ERR_A_OPEN_5;
	}
	
	result  = audio_play_init();
	if (result != ERROR_SUCCESS) {
		goto ERR_A_OPEN_4;
	}
	
	result = pthread_create(&recordThreadID, NULL, imp_audio_record_thread, audio);
	if (result != 0) {
		goto ERR_A_OPEN_3;
	}

	result = pthread_create(&playFileThreadID, NULL, imp_audio_play_file_thread, audio);
	if (result != 0) {
		goto ERR_A_OPEN_2;
	}

	result = pthread_create(&playAudioThreadID, NULL, imp_audio_play_voice_thread, NULL);
	if (result != 0) {
		goto ERR_A_OPEN_1;
	}

	audio->ready = 1;
	
	return ERROR_SUCCESS;

ERR_A_OPEN_1:
	pthread_cancel(playFileThreadID);
	pthread_join(playFileThreadID, NULL);	
ERR_A_OPEN_2:
	pthread_cancel(recordThreadID);
	pthread_join(recordThreadID, NULL);	
ERR_A_OPEN_3:
	audio_play_uninit();
ERR_A_OPEN_4:
	audio_record_uninit();
ERR_A_OPEN_5:
	
ERR_A_OPEN_6:
	for (; i>=1; i--) {
		free(aPlayBufAttr[i-1].aPlayBuf);
	}	
ERR_A_OPEN_7:
	for (; j>=1; j--) {
		free(aRecordBufAttr[j-1].aRecordBuf);
	}	
	return ERROR_FALSE;
}


int mw_media_audio_close(struct media_a *audio)
{
	int i;
	//1. 停止工作线程；
	pthread_cancel(recordThreadID);
	pthread_join(recordThreadID, NULL);
	pthread_cancel(playFileThreadID);
	pthread_join(playFileThreadID, NULL);
	pthread_cancel(playAudioThreadID);
	pthread_join(playAudioThreadID, NULL);

	//2. 关闭codec，其他处理；
	audio_play_uninit();
	audio_record_uninit();
	//3. 释放audio结构中指针；
	for (i=0; i<AUDIOPLAYBUFNUM; i++) {
		free(aPlayBufAttr[i].aPlayBuf);
	}	
	for (i=0; i<AUDIORECORDBUFNUM; i++) {
		free(aRecordBufAttr[i].aRecordBuf);
	}	

	return ERROR_SUCCESS;
}

