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
#include "audio.h"

#define INIT_AI_ERR_SUCESS      0		//init ai success
#define INIT_AI_ERR_SET_ATTR    1		//IMP_AI_SetPubAttr  error
#define INIT_AI_ERR_ENABLE_AI   2		//IMP_AI_Enable  error
#define INIT_AI_ERR_SET_CHNPAR  3		//IMP_AI_SetChnParam  error
#define INIT_AI_ERR_ENABLE_CHN  4		//IMP_AI_EnableChn  error
#define INIT_AI_ERR_SET_VOL     5		//IMP_AI_SetVol  error
#define INIT_AI_ERR_ENABLE_HPF  6		//IMP_AI_EnableHpf  error
#define INIT_AI_ERR_ENABLE_NS   7		//IMP_AI_EnableNs  error
#define INIT_AI_ERR_REG_ENC     8		//IMP_AENC_RegisterEncoder  error
#define INIT_AI_ERR_CREA_ENC    9		//IMP_AENC_CreateChn  error

#define INIT_AO_ERR_SUCESS      0		//init ao success
#define INIT_AO_ERR_SET_ATTR    1		//IMP_AO_SetPubAttr  error
#define INIT_AO_ERR_ENABLE_AO   2		//IMP_AO_Enable  error
#define INIT_AO_ERR_ENABLE_CHN  3		//IMP_AO_EnableChn  error
#define INIT_AO_ERR_SET_VOL     4		//IMP_AO_SetVol  error
#define INIT_AO_ERR_ENABLE_HPF  5		//IMP_AO_EnableHpf  error
#define INIT_AO_ERR_REG_DEC     6		//IMP_ADEC_RegisterDecoder  error
#define INIT_AO_ERR_CREA_DEC    7		//IMP_ADEC_CreateChn  error


typedef struct {
	int deviceID;
	int chnID;
}IMPAUDIOCELL;

typedef enum{
	MW_PT_PCM = 0, 		//be same with PT_PCM
	MW_PT_G711A = 1,	//be same with PT_G711A	
	MW_PT_G711U = 2,	//be same with PT_G711U
	MW_PT_G726 = 3,		//be same with PT_G726
	MW_PT_AEC = 4,
	MW_PT_ADPCM = 5,
	MW_PT_MAX = 6,
	MY_G711A = 7,
	MY_G711U = 8,
	MY_5350AUDIOENCDEC = 9
}IMPAudioEncDecName;


/*
**音频输入通道默认属性:采样率8K，采样精度16bit，单声道，音量60，编码格式PT_G711A
**音频输出通道默认属性:采样率8K，采样精度16bit，单声道，音量60，解码格式PT_G711A
*/

static IMPAUDIOCELL aiCell = {
	.deviceID = 1,
	.chnID = 0
};

static IMPAUDIOCELL aoCell = {
	.deviceID = 0,
	.chnID = 0
};

static IMPAudioIOAttr aiAttr = {
	.samplerate	= AUDIO_SAMPLE_RATE_8000,
	.bitwidth   = AUDIO_BIT_WIDTH_16,
	.soundmode  = AUDIO_SOUND_MODE_MONO,
	.frmNum     = 3,//20,
//	.numPerFrm	= 160,					//20ms 
	.numPerFrm	= 400,					//50ms 
	.chnCnt     = 1
};

static IMPAudioIOAttr aoAttr = {
	.samplerate	= AUDIO_SAMPLE_RATE_8000,
	.bitwidth   = AUDIO_BIT_WIDTH_16,
	.soundmode  = AUDIO_SOUND_MODE_MONO,
	.frmNum     = 3,//30,
	.numPerFrm	= 400,					//50ms
//	.numPerFrm	= 160,					//20ms
	.chnCnt     = 1
};

static int aivol = 60;
static int aovol = 60;
static IMPAudioEncDecName audioEncName = MW_PT_PCM;//MY_G711A;//MW_PT_G711A;
static IMPAudioEncDecName audioDecName = MW_PT_PCM;//MY_G711A;//MW_PT_G711A;

static int handleDecoder;
static int handleEncoder;
static int initAudioRecordErr = INIT_AI_ERR_SUCESS;
static int initAudioPlayErr   = INIT_AO_ERR_SUCESS;

