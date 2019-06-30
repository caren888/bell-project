/**
 * @file audio.h
 * @version 1.0.0
 * @date 2016-11-10
 * @author lzg
 * @copyright GAOGUZHILIAN 
 */

/**
 * @defgroup middle_developer Middle Developer
 */

/**
 * @defgroup audio
 * @ingroup middle_developer
 * @brief
 *		this head file is file provide interface to control audio module
 * @section concept 相关概念
 * audio主要为mw_audio中间件接口提供服务，对上次用户不提供接口，提供的接口如下\n
 * -# init the audio module include play and record
 * -# uninit the audio module include play and record
 * -# set and get adc(record) volume
 * -# set and get dac(play) volume
 * -# set and get adc(record) parameter include sample rate, channel mode and sample bit
 * -# set and get dac(play) parameter include sample rate, channel mode and sample bit
 * -# play audio buffer(encoded buffer)
 * -# play music buffer
 * -# get record audio buffer
 * @warning
 * @{
 */

#ifndef __AUDIO_H__
#define __AUDIO_H__

/**
 * @fn int audio_play_init(void)
 * @brief  init audio play module
 * @param void
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 *	if not use audio_dac_param_set and audio_dac_param_set, the play module\n
 *	will use default configure	
 * @attention
 */
int audio_play_init(void) ;


/**
 * @fn int audio_play_uninit(void)
 * @brief  uninit audio play module
 * @param void
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 * @attention
 */
int audio_play_uninit(void);


/**
 * @fn int audio_record_init(void)
 * @brief  init audio record module
 * @param
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 * @attention
 */
int audio_record_init(void);


/**
 * @fn int audio_record_uninit(void)
 * @brief  uninit audio record module
 * @param
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 * @attention
 */
int audio_record_uninit(void);


/**
 * @fn int audio_dac_volume_get(int *value)
 * @brief  get the volume of audio play module 
 * @param[out]  value store the value of volume
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 * @attention
 *		before using this function, must init audio play module(int audio_play_init())
 */
int audio_dac_volume_get(int *value);


/**
 * @fn int audio_dac_volume_set(int value)
 * @brief  get the volume of audio play module 
 * @param[int]  value the volume value
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 * @attention
 *		before using this function, must init audio play module(int audio_play_init())
 */
int audio_dac_volume_set(int value);


/**
 * @fn int audio_adc_volume_get(int *value)
 * @brief  get the volume of audio play module 
 * @param[out] value store the volume
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 * @attention
 *		before using this function, must init audio record module(int audio_record_init())
 */
int audio_adc_volume_get(int *value);


/**
 * @fn int audio_adc_volume_set(int value)
 * @brief  set the volume of audio record module 
 * @param[in] value the volume value
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 * @attention
 *		before using this function, must init audio record module(int audio_record_init())
 */
int audio_adc_volume_set(int value);

/**
 * @fn int audio_dac_param_set(int sampleRate, int channelMode, int sampleBit)
 * @brief  set sample rate, channel mode and sample bit of audio play module 
 * @param[in] sampleRate
 * @param[in] channelMode
 * @param[in] sampleBit
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 * @attention
 *		before using this function, must init audio play module(int audio_play_init())
 */
int audio_dac_param_set(int sampleRate, int channelMode, int sampleBit);

/**
 * @fn int audio_adc_param_set(int sampleRate, int channelMode, int sampleBit)
 * @brief  set sample rate, channel mode and sample bit of audio record module 
 * @param[in] sampleRate
 * @param[in] channelMode
 * @param[in] sampleBit
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 * @attention
 *		before using this function, must init audio record module(int audio_record_init())
 */
int audio_adc_param_set(int sampleRate, int channelMode, int sampleBit);

/**
 * @fn int audio_dac_param_get(int *sampleRate, int *channelMode, int *sampleBit)
 * @brief  get sample rate, channel mode and sample bit of audio play module 
 * @param[out] sampleRate
 * @param[out] channelMode
 * @param[out] sampleBit
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 * @attention
 *		before using this function, must init audio play module(int audio_play_init())
 */
int audio_dac_param_get(int *sampleRate, int *channelMode, int *sampleBit);


/**
 * @fn int audio_adc_param_get(int *sampleRate, int *channelMode, int *sampleBit)
 * @brief  get sample rate, channel mode and sample bit of audio record module 
 * @param[out] sampleRate
 * @param[out] channelMode
 * @param[out] sampleBit
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 * @attention
 *		before using this function, must init audio record module(int audio_record_init())
 */
int audio_adc_param_get(int *sampleRate, int *channelMode, int *sampleBit);


/**
 * @fn int play_music_buffer(char *buffer, int len)
 * @param[in] buffer
 * @param[in] len
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 * @attention
 *		before using this function, must init audio play module(int audio_play_init())
 */
int play_music_buffer(char *buffer, int len);


/**
 * @fn int play_voice_buffer(char *buffer, int len)
 * @param[in] buffer
 * @param[in] len
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 *	the buffer is always encoded
 * @attention
 *		before using this function, must init audio play module(int audio_play_init())
 */
int play_voice_buffer(char *buffer, int len);


/**
 * @fn int play_voice_buffer(char *buffer, int len)
 * @param[in] buffer
 * @param[in][out] len
 * @retval 0  success
 * @retval -1 fail, other values please reference common.h
 * @remarks
 *	if the length of the buffer is not enough, this func will write the param len a suitable value
 * @attention
 *		before using this function, must init audio record module(int audio_play_init())
 */
int get_audio_frame(char *buffer, int *len);


int audio_adc_param_get(int *sampleRate, int *channelMode, int *sampleBit);
int audio_dac_param_get(int *sampleRate, int *channelMode, int *sampleBit);
int audio_adc_param_set(int sampleRate, int channelMode, int sampleBit);
int audio_dac_param_set(int sampleRate, int channelMode, int sampleBit);
int audio_adc_volume_get(int *value);
int audio_dac_volume_set(int value);
void clear_ao_chn(void);

/**
 * @}
 */

#endif
