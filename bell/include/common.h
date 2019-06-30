/**
 * @file		common.h
 * @version		1.0.0
 * @date		2016-11-09
 * @author		lzg
 * @brief
 *		this head file provide information all modules would use(include up-application)
 * @warning
 * @copyright
 *		GAOGUZHILIAN
 */
 
/*
 * @defgroup common COMMON
 */
 
/**
 * @defgroup common COMMON
 * @brief 该文件定义了所有模块使用到用来标识返回的宏
 * @{
 */
 
#ifndef __COMMON_H__
#define __COMMON_H__

#include <pthread.h>
#include <stdio.h>

#define TRUE  1                         ///< 真
#define FALSE 0							///< 假	

#define HIGH_LEVEL 		1				///< ????
#define LOW_LEVEL			0			///< ????
#define ACTIVE_LEVEL	1				///< ??Ч???
#define INACTIVE_LEVEL	0				///< ??Ч???

#define ERROR_FALSE     -1				///< 失败
#define ERROR_SUCCESS   0				///< 成功
#define ERROR_EXIST     1				///< 已经存在

#define ERR_BUF_INSUFFICIENT		2	///< the buffer is not enough, need a bigger buffer----提供buffer空间不足，请求更大的buffer	
#define ERR_EMPTY_POINTER			3	///< empty pointer----空指针
#define ERR_OPEN_FILE				4   ///< open file error---打开文件失败
#define ERR_PARAMETER 				5	///< function parameter error(maybe empty pointer)---函数参数错误
#define ERR_BUFFER_FULL				6	///< the buffer is full, can not push new data----缓存满， 不能压入新的数据
#define ERR_SUPPORT_SAMPLERATE		10	///< not support this sample rate----不支持该采样率
#define ERR_SUPPORT_SAMPLEBIT		11	///< not support this sample bit----不支持该采样精度
#define ERR_SUPPORT_SAMPLECHN		12	///< not support this sample channel-----不支持该声道数
#define ERR_SUPPORT_VOL				13  ///< not support this volume----不支持该设置音量	    
#define ERR_FILENAME   				14	///< file name too long or is NULL----文件名太长或者文件名为空
#define ERR_PLAYTIME   				15	///< play time errors----设置播放次数错误
#define ERR_NO_NEW_DATA				16  ///< there have no new data to get----没有新的数据获取
#define ERR_NOT_SUPPORT_AP			17  ///< this function not support Ap----该功能不支持AP模式
#define ERR_SET_IP					18	///< set ip address error-----设置IP地址错误
#define ERR_GET_IP					19	///< get ip address error-----获取IP地址错误
#define ERR_SET_NETMASK				20	///< set net mask error----设置子网掩码错误
#define ERR_GET_NETMASK				21	///< get net mask error----设置子网掩码错误
#define ERR_SET_GATEWAY				22	///< set gateway error----设置网关错误
#define ERR_GET_GATEWAY             23 	///< get gateway error----获取网关错误
#define ERR_SET_DNS					24	///< set dns error----设置DNS错误
#define ERR_GET_DNS					25	///< get dns error----获取DNS错误
#define ERR_CREATE_SOCKET			26  ///< create socket failed
#define ERR_SOCKET_BIND				27  ///< socket bind error
#define ERR_SOCKER_SEND				28  ///< socket send error
#define ERR_SET_SOCKET_OPT			29	///< set socket opt failed
#define ERR_P2P_CHECK_BUF			29	///< ppcs_check_buffer
#define ERR_P2P_READ_BUF			30	///< ppcs_read
#define ERR_P2P_HEAD				31	///< head error
#define ERR_P2P_NO_DATA_READ		32	///< ppcs_check_buffer, and there is no data to be readed	
#define ERR_NAME_OR_PASSWD			33	///< err name or passwd
#define ERR_STATUS_NO_DEFINED 		34	///< ?????????δ?
#define ERR_GPIO_CONFIG_FORMAT		35	///< gpio config format error
#define ERR_EXCEED_MAX_CONFIG		36 ///< exceed the largest number of configurable
#define ERR_GPIO_REGIST				37	///< regist gpio failed
#define ERR_GPIO_INIT					38 ///< init gpio failed (contain direct, active level, origianl level)
#define ERR_FILE_FIND					39 ///< find the file
#define ERR_FILE_NOT_FIND				40 ///< not find the file							
#define ERR_UPDATE_PARAM				41	///< update paramter file failed
#define ERR_READ_FILE					42	///< read file error
#define ERR_WRITE_FILE					43	///< write file error
#define ERR_FORMAT						44	///< format error
#define ERR_IN_CUR_STATUS				45	///< is current status


//#define PRINT_TO_REMOTE

#define PRINT_IMPORTANT_INFO
#define PRINT_NORMAL_INFO
#define PRINT_ERR
#define PRINTFDEBUGTEST

//#define G827_V1_1
#define G827_V1_3
//#define YIDUIYI

void lock_print(void);
void unlock_print(void);
void DEBUG_ERR(const char *format, ...);
void DEBUG_INFO(const char *format, ...);
void DEBUG_TEST(const char *format, ...);
void DEBUG_IMPORTANT_INFO(const char *format, ...);
int strcmpEx( const char *str1, const char *str2 );
int my_system(char *cmd);
FILE *my_popen(const char *cmdstring, const char *type);
int my_pclose(FILE *fp);
void append_pthread_pid(char *pthreadName);

void HexToStr(char *pbDest, char *pbSrc, int nLen);


#ifdef YIDUIYI
#include "yiduiyi.h"
#else
#include "g827.h"
#endif

/*
struct media_v *mw_video = NULL;
struct media_a *mw_audio = NULL;
struct event *mw_event = NULL;
struct net_info *mw_net = NULL;
struct i2c_dev *i2c = NULL;
struct uart_dev *uart = NULL;
struct pm_fun *pm = NULL;
*/
/**
 * @}
 */
#endif 		//__COMMON_H__