/*
**函数原型:int init_audio_decoder(IMPAudioEncDecName audioEncoderName)
**输入参数:encoder type
**返回值  :0 success;
**函数功能:init encoder----if you not use system encoder, you must registe your encoder first
**         default encoder is PT_G711A			
*/
static int init_audio_decoder(IMPAudioEncDecName audioDecoderName)
{
	int ret = -1;
	IMPAudioPalyloadType handle = 0;
	IMPAudioDecDecoder myDecoder;
	int adChn = 0;
	IMPAudioDecChnAttr decoderChnAttr;
	
	memset(&myDecoder, 0x0, sizeof(myDecoder));
	myDecoder.openDecoder = NULL;
	myDecoder.closeDecoder = NULL;
	myDecoder.getFrmInfo = NULL;
	
	switch(audioDecoderName)
	{
		case MW_PT_PCM:				//not sure
			handle = PT_PCM;
			handleDecoder = (int)handle;
			break;
		case MW_PT_G711A:
			handle = PT_G711A;
			handleDecoder = (int)handle;
			break;
		case MW_PT_G711U:
			handle = PT_G711U;
			handleDecoder = (int)handle;
			break;
		case MW_PT_G726:
			handle = PT_G726;
			handleDecoder = (int)handle;
			break;
		case MW_PT_AEC:
			handle = PT_AEC;
			handleDecoder = (int)handle;
			break;
		case MW_PT_ADPCM:
			handle = PT_ADPCM;
			handleDecoder = (int)handle;
			break;
		case MW_PT_MAX:
			handle = PT_MAX;
			handleDecoder = (int)handle;
			break;
		case MY_G711A:
			sprintf(myDecoder.name, "%s", "MY_G711A");
			myDecoder.decodeFrm = MY_G711A_Decode_Frm;
			ret = IMP_ADEC_RegisterDecoder(&handleDecoder, &myDecoder);
			if(ret != 0) {
				initAudioPlayErr = INIT_AO_ERR_REG_DEC;
				return ERROR_FALSE;
			}
			break;
		case MY_G711U:
			sprintf(myDecoder.name, "%s", "MY_G711U");
			myDecoder.decodeFrm = MY_G711U_Decode_Frm;
			ret = IMP_ADEC_RegisterDecoder(&handleDecoder, &myDecoder);
			if(ret != 0) {
				initAudioPlayErr = INIT_AO_ERR_REG_DEC;
				return ERROR_FALSE;
			}
			break;
		case MY_5350AUDIOENCDEC:
			sprintf(myDecoder.name, "%s", "MY_5350");
			myDecoder.decodeFrm = MY_5350_Decode_Frm;
			ret = IMP_ADEC_RegisterDecoder(&handleDecoder, &myDecoder);
			if(ret != 0) {
				initAudioPlayErr = INIT_AO_ERR_REG_DEC;
				return ERROR_FALSE;
			}
			break;
		default:
			handle = PT_G711A;
			handleDecoder = (int)handle;
			break;
	}

	/* audio encode create channel. */
	decoderChnAttr.type = handleDecoder; 
	decoderChnAttr.bufSize = 30;
	decoderChnAttr.mode = ADEC_MODE_PACK;

	ret = IMP_ADEC_CreateChn(adChn, &decoderChnAttr);
	if(ret != 0) {
		initAudioPlayErr = INIT_AO_ERR_CREA_DEC;
		return ERROR_FALSE;
	}
	
	ret = IMP_ADEC_ClearChnBuf(adChn);
	if(ret != 0) {
		initAudioPlayErr = INIT_AO_ERR_CREA_DEC;
		return ERROR_FALSE;
	}
	return ERROR_SUCCESS;		
}


/*
**函数原型:int init_audio_encoder(IMPAudioEncDecName audioEncoderName)
**输入参数:encoder type
**返回值  :0 success;
**函数功能:init encoder----if you not use system encoder, you must registe your encoder first
**         default encoder is PT_G711A			
*/
static int init_audio_encoder(IMPAudioEncDecName audioEncoderName)
{
	int ret = -1;
	IMPAudioPalyloadType handle = 0;
	IMPAudioEncEncoder myEncoder;
	int aeChn = 0;
	IMPAudioEncChnAttr encoderChnAttr;
	
	memset(&myEncoder, 0x0, sizeof(myEncoder));
	myEncoder.maxFrmLen = 1024;
	myEncoder.openEncoder = NULL;
	myEncoder.closeEncoder = NULL;

	switch(audioEncoderName)
	{
		case MW_PT_PCM:				//not sure
			handle = PT_PCM;
			handleEncoder = (int)handle;
			break;
		case MW_PT_G711A:
			handle = PT_G711A;
			handleEncoder = (int)handle;
			break;
		case MW_PT_G711U:
			handle = PT_G711U;
			handleEncoder = (int)handle;
			break;
		case MW_PT_G726:
			handle = PT_G726;
			handleEncoder = (int)handle;
			break;
		case MW_PT_AEC:
			handle = PT_AEC;
			handleEncoder = (int)handle;
			break;
		case MW_PT_ADPCM:
			handle = PT_ADPCM;
			handleEncoder = (int)handle;
			break;
		case MW_PT_MAX:
			handle = PT_MAX;
			handleEncoder = (int)handle;
			break;			
		case MY_G711A:
			sprintf(myEncoder.name, "%s", "MY_G711A");
			myEncoder.encoderFrm = MY_G711A_Encode_Frm;
			ret = IMP_AENC_RegisterEncoder(&handleEncoder, &myEncoder);
			if(ret != 0) {
				DEBUG_IMPORTANT_INFO("%s  %04d  IMP_AENC_RegisterEncoder failed\n", __FILE__, __LINE__);
				return ERROR_FALSE;
			}
			break;
		case MY_G711U:
			sprintf(myEncoder.name, "%s", "MY_G711U");
			myEncoder.encoderFrm = MY_G711U_Encode_Frm;
			ret = IMP_AENC_RegisterEncoder(&handleEncoder, &myEncoder);
			if(ret != 0) {
				DEBUG_IMPORTANT_INFO("%s  %04d  IMP_AENC_RegisterEncoder failed\n", __FILE__, __LINE__);
				return ERROR_FALSE;
			}
			break;
		case MY_5350AUDIOENCDEC:
			sprintf(myEncoder.name, "%s", "MY_5350");
			myEncoder.encoderFrm = MY_5350_Encode_Frm;
			ret = IMP_AENC_RegisterEncoder(&handleEncoder, &myEncoder);
			if(ret != 0) {
				DEBUG_IMPORTANT_INFO("%s  %04d  IMP_AENC_RegisterEncoder failed\n", __FILE__, __LINE__);
				return ERROR_FALSE;
			}
			break;
		default:
			handle = PT_G711A;
			handleEncoder = (int)handle;
			break;
	}

	/* audio encode create channel. */
	encoderChnAttr.type = handleEncoder; 
	encoderChnAttr.bufSize = 24;
	ret = IMP_AENC_CreateChn(aeChn, &encoderChnAttr);
	if(ret != 0) {
		DEBUG_IMPORTANT_INFO("%s  %04d  imp audio encode create channel failed\n", __FILE__, __LINE__);
		return ERROR_FALSE;
	}

	return ERROR_SUCCESS;		
}

