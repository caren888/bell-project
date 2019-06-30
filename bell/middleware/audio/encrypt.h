
#ifndef __ENCRYPT_H__
#define __ENCRTPT_H__

#include <imp/imp_audio.h>
/**
 * @file 		encrypt.h
 * @version		1.0.0
 * @date		2016-11-09
 * @author		lzg

 * @copyright
 *		GAOGUZHILIAN
 */

/**
 * @defgroup audio_enc_dec
 * @ingroup middle_developer
 * @brief
 *		this head file provides some interfaces about encryption and decryption
 * @details
 *		this head file provides these interfaces below:
 *		1. G711U encoder and decoder interface to encode and decode buffer
 *		2. G711A encoder and decoder interface to encode and decode buffer
 *		3. 5350  encoder and decoder interface to encode and decode buffer
 * @{
 */

/**
 * @fn int MY_G711U_Encode_Frm(void *encoder, IMPAudioFrame *data, unsigned char *outbuf,int *outLen)
 * @brief 采用G711U加密数据
 * @param[in] encoder 
 * @param[in] data 需要加密的数据
 * @param[out] outbuf 输出的加密数据
 * @param[out] outLen 输出数据长度
 * @retval 0 成功
 * @retval -1 失败，其他错误请参看common.h
 * @attention
 */
int MY_G711U_Encode_Frm(void *encoder, IMPAudioFrame *data, unsigned char *outbuf,int *outLen);


/**
 * @fn int MY_G711A_Encode_Frm(void *encoder, IMPAudioFrame *data, unsigned char *outbuf,int *outLen)
 * @brief 采用G711A加密数据
 * @param[in] encoder ？
 * @param[in] data 需要加密的数据
 * @param[out] outbuf 输出的加密数据
 * @param[out] outLen 输出数据长度
 * @retval 0 成功
 * @retval -1 失败，其他错误请参看common.h
 * @attention
 */
int MY_G711A_Encode_Frm(void *encoder, IMPAudioFrame *data, unsigned char *outbuf,int *outLen);

/**
 * @fn int MY_G711A_Decode_Frm(void *decoder, unsigned char *inbuf,int inLen, unsigned short *outbuf,int *outLen,int *chns)
 * @brief 采用G711A解密数据
 * @param[in] decoder 
 * @param[in] inbuf 需要解密的数据
 * @param[in] inLen 解密数据长度
 * @param[out] outbuf 输出的解密数据
 * @param[out] outLen 输出数据长度
 * @param[in] chns ？
 * @retval 0 成功
 * @retval -1 失败，其他错误请参看common.h
 * @attention
 */
int MY_G711A_Decode_Frm(void *decoder, unsigned char *inbuf,int inLen, unsigned short *outbuf,int *outLen,int *chns);

/**
 * @fn int MY_G711U_Decode_Frm(void *decoder, unsigned char *inbuf,int inLen, unsigned short *outbuf,int *outLen,int *chns)
 * @brief 采用G711U解密数据
 * @param[in] decoder 
 * @param[in] inbuf 需要解密的数据
 * @param[in] inLen 解密数据长度
 * @param[out] outbuf 输出的解密数据
 * @param[out] outLen 输出数据长度
 * @param[in] chns ？
 * @retval 0 成功
 * @retval -1 失败，其他错误请参看common.h
 * @attention
 */
int MY_G711U_Decode_Frm(void *decoder, unsigned char *inbuf,int inLen, unsigned short *outbuf,int *outLen,int *chns);

/**
 * @fn int GetEncoderAduioPreSample(void)
 * @brief 获取\b sample
 * @param void
 * @retval 0 成功
 * @retval -1 失败，其他错误请参看common.h
 * @attention 该函数和\ref GetEncoderAudioIndex 以及 \ref EncoderClr 密切相关，属于5350加解密参数
 */
int GetEncoderAduioPreSample(void);

/**
 * @fn int GetEncoderAudioIndex(void)
 * @brief 获取\b index
 * @param void
 * @retval 0 成功
 * @retval -1 失败，其他错误请参看common.h
 * @attention 该函数和\ref GetEncoderAduioPreSample 以及 \ref EncoderClr 密切相关，属于5350加解密参数
 */
int GetEncoderAudioIndex(void);

/**
 * @fn void EncoderClr(int sample,int index)
 * @brief 设置\b sample 以及 \b index
 * @param[in] sample
 * @param[in] index
 * @return void
 * @attention 该函数和\ref GetEncoderAudioIndex 以及 \ref GetEncoderAduioPreSample 密切相关，属于5350加解密参数
 */
void EncoderClr(int sample,int index);

/**
 * @fn int MY_5350_Encode_Frm(void *encoder, IMPAudioFrame *data, unsigned char *outbuf,int *outLen)
 * @brief 采用5350加密数据
 * @param[in] encoder 
 * @param[in] data 需要加密的数据
 * @param[out] outbuf 输出的加密数据
 * @param[out] outLen 输出数据长度
 * @retval 0 成功
 * @retval -1 失败，其他错误请参看common.h
 * @attention
 */
int MY_5350_Encode_Frm(void *encoder, IMPAudioFrame *data, unsigned char *outbuf,int *outLen);

/**
 * @fn int MY_5350_Decode_Frm(void *decoder, unsigned char *inbuf,int inLen, unsigned short *outbuf,int *outLen,int *chns)
 * @brief 采用5350解密数据
 * @param[in] decoder 
 * @param[in] inbuf 需要解密的数据
 * @param[in] inLen 解密数据长度
 * @param[out] outbuf 输出的解密数据
 * @param[out] outLen 输出数据长度
 * @param[in] chns ？
 * @retval 0 成功
 * @retval -1 失败，其他错误请参看common.h
 * @attention
 */
int MY_5350_Decode_Frm(void *decoder, unsigned char *inbuf,int inLen, unsigned short *outbuf,int *outLen,int *chns);

/**
 * @}
 */
#endif