void clear_ao_chn(void)
{
	IMP_AO_ClearChnBuf(aoCell.deviceID, aoCell.chnID);
}

int get_audio_frame(char *buffer, int *len)
{
	int ret = -1;
	int i = 0, writeLen = 0;
	IMPAudioFrame frm;
	int AeChn = 0;		//AeChn is equal to the AeChn in function --static int init_audio_encoder(IMPAudioEncDecName audioEncoderName)
	IMPAudioStream stream;
		
	for	(i = 0; i < 1; i++) {	
		ret = IMP_AI_PollingFrame(aiCell.deviceID, aiCell.chnID, 1000);
		if (ret != 0 ) {
			DEBUG_IMPORTANT_INFO("%s %04d %s Audio Polling Frame Data error\n", __FILE__, __LINE__, __FUNCTION__);
			return ERROR_FALSE;
		}
		ret = IMP_AI_GetFrame(aiCell.deviceID, aiCell.chnID, &frm, BLOCK);
		if(ret != 0) {
			DEBUG_IMPORTANT_INFO("%s %04d %s Audio Get Frame Data error\n", __FILE__, __LINE__, __FUNCTION__);
			return ERROR_FALSE;
		}

		if (audioEncName == MW_PT_PCM) {
			memcpy(buffer + writeLen, frm.virAddr, frm.len);
			writeLen += frm.len;
			*len = writeLen;
			ret = IMP_AI_ReleaseFrame(aiCell.deviceID, aiCell.chnID, &frm);
			if(ret != 0) {
				DEBUG_IMPORTANT_INFO("%s %04d %s Audio release frame data error\n", __FILE__, __LINE__, __FUNCTION__);
				return ERROR_FALSE;
			}			
		} else {
			ret = IMP_AENC_SendFrame(AeChn, &frm);
			if(ret != 0) {
				DEBUG_IMPORTANT_INFO("%s %04d %s imp audio encode send frame failed\n", __FILE__, __LINE__, __FUNCTION__);
				ret = IMP_AI_ReleaseFrame(aiCell.deviceID, aiCell.chnID, &frm);
				if(ret != 0) {
					DEBUG_INFO("---daemon---%15s,%5d,%15s---%s %04d %s Audio release frame data error\n",__FILE__, __LINE__, __func__,  __FILE__, __LINE__, __FUNCTION__);
					return ERROR_FALSE;
				}							
				return ERROR_FALSE;
			} 
			ret = IMP_AI_ReleaseFrame(aiCell.deviceID, aiCell.chnID, &frm);
			if(ret != 0) {
				DEBUG_IMPORTANT_INFO("%s %04d %s Audio release frame data error\n", __FILE__, __LINE__, __FUNCTION__);
				return ERROR_FALSE;
			}			
			ret = IMP_AENC_PollingStream(AeChn, 1000);
			if (ret != 0) {
				DEBUG_IMPORTANT_INFO("%s %04d %s imp audio encode polling stream failed\n", __FILE__, __LINE__, __FUNCTION__);
				return ERROR_FALSE;
			}
			ret = IMP_AENC_GetStream(AeChn, &stream, BLOCK);
			if(ret != 0) {
				DEBUG_IMPORTANT_INFO("%s %04d %s imp audio encode get stream failed\n", __FILE__, __LINE__, __FUNCTION__);
				return ERROR_FALSE;
			}
			memcpy(buffer + writeLen, stream.stream, stream.len);
			writeLen += stream.len;
			*len = writeLen;
			//	push_encode_buffer(&stream);
//			DEBUG_INFO("src len = %d, encoder len = %d", frm.len, stream.len);
			
			ret = IMP_AENC_ReleaseStream(AeChn, &stream);
			if(ret != 0) {
				DEBUG_IMPORTANT_INFO("%s %04d %s imp audio encode release stream failed\n", __FILE__, __LINE__, __FUNCTION__);
				return ERROR_FALSE;
			}
		}		
	}
	
	return ERROR_SUCCESS;
}

int play_voice_buffer(char *buffer, int len)
{
	//DEBUG_INFO("---daemon---%15s,%5d,%15s---play_voice_buffer =================\n",__FILE__, __LINE__, __func__ );
	int ret = -1;
	int AdChn = 0;
	IMPAudioStream stream_in;
	IMPAudioStream stream_out;
	IMPAudioFrame frm;

//	DEBUG_INFO("play voice len = %d", len);
	
	if (audioDecName != MW_PT_PCM) {
	
		stream_in.stream = (unsigned char *)buffer;
		stream_in.len = len;
		ret = IMP_ADEC_SendStream(AdChn, &stream_in, BLOCK);
		if (ret != 0) {
			DEBUG_IMPORTANT_INFO("%s  %04d  %s  imp audio encode send frame failed\n", __FILE__, __LINE__, __FUNCTION__);
			return ERROR_FALSE;
		}
	
		ret = IMP_ADEC_PollingStream(AdChn, 1000);
		if (ret != 0) {
			DEBUG_IMPORTANT_INFO("%s  %04d  %s  imp audio encode polling stream failed\n", __FILE__, __LINE__, __FUNCTION__);
			return ERROR_FALSE;
		}
	
		ret = IMP_ADEC_GetStream(AdChn, &stream_out, BLOCK);
		if (ret != 0) {
			DEBUG_IMPORTANT_INFO("%s  %04d  %s  imp audio decoder get stream failed\n", __FILE__, __LINE__, __FUNCTION__);
			return ERROR_FALSE;
		}
		frm.virAddr = (unsigned int *)stream_out.stream;
		frm.len = stream_out.len;
//		DEBUG_INFO("src len = %d, decoder len = %d", len, stream_out.len);
	} else {
		frm.virAddr = (unsigned int *)buffer;
		frm.len = len;
	}
	
	ret = IMP_AO_SendFrame(aoCell.deviceID, aoCell.chnID, &frm, BLOCK);
	if (ret != 0) {
		DEBUG_IMPORTANT_INFO("%s  %04d  %s  send Frame Data error\n", __FILE__, __LINE__, __FUNCTION__);
		return ERROR_FALSE;
	}

	if (audioDecName != MW_PT_PCM) {
		ret = IMP_ADEC_ReleaseStream(AdChn, &stream_out);
		if (ret != 0) {
			DEBUG_IMPORTANT_INFO("%s  %04d  %s  imp audio decoder release stream failed\n", __FILE__, __LINE__, __FUNCTION__);
			return ERROR_FALSE;
		}
	}
/*	
	ret = IMP_AO_QueryChnStat(aoCell.deviceID, aoCell.chnID, &play_status);
	if (ret != 0) {
		DEBUG_INFO("---daemon---%15s,%5d,%15s---%s  %04d  %s  IMP_AO_QueryChnStat error\n",__FILE__, __LINE__, __func__,  __FILE__, __LINE__, __FUNCTION__);
		return ERROR_FALSE;
	}
*/
	return ERROR_SUCCESS;
}

/*
**函数原型:void play_music_frame(char *buffer, int len)
**输入参数:buffer--point to the buffer where store the audio_data will be played, len--the length of buffer
**返回值  :none
**函数功能:play one frame audio data----(unsigned int *)buffer
*/
int play_music_buffer(char *buffer, int len)
{
	int ret = -1;
	IMPAudioFrame frm;
	frm.virAddr = (unsigned int *)buffer;
	frm.len = len;

	ret = IMP_AO_SendFrame(aoCell.deviceID, aoCell.chnID, &frm, BLOCK);
	if(ret != 0) {
		DEBUG_ERR("---daemon---%15s,%5d,%15s---%s  %04d  send Frame Data error\n",__FILE__, __LINE__, __func__,  __FUNCTION__, __LINE__);
		return ERROR_FALSE;
	}

	return ERROR_SUCCESS;
}

static void print_init_ai_error(void)
{
	switch(initAudioRecordErr)
	{
		case INIT_AI_ERR_SUCESS:
			DEBUG_IMPORTANT_INFO("init ai success\n");
			break;
		case INIT_AI_ERR_SET_ATTR:
			DEBUG_IMPORTANT_INFO("IMP_AI_SetPubAttr error\n");
			break;
		case INIT_AI_ERR_ENABLE_AI:
			DEBUG_IMPORTANT_INFO("IMP_AI_Enable  error\n");
			break;
		case INIT_AI_ERR_SET_CHNPAR:
			DEBUG_IMPORTANT_INFO("IMP_AI_SetChnParam  error\n");
			break;
		case INIT_AI_ERR_ENABLE_CHN:
			DEBUG_IMPORTANT_INFO("IMP_AI_EnableChn  error\n");
			break;
		case INIT_AI_ERR_SET_VOL:
			DEBUG_IMPORTANT_INFO("IMP_AI_SetVol  error\n");
			break;
		case INIT_AI_ERR_ENABLE_HPF:
			DEBUG_IMPORTANT_INFO("IMP_AI_EnableHpf  error\n");
			break;
		case INIT_AI_ERR_ENABLE_NS:
			DEBUG_IMPORTANT_INFO("IMP_AI_EnableNs  error\n");
			break;
		case INIT_AI_ERR_REG_ENC:
			DEBUG_IMPORTANT_INFO("IMP_AENC_RegisterEncoder  error\n");
			break;
		case INIT_AI_ERR_CREA_ENC:
			DEBUG_IMPORTANT_INFO("IMP_AENC_CreateChn  error\n");
			break;
	}
}

static void print_init_ao_error(void)
{
	switch(initAudioPlayErr)
	{
		case INIT_AO_ERR_SUCESS:
			DEBUG_IMPORTANT_INFO("init ao success\n");
			break;
		case INIT_AO_ERR_SET_ATTR:
			DEBUG_IMPORTANT_INFO("IMP_AO_SetPubAttr  error\n");
			break;
		case INIT_AO_ERR_ENABLE_AO:
			DEBUG_IMPORTANT_INFO("IMP_AO_Enable  error\n");
			break;
		case INIT_AO_ERR_ENABLE_CHN:
			DEBUG_IMPORTANT_INFO("IMP_AO_EnableChn  error\n");
			break;
		case INIT_AO_ERR_SET_VOL:
			DEBUG_IMPORTANT_INFO("IMP_AO_SetVol  error\n");
			break;
		case INIT_AO_ERR_ENABLE_HPF:
			DEBUG_IMPORTANT_INFO("IMP_AO_EnableHpf  error\n");
			break;
		case INIT_AO_ERR_REG_DEC:
			DEBUG_IMPORTANT_INFO("IMP_ADEC_RegisterDecoder  error\n");
			break;
		case INIT_AO_ERR_CREA_DEC:
			DEBUG_IMPORTANT_INFO("IMP_ADEC_CreateChn  error\n");
			break;
	}
}

/*
**change audio_param format from user format(int) to the forma that IMP interface handfile gived(enum)
*/
static int audio_param_to_imp(IMPAudioIOAttr *attr, int sampleRate, int channelMode, int sampleBit)
{
	int ret = ERROR_SUCCESS;
	
	switch(sampleRate)
	{
		case 8000:/**< 8KHz采样率 */
			attr->samplerate = AUDIO_SAMPLE_RATE_8000;
			break;
		case 16000:/**< 16KHz采样率 */
			attr->samplerate = AUDIO_SAMPLE_RATE_16000;
			break;
		case 44100:/**< 44.1KHz采样率 */
			attr->samplerate = AUDIO_SAMPLE_RATE_44100;
			break;
		case 48000:/**< 48KHz采样率 */
			attr->samplerate = AUDIO_SAMPLE_RATE_48000;
			break;
		case 96000:/**< 96KHz采样率 */
			attr->samplerate = AUDIO_SAMPLE_RATE_96000;
			break;
		default:
			ret = ERR_SUPPORT_SAMPLERATE;
			break;
	}
	
	switch(sampleBit)
	{
		case 16:
			attr->bitwidth = AUDIO_BIT_WIDTH_16;
			break;
		default:
			ret = ERR_SUPPORT_SAMPLEBIT;
			break;
	}

	switch(channelMode)
	{
		case 1:
			attr->soundmode= AUDIO_SOUND_MODE_MONO;
			break;
		case 2:
			attr->soundmode = AUDIO_SOUND_MODE_STEREO;
			break;
		default:
			ret = ERR_SUPPORT_SAMPLECHN;
			break;
	}

	return ret;	
}

int audio_adc_param_get(int *sampleRate, int *channelMode, int *sampleBit)
{
	IMPAudioIOAttr attr;
	int ret = -1;

	ret = IMP_AI_GetPubAttr(aiCell.deviceID, &attr);
	if(ret != 0) {
		DEBUG_IMPORTANT_INFO("file:%s line:%04d get ao %d attr err: %d\n", __FILE__, __LINE__, aiCell.deviceID, ret);
		return ERROR_FALSE;
	}

	*sampleRate = (int)attr.samplerate;
	*channelMode = (int)attr.soundmode;
	*sampleBit = (int)attr.bitwidth;

	return ERROR_SUCCESS;
}

int audio_dac_param_get(int *sampleRate, int *channelMode, int *sampleBit)
{
	IMPAudioIOAttr attr;
	int ret = -1;

	ret = IMP_AO_GetPubAttr(aoCell.deviceID, &attr);
	if(ret != 0) {
		DEBUG_IMPORTANT_INFO("file:%s line:%04d get ao %d attr err: %d\n", __FILE__, __LINE__, aoCell.deviceID, ret);
		return ERROR_FALSE;
	}

	*sampleRate = (int)attr.samplerate;
	*channelMode = (int)attr.soundmode;
	*sampleBit = (int)attr.bitwidth;

	return ERROR_SUCCESS;
}

int audio_adc_param_set(int sampleRate, int channelMode, int sampleBit)
{
	int ret = -1;

	IMPAudioSampleRate localSampleRate = aiAttr.samplerate;
	IMPAudioSoundMode localChannelMode = aiAttr.soundmode;
	IMPAudioBitWidth localSampleBit = aiAttr.bitwidth;


	
	ret = audio_param_to_imp(&aiAttr, sampleRate, channelMode, sampleBit);
	if(ret != ERROR_SUCCESS)
	{
		goto ERR_HANDLE;
	}

	ret = IMP_AI_SetPubAttr(aiCell.deviceID, &aiAttr);
	if(ret != 0) {
		goto ERR_HANDLE;
	}

	return ret;
	
/*if failed . recover the param*/
ERR_HANDLE:
	aiAttr.samplerate = localSampleRate;
	aiAttr.soundmode  = localChannelMode;
	aiAttr.bitwidth   = localSampleBit;
	DEBUG_IMPORTANT_INFO("file:%s line:%04d Set ao %d attr err: %d\n", __FILE__, __LINE__, aiCell.deviceID, ret);
	return ret;		
}

int audio_dac_param_set(int sampleRate, int channelMode, int sampleBit)
{
	int ret = -1;

	printf(" ========================= %d %d %d\n", aoAttr.samplerate, aoAttr.soundmode, aoAttr.bitwidth);

	IMPAudioSampleRate localSampleRate = aoAttr.samplerate;
	IMPAudioSoundMode localChannelMode = aoAttr.soundmode;
	IMPAudioBitWidth localSampleBit = aoAttr.bitwidth;
	
	ret = audio_param_to_imp(&aoAttr, sampleRate, channelMode, sampleBit);
	if(ret != ERROR_SUCCESS)
	{
		goto ERR_HANDLE;
	}

	ret = IMP_AO_SetPubAttr(aoCell.deviceID, &aoAttr);
	if(ret != 0) {
		goto ERR_HANDLE;
	}
		
	return ret;

/*if failed . recover the param*/
ERR_HANDLE: 
	aoAttr.samplerate = localSampleRate;
	aoAttr.soundmode  = localChannelMode;
	aoAttr.bitwidth   = localSampleBit;
	DEBUG_IMPORTANT_INFO("file:%s line:%04d set ao %d attr err: %d\n", __FILE__, __LINE__, aoCell.deviceID, ret);
	
	return ret;	
}

int audio_adc_volume_set(int value)
{
	int ret = -1;
	
	aivol = value;
	
	ret = IMP_AI_SetVol(aiCell.deviceID, aiCell.chnID, aivol);
	if(ret != 0) {
		DEBUG_IMPORTANT_INFO("file:%s  line:%04d  Audio Record set volume failed\n", __FILE__, __LINE__);
		return ERROR_FALSE;
	}
	
	return ERROR_SUCCESS;
}

int audio_adc_volume_get(int *value)
{
	int ret = -1;
	int aiVol;
		
	ret = IMP_AI_GetVol(aiCell.deviceID, aiCell.chnID, &aiVol);
	if(ret != 0) {
		DEBUG_IMPORTANT_INFO("file:%s  line:%04d  Audio Record get volume failed\n", __FILE__, __LINE__);
		return ERROR_FALSE;
	}

	*value = aiVol;
	
	return ERROR_SUCCESS;
}

int audio_dac_volume_set(int value)
{
	int ret = -1;
	
	aovol = value;
	
	ret = IMP_AO_SetVol(aoCell.deviceID, aoCell.chnID, aovol);
	if(ret != 0) {
		DEBUG_IMPORTANT_INFO("file:%s  line:%04d  Audio Play set volume failed\n", __FILE__, __LINE__);
		return ERROR_FALSE;
	}
	
	return ERROR_SUCCESS;
}

int audio_dac_volume_get(int *value)
{
	int ret = -1;
	int aoVol = 0;
		
	ret = IMP_AO_GetVol(aoCell.deviceID, aoCell.chnID, &aoVol);
	if(ret != 0) {
		DEBUG_IMPORTANT_INFO("Audio Play get volume failed\n");
		return ERROR_FALSE;
	}

	*value = aoVol;
	
	return ERROR_SUCCESS;
}


/*
**函数原型:int mw_audio_record_init(void)
**输入参数:none
**返回值  :0 success;
**函数功能:init audio module
*/
int audio_record_init(void)
{
	int ret = -1;
//	int aigain = 30;	
	IMPAudioIChnParam chnParam;
	chnParam.usrFrmDepth = 2;//20;

//	speex_preprocess_init();

	DEBUG_IMPORTANT_INFO("aiAttr.samplerate = %d, aiAttr.soundmode = %d, aiAttr.bitwidth = %d\n", aiAttr.samplerate, aiAttr.soundmode, aiAttr.bitwidth);
	DEBUG_IMPORTANT_INFO("aiAttr.frmNum = %d, aiAttr.numPerFrm = %d, aiAttr.chnCnt = %d\n", aiAttr.frmNum, aiAttr.numPerFrm, aiAttr.chnCnt);
	ret = IMP_AI_SetPubAttr(aiCell.deviceID, &aiAttr);
	if(ret != 0) {
		initAudioRecordErr = INIT_AI_ERR_SET_ATTR;
		goto ERR_HANDLING;
	}
/*	
	IMPAudioIOAttr attr;
	memset(&attr, 0x0, sizeof(attr));
	ret = IMP_AI_GetPubAttr(aiCell.deviceID, &attr);
	if (ret != 0) {
		goto ERR_HANDLING;
	}
	DEBUG_INFO("---daemon---%15s,%5d,%15s---attr.samplerate = %d, attr.soundmode = %d, attr.bitwidth = %d\n",__FILE__, __LINE__, __func__,  attr.samplerate, attr.soundmode, attr.bitwidth);
	DEBUG_INFO("---daemon---%15s,%5d,%15s---attr.frmNum = %d, attr.numPerFrm = %d, attr.chnCnt = %d\n",__FILE__, __LINE__, __func__,  attr.frmNum, attr.numPerFrm, attr.chnCnt);
*/	
	ret = IMP_AI_Enable(aiCell.deviceID);
	if (ret != 0) {
		initAudioRecordErr = INIT_AI_ERR_ENABLE_AI;
		goto ERR_HANDLING;
	}

	ret = IMP_AI_SetChnParam(aiCell.deviceID, aiCell.chnID, &chnParam);
	if (ret != 0) {
		initAudioRecordErr = INIT_AI_ERR_SET_CHNPAR;
		goto ERR_HANDLING;
	}
	
	ret = IMP_AI_EnableChn(aiCell.deviceID, aiCell.chnID);
	if (ret != 0) {
		initAudioRecordErr = INIT_AI_ERR_ENABLE_CHN;
		DEBUG_ERR("---main---%s,%d,%s---IMP_AI_EnableChn error.", __FILE__, __LINE__, __func__); 			
		goto ERR_HANDLING;
	}

	ret = IMP_AI_SetVol(aiCell.deviceID, aiCell.chnID, aivol);
	if (ret != 0) {
		initAudioRecordErr = INIT_AI_ERR_SET_VOL;
		DEBUG_ERR("---main---%s,%d,%s---IMP_AI_SetVol error.", __FILE__, __LINE__, __func__); 			
		goto ERR_HANDLING;
	}
/*
	ret = IMP_AI_SetGain(aiCell.deviceID, aiCell.chnID, 15); 
	if(ret != 0) {		
		initAudioRecordErr = INIT_AI_ERR_ENABLE_HPF;
		DEBUG_ERR("---main---%s,%d,%s---IMP_AI_SetGain error.", __FILE__, __LINE__, __func__); 			
		goto ERR_HANDLING;
	}
*/
	IMP_AI_DisableAec(aiCell.deviceID, aiCell.chnID);
	
	ret = IMP_AI_EnableNs(&aiAttr, NS_VERYHIGH);	
	if(ret != 0) {		
		DEBUG_ERR("---main---%s,%d,%s---IMP_AI_EnableNs error.", __FILE__, __LINE__, __func__); 	
		initAudioRecordErr = INIT_AI_ERR_ENABLE_HPF;
		goto ERR_HANDLING;
	}
	

	ret = IMP_AI_EnableHpf(&aiAttr);
	if (ret != 0) {
		initAudioRecordErr = INIT_AI_ERR_ENABLE_HPF;
		goto ERR_HANDLING;
	}

	IMPAudioAgcConfig agcConfig = { 	
		.TargetLevelDbfs = 0,		
		.CompressionGaindB = 5
	};	//启用音频输入的自动增益功能.
	
	ret = IMP_AI_EnableAgc(&aiAttr, agcConfig);	
	if(ret != 0) {		
		DEBUG_ERR("---main---%s,%d,%s---IMP_AI_EnableAgc error.", __FILE__, __LINE__, __func__); 	
		initAudioRecordErr = INIT_AI_ERR_ENABLE_HPF;
		goto ERR_HANDLING;
	}


//	IMP_AI_DisableAec(aiCell.deviceID, aiCell.chnID);
//	IMP_AI_DisableHpf();
/*	
	ret = IMP_AI_EnableNs(&aiAttr, NS_VERYHIGH); //NS_MODERATE	
//	ret = IMP_AI_EnableNs(&aiAttr, NS_MODERATE); //
	if (ret != 0) {
		initAudioRecordErr = INIT_AI_ERR_ENABLE_NS;
		goto ERR_HANDLING;
	}
*/

	if (audioEncName == MW_PT_PCM) {
		initAudioRecordErr = INIT_AI_ERR_ENABLE_NS;		
	} else {
		ret = init_audio_encoder(audioEncName);
		if (ret != 0){
			goto ERR_HANDLING;
		} else {
			initAudioRecordErr = INIT_AI_ERR_SUCESS;		
		}
	}
			
	return ERROR_SUCCESS;

ERR_HANDLING:
	print_init_ai_error();
	audio_record_uninit();
	
	return ERROR_FALSE;		
}


/*
**函数原型:int mw_audio_play_init(void)
**输入参数:none
**返回值  :0 success;
**函数功能:init audio module
*/
int audio_play_init(void) 
{
	int ret   = -1;
	
	ret = IMP_AO_SetPubAttr(aoCell.deviceID, &aoAttr);
	if (ret != 0) {
		initAudioPlayErr = INIT_AO_ERR_SET_ATTR;
		goto ERR_HANDLING;
	}

	ret = IMP_AO_Enable(aoCell.deviceID);
	if (ret != 0) {
		initAudioPlayErr = INIT_AO_ERR_ENABLE_AO;
		goto ERR_HANDLING;
	}

	ret = IMP_AO_EnableChn(aoCell.deviceID, aoCell.chnID);
	if (ret != 0) {
		initAudioPlayErr = INIT_AO_ERR_ENABLE_CHN;
		goto ERR_HANDLING;
	}

	ret = IMP_AO_SetVol(aoCell.deviceID, aoCell.chnID, aovol);
	if (ret != 0) {
		initAudioPlayErr = INIT_AO_ERR_SET_VOL;
		goto ERR_HANDLING;
	}
	
	ret = IMP_AO_EnableHpf(&aoAttr);
	if (ret != 0) {
		initAudioPlayErr = INIT_AO_ERR_ENABLE_HPF;
		goto ERR_HANDLING;
	}

	if (audioDecName != MW_PT_PCM) {
		ret = init_audio_decoder(audioDecName);
			if (ret != 0) {
				goto ERR_HANDLING;
		}
	}

	initAudioPlayErr = INIT_AO_ERR_SUCESS;

	return ERROR_SUCCESS;

ERR_HANDLING:
	print_init_ao_error();
	audio_play_uninit();
	
	return ERROR_FALSE;	
}



int audio_record_uninit(void)
{
	int ret = -1;
	int aeChn = 0;
	
	switch(initAudioRecordErr)
	{
		case INIT_AI_ERR_SUCESS:
			ret = IMP_AENC_DestroyChn(aeChn);
			if(ret != 0) {
				return ERROR_FALSE;
			}
			else{
				initAudioRecordErr = INIT_AI_ERR_CREA_ENC;
			}
			
		case INIT_AI_ERR_CREA_ENC  :
			if(audioEncName!=MW_PT_PCM && audioEncName!=MW_PT_G711A && audioEncName!=MW_PT_G711U && audioEncName!=MW_PT_G726  && audioEncName!=MW_PT_AEC && audioEncName!=MW_PT_ADPCM && audioEncName!=MW_PT_MAX) {
				ret = IMP_AENC_UnRegisterEncoder(&handleEncoder);
				if(ret != 0) {
					return ERROR_FALSE;
				}
				else{
					initAudioRecordErr = INIT_AI_ERR_REG_ENC;
				}
			}
			
		case INIT_AI_ERR_REG_ENC   :
		case INIT_AI_ERR_ENABLE_NS :
		case INIT_AI_ERR_ENABLE_HPF:
		case INIT_AI_ERR_SET_VOL   :
			ret = IMP_AI_DisableChn(aiCell.deviceID, aiCell.chnID);
			if(ret != 0) {
				return ERROR_FALSE;
			}
			else{
				initAudioRecordErr = INIT_AI_ERR_ENABLE_CHN;
			}
			
		case INIT_AI_ERR_ENABLE_CHN:
		case INIT_AI_ERR_SET_CHNPAR:
			ret = IMP_AI_Disable(aiCell.deviceID);
			if(ret != 0) {
				return ERROR_FALSE;
			}
			else{
				initAudioRecordErr = INIT_AI_ERR_ENABLE_AI;
			}
			
		case INIT_AI_ERR_ENABLE_AI :
		case INIT_AI_ERR_SET_ATTR  :
			return ERROR_SUCCESS;
			
		default:
			return ERROR_FALSE;
			
	}
}

int audio_play_uninit(void)
{
	int ret = -1;
	int adChn = 0;
	
	switch(initAudioPlayErr)
	{
		case INIT_AO_ERR_SUCESS:
			ret = IMP_ADEC_DestroyChn(adChn);
			if(ret != 0) {
				return ERROR_FALSE;
			}
			else{
				initAudioPlayErr = INIT_AO_ERR_CREA_DEC;
			}
			
		case INIT_AO_ERR_CREA_DEC:
			if(audioDecName!=MW_PT_PCM && audioDecName!=MW_PT_G711A && audioDecName!=MW_PT_G711U && audioDecName!=MW_PT_G726 && audioEncName!=MW_PT_AEC && audioEncName!=MW_PT_ADPCM && audioEncName!=MW_PT_MAX) {
				//IMP_ADEC_ReleaseDecoder
				//IMP_ADEC_UnRegeisterDecoder
				ret = IMP_ADEC_UnRegisterDecoder(&handleDecoder);
				if(ret != 0) {
					return ERROR_FALSE;
				}
				else{
					initAudioPlayErr = INIT_AO_ERR_REG_DEC;
				}
			}
			
		case INIT_AO_ERR_REG_DEC:
		case INIT_AO_ERR_ENABLE_HPF:
		case INIT_AO_ERR_SET_VOL:
			ret = IMP_AO_DisableChn(aoCell.deviceID, aoCell.chnID);
			if(ret != 0) {
				return ERROR_FALSE;
			}
			else{
				initAudioPlayErr = INIT_AO_ERR_ENABLE_CHN;
			}
			
		case INIT_AO_ERR_ENABLE_CHN:
			ret = IMP_AO_Disable(aoCell.deviceID);
			if(ret != 0) {
				return ERROR_FALSE;
			}
			else{
				initAudioPlayErr = INIT_AO_ERR_ENABLE_AO;
			}
			
		case INIT_AO_ERR_ENABLE_AO:
		case INIT_AO_ERR_SET_ATTR:
			return ERROR_SUCCESS;
			
		default:
			return ERROR_FALSE;
	}
}


