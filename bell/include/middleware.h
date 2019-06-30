#ifndef __MIDDLEWARE_H__
#define __MIDDLEWARE_H__

/**
 * @file    middleware.h
 */

/*
 * @defgroup middle_ware_for_user USER_INTERFACE
 */

/**
 * @defgroup user_interface USER INTERFACE
 * @brief 这个文件为上层应用开发提供了一些接口，这些接口按照功能封装到了不同的功能结构体中。
 *
 * 应用开发可用接口包括：
 * -# 音频模块
 * -# 视频模块
 * -# 网络模块
 * -# 事件模块
 * -# I2C模块
 * -# UART模块
 * -# PM模块
 * 
 * 关于每个模块提供接口的具体信息，请仔细阅读每个模块结构体的说明信息
 * @attention 使用这些模块请使用模块内部的成员函数， 请不要使用成员变量，需要用到的成员变量\n
 * 都可以通过模块提供的接口函数进行操作
 * @{
 */

/* network module*/
/*****************************************************/
/**
 * @typedef NET_MODE
 * @brief 网络模式
*/
typedef enum _NET_MODE {  
	NET_MODE_AP = 1, 	///< AP模式
	NET_MODE_WIRE, 		///< wifi模式
	NET_MODE_WIRELESS, 	///< 有线模式
	NET_MODE_YIJIAN		///< 一键配置
}NET_MODE;

/**
 * @typedef _NET_CONN_STATUS
 * @brief 网络状态
 */
typedef enum _NET_CONN_STATUS {  
	NET_CONNING = 1,	///< 成长配置
	NET_UNCONN,			///< 断开
	NET_CONNED			///< 连接
}NET_CONN_STATUS;

/**
 *@struct net_para
 *@brief 网络参数绝构体
 */
struct net_para {
	char ip_addr[16]; ///< IP地址
	char net_gate[16];/// 网关
	char net_mask[16];///子网掩码
	char dns1[16];///DNS
	char dns2[16];///DNS
};

/**
 * @struct wifi_scan_res
 * @brief wifi信息结构体
 */
struct wifi_scan_res{
	char bssid[20]; ///< bssid，相当于硬件地址
	int freq; ///< 信道，频率
	int sig_level; ///< 信号强度
	char flags[128]; ///< 加密和安全模式
	char ssid[128];///< wifi名
};

/**
 * @struct net_info
 * @brief 网络模块功能结构体
 *
 * 该模块提供的操作有：
 * -# 网络模式的切换，AP, 有线和无线直接的切换，模块启动初始化时默认切换到有效模式
 * -# 网络模式的获取
 * -# 网络参数的设置，该设置只针对有线和无线模式有效，可设置DHCP，也可静态设置\n
 * 静态设置需要设置参数包括ip地址，网关，子网掩码，两个dns
 * -# 网络参数获取， 该操作只针对有线和无线模式有效
 * -# 获取网络连接状况，该操作只针对有效和无线模式有效，可同时获取外网和内网的联网状态
 * -# wifi设置，设置wifi名和密码，该操作只可在wifi模式下使用
 * -# wifi扫描，获取附件的wifi列表， 该操作只可在wifi模式下使用
 * -# wifi信号强度获取， 该操作只可在wifi模式下使用，而且只能获取到当前连接wifi的信号强度
 * -# ap设置， 设置ap的名称和密码
 * -# ap获取， 获取ap名称和密码
 * @example network_test.c
 */
struct net_info
{
	NET_MODE mode; /**< 模块当前所处的网络模式，供底层使用，应用层请不要直接操作 */
	NET_CONN_STATUS con_status; /**< 当前的网络状态， 供底层使用， 应用层请不要直接操作 */
	int wifi_sign; /**< wifi信号强度， 供底层使用， 应用层请不要直接操作*/
	int ready; /**< 模块准备情况，当ready=1表示模块已正常启动， 可正常操作模块， 供底层使用\n
	应用层请不要直接操作*/
	
	/**
	 * @fn int (*mode_set) (struct net_info *net, NET_MODE mode)
	 * @brief 设置当前的网络模式
	 * @param net [in] 必须是 \ref get_mw_network 返回的指针
	 * @param mode [in] 需要设置的网络模式
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请查看 \ref common.h
	 * @attention 该操作必须 \ref get_mw_network 操作成功才可使用
	 */
	int (*mode_set) (struct net_info *net, NET_MODE mode);
	
	/**
	 * @fn int (*mode_get) (struct net_info *net)
	 * @brief 获取当前的网络模式
	 * @param[in] net 必须是 \ref get_mw_network 返回的指针
	 * @return 当前的网络模式，1 AP， 2 wifi， 3 有线
	 * @attention 该操作必须 \ref get_mw_network 操作成功才可使用
	 */
	int (*mode_get) (struct net_info *net);
	
	/**
	 * @fn int (*set_mode_para) (struct net_info *net, NET_MODE mode, int dhcp_flag, struct net_para net_para)
	 * @brief 设置网络参数，只针对wifi和有线模式有效，可设置DHCP，已可静态设置，静态设置需要填充参数结构体 net_para
	 *
	 * DHCP设置，不需要填充参数结构体，DHCP可自动设置\n
	 * 静态设置，需要填充参数结构体，具体包括IP地址，子网掩码，网关，两个DNS
	 * @param net [in] 必须是 \ref get_mw_network 返回的指针
	 * @param mode [in] 当前的网络模式
	 * @param dhcp_flag [in] dhcp标志，1为dhc，0位静态设置
	 * @param net_para [in] 参数结构体
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须 \ref get_mw_network 操作成功才可使用
	 */
	int (*set_mode_para) (struct net_info *net, NET_MODE mode, int dhcp_flag, struct net_para net_para);

	/**
	 * @fn int (*get_mode_para) (struct net_info *net, NET_MODE mode, struct net_para *net_para)
	 * @brief 获取网络参数， 只针对wifi和有线模式有效
	 * @param net [in] 必须是 \ref get_mw_network 返回的指针
	 * @param mode [in] 当前网络模式
	 * @param net_para [out] 网络参数结构体指针
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h	 
	 * @attention 该操作必须 \ref get_mw_network 操作成功才可使用
	 */
	int (*get_mode_para) (struct net_info *net, NET_MODE mode, struct net_para *net_para);
	
	/**
	 * @fn int (*get_conn_sta) (struct net_info *net, int *intra_sta, int *inter_sta)
	 * @brief 获取当前的网络状态，只针对wifi和有线模式有效
	 * @param net [in]必须是 \ref get_mw_network 返回的指针
	 * @param intra_sta [out]返回内网的联网状态
	 * @param inter_sta [out]返回外网的联网状态
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须 \ref get_mw_network 操作成功才可使用
	 */
	int (*get_conn_sta) (struct net_info *net, int *intra_sta, int *inter_sta);
	
	/**
	 * @fn int (*wifi_set) (struct net_info *net, short *ssid, short *password, char *security)
	 * @brief 设置wifi名和密码，该操作只可在wifi模式下使用
	 * @param net [in]必须是 \ref get_mw_network 返回的指针
	 * @param ssid [in]指向wifi名的指针
	 * @param password [in] 指向wifi密码的指针
	 * @param security [in] 指向加密模式的指针
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须 \ref get_mw_network 操作成功才可使用
	 */
	int (*wifi_set) (struct net_info *net, short *ssid, short *password, char *security);	//utf16 for special character.
	
	/**
	 * @fn int (*wifi_scan) (struct net_info *net, struct wifi_scan_res *buf_list, int *count)
	 * @brief wifi扫描，获取附件的wifi列表，该操作只可在wifi模式下使用
	 * @param net [in] 必须是 \ref get_mw_network 返回的指针
	 * @param buf_list [out] 获取到的wifi列表结构体
	 * @param count [in][out]调用时需要设置想要获取的wifi列表的个数，如果函数成功，返回时程序将该值修正为真正获取到的个数
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @remark 建议不要超过32个
	 * @attention 该操作必须 \ref get_mw_network 操作成功才可使用
	 */
	int (*wifi_scan) (struct net_info *net, struct wifi_scan_res *buf_list, int *count);
	
	/**
	 * @fn int (*get_wifi_sig_lev) (struct net_info *net, int *sig_lev)
	 * @brief 获取wifi信号强度，该操作只可在wifi模式下使用
	 * @param net [in] 必须是 \ref get_mw_network 返回的指针
	 * @param sig_lev [out] 返回信号强度
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须 \ref get_mw_network 操作成功才可使用，该操作只可获取到当前连接wifi的信号强度
	 */
	int (*get_wifi_sig_lev) (struct net_info *net, int *sig_lev);
	
	/**
	 * @fn int (*ap_set) (struct net_info *net, char *ssid, char *passwd)
	 * @brief ap设置，设置ap名称和密码
	 * @param net [in] 必须是 \ref get_mw_network 返回的指针
	 * @param ssid [in] ap名称
	 * @param passwd [in] ap密码
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @remark 初始AP名称为RT2860AP，无密码
	 * @attention 该操作必须 \ref get_mw_network 操作成功才可使用
	 */
	int (*ap_set) (struct net_info *net, char *ssid, char *passwd);
	
	/**
	 * @fn int (*ap_get) (struct net_info *net, char *ssid, char *passwd)
	 * @brief 获取ap名称和密码
	 * @param net [in]必须是 \ref get_mw_network 返回的指针
	 * @param ssid [out] 指向存储获取到的 \a ssid ，长度根据实际情况，自行评估
	 * @param passwd [out] 指向错误获取到的 \a passwd ，长度根据实际情况，自己评估
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须 \ref get_mw_network 操作成功才可使用
	 */
	int (*ap_get) (struct net_info *net, char *ssid, char *passwd);
};

/**
 * @fn struct net_info *get_mw_network(void)
 * @brief 获取指向 net_info 的指针，应用层在使用网络模块接口之前，需要先调用该接口，且要成功才对网络模块进行操作
 * @param void
 * @return 指向 net_info 的指针，返回NULL表示失败
 * @attention 应用层调用该接口，意味着对网络模块进行初始化，底层会进行相关资源的申请，以及初始化内部函数指针等信息\n
 * 所有在对网络模块进行操作之前，必须先要调用该接口，且要返回成功
 */
struct net_info *get_mw_network(void);

/* media data module */
/*****************************************************/

/**
 * @define MAX_MOTION_AREA_NUM
 * @brief 移动侦测区域最大设置区域数，这个是与底层相关的设置，请勿修改
 */
#define MAX_MOTION_AREA_NUM 4
/* video part ...*/

/**
 * @struct point
 * @brief 点坐标结构体
 */
struct point
{
	int x_coordinate;	///< 横坐标
	int y_axis; ///< 纵坐标
};

/**
 * @struct area_size
 * @brief 区域范围结构体
 */
struct area_size
{
	struct point left_top_corner; ///< 左上角点坐标
	struct point right_low_corner; ///< 右下角点坐标
};

/**
 * @struct area_property
 * @brief 区域结构体
 */
struct area_property
{
	struct area_size area; ///< 区域范围
	int area_sense; ///< 区域灵敏度，0-4
};

/**
 * @struct motion_dec_param
 * @brief 移动侦测参数结构体
 */
struct motion_dec_param
{
	struct area_property area_pro[MAX_MOTION_AREA_NUM]; ///< 移动侦测区域结构体数组
	int motion_area_num; ///< 需要设置的移动区域数量， 该值可以小于MAX_MOTION_AREA_NUM，但是不能大于MAX_MOTION_AREA_NUM
};

/**
 * @struct mw_resolution
 * @brief 分辨率结构体
 */
struct mw_resolution
{
	int width;  ///< 宽度
	int heigth; ///< 高度
};

/**
 * @struct sensor_param
 * @brief 视频模块参数结构体
 */
struct  sensor_param
{
	struct mw_resolution videoResolution; ///< 视频分辨率		
	struct mw_resolution photoResolution; ///< 图片分辨率
	int workMode;							///< 工作模式 0=normal, 1=night
	int flip_direct;						///< 上下反转 0=normal, 1=flip(up and down)  
	int mirror_direct;						///< 左右反转0=normal, 1=mirror(left and right)
//	int md_witch;					///< 移动侦测开关 0=close , 1=open
};

struct event;


#define VIDEOSMOOTHQULITY		0
#define VIDEOSTANDARDQULITY		1
#define VIDEOHIGHQULITY			2
/**
 * @struct media_v
 * @brief 视频模块接口结构体
 *
 * 该模块提供的接口有:
 * -# 设置视频模块参数，包括视频和图片的分辨率，图像上下和左右反转，工作模式，移动侦测开关\n
 * 其中图像上下和左右反转，以及工作模式一旦设置，同时对视频和图片均生效
 * -# 获取视频模块参数
 * -# 开始录像
 * -# 停止录像
 * -# 获取录像帧数据
 * -# 获取图片数据
 * -# 移动侦测区域设置，包括区域范围和灵敏度
 * -# 移动侦测区域属性获取， 包括区域范围以及每个区域的灵敏度
 * -# 移动侦测开启与关闭
 * -# 移动侦测开关状态获取
 * @example video_test.c
 */
struct media_v
{
	struct sensor_param param; ///< 视频模块参数，不包括移动侦测
	struct motion_dec_param mv_param; ///< 移动侦测参数
	int md_status;					///< 移动侦测当前开关状态
	int ready;						///< 模块准备状态
	
	/**
	 * @fn int (*sensor_param_set)(struct media_v *video, struct sensor_param param)
	 * @brief 设置视频模块的参数，包括视频和图片的分辨率，图像上下和左右反转，工作模式\n
	 * 其中图像上下和左右反转，以及工作模式一旦设置，同时对视频和图片均生效
	 * @param video [in] 必须是 \ref get_media_video 返回指针
	 * @param param [in] 视频模块相关参数，内容参看结构体
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @remark 如果不设置，默认参数为工作在正常模式，图像不反转，视频分辨率720P（1280*720），图片\n
	 * 分辨率为VGA（640*480）
	 * @attention 该操作必须在 \ref get_media_video 操作成功之后才可使用，设置参数时，请保证已停止录像
	 */
	int (*sensor_param_set)(struct media_v *video, struct sensor_param param);
	
	/**
	 * @fn int (*sensor_param_get)(struct media_v *video, struct sensor_param *param)
	 * @brief 获取视频模块的参数
	 * @param video [in]必须是 \ref get_media_video 返回指针
	 * @param param [out] 获取到的视频模块相关参数，内容参看结构体
	 * @retval 0 成功
	 * @attention 该操作必须在 \ref get_media_video 操作成功之后才可使用
	 */
	int (*sensor_param_get)(struct media_v *video, struct sensor_param *param);

	int (*change_video_qulity)(struct media_v *video, int type);
	
	/**
	 * @fn int (*start)(struct media_v *video)
	 * @brief 开始录像
	 * @param video [in]必须是 \ref get_media_video 返回指针
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须在 \ref get_media_video 操作成功之后才可使用
	 */
	int (*start)(struct media_v *video);
	
	/**
	 * @fn int (*stop)(struct media_v *video)
	 * @brief 停止录像
	 * @param video [in] 必须是 \ref get_media_video 返回指针
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须在 \ref get_media_video 操作成功之后才可使用
	 */
	int (*stop)(struct media_v *video);
	
	/**
	 * @fn int (*get_frame) (struct media_v *video, char *buf, int *len, int *frame_flag)
	 * @brief 获取视频帧数据，阻塞
	 * @param video [in]必须是 \ref get_media_video 返回指针
	 * @param buf [out] 获取到的视频帧数据填充缓存
	 * @param len [in][out] 接口调用时，传入 buf 长度，如果因为 buf 长度不够而导致调用失败，\n
	   函数把需求缓存长度写入该变量
	 * @param frame_flag [out] 帧类型, 0为I帧
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须在\ref start 操作成功之后才可使用
	 */
	int (*get_major_frame) (struct media_v *video, char *buf, int *len, int *frame_flag, int *channel);
	int (*get_minor_frame) (struct media_v *video, char *buf, int *len, int *frame_flag, int *channel);
//	int (*get_i_video) (struct media_v *video, char *buf, int *len); 
	
	/**
	 * @fn int (*get_photo) (struct media_v *video, char *buf, int *len)
	 * @brief 获取图片数据，阻塞
	 * @param video [in] 必须是 \ref get_media_video 返回指针
	 * @param buf [out] 获取到的图片数据写入缓存
	 * @param len [in][out] 接口调用时，传入 buf 长度，如果因为 buf 长度不够而导致调用失败，\n
	   函数将把需求缓存长度写入该变量
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须在 \ref get_media_video 操作成功之后才可使用
	 */
	int (*get_photo) (struct media_v *video, char *buf, int *len);

	/**
	 * @fn int (*md_para_set)(struct media_v *video, struct motion_dec_param md_para)
	 * @brief 移动侦测参数设置，包括设置几个区域，以及每个区域的灵敏度
	 * @param video [in] 必须是 \ref get_media_video 返回指针
	 * @param md_para [in] 移动侦测设置参数
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @remark 默认参数为4个区域，每个区域都是全屏，灵敏度均为4
	 * @attention 该操作必须在 \ref get_media_video 操作成功之后才可使用
	 */
	int (*md_para_set)(struct media_v *video, struct motion_dec_param md_para);
	
	/**
	 * @fn int (*md_para_get)(struct media_v *video, struct motion_dec_param *md_para)
	 * @brief 移动侦测参数获取
	 * @param video [in] 必须是 \ref get_media_video 返回指针
	 * @param md_para [out] 存储获取到的移动侦测的数据
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须在 \ref get_media_video 操作成功之后才可使用
	 */
	int (*md_para_get)(struct media_v *video, struct motion_dec_param *md_para);
	
	/**
	 * @fn int (*md_set)(struct media_v *video, int flag)
	 * @brief 设置移动侦测开关
	 * @param video [in] 必须是 \ref get_media_video 返回指针
	 * @param flag [in] flag = 1开启，flag = 0关闭
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @remark 移动侦测默认是关闭的，需要自己打开
	 * @attention 该操作必须在 \ref get_media_video 操作成功之后才可使用
	 */
	//int (*md_set)(struct media_v *video, int flag);
	
	/**
	 * @fn int (*md_get)(struct media_v *video, int *flag)
	 * @brief 获取移动侦测开关状态
	 * @param video [in] 必须是 \ref get_media_video 返回指针
	 * @param flag [out] 存储移动侦测开关状态
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须在 \ref get_media_video 操作成功之后才可使用
	 */
	//int (*md_get)(struct media_v *video, int *flag);
//	int (*get_mov_detc_result)(struct media_v *video, int *ret);   i think need 


	int (*md_result_get)(struct media_v *video, int *result, int *zone);

	//移动侦测区域设置

};

/**
 * @fn struct media_v *get_media_video(void)
 * @brief 获取指向 \ref media_v 的指针，应用层在使用视频模块接口之前，需要先调用该接口，且要成功才对视频模块进行操作
 * @param void
 * @return 指向 media_v 的指针，返回NULL表示失败
 * @attention 应用层调用该接口，意味着对视频模块进行初始化，底层会进行相关资源的申请，以及初始化内部函数指针等信息\n
 * 所有在对视频模块进行操作之前，必须先要调用该接口，且要返回成功
 */
struct media_v *get_media_video(void);

/* audio part ...*/
/**
 * @struct media_a
 * @brief 音频模块接口结构体
 *
 * 该模块提供的接口有:
 * -# 设置/获取音频采集和播放模块的采样率，声道数，采样精度，音量
 * -# 播放音频文件
 * -# 开启和关闭录制音频， 获取音频数据
 * -# 开启和关闭播放语言， 播放语音数据
 * -# 播放音频文件
 * @example audio_test.c
 */
struct media_a
{
	int adc_sam_rate; ///< 音频采集模块采样率
	int adc_ch_mode;  ///< 音频采集声道数
	int adc_sam_bit;  ///< 音频采集采样深度
	int adc_vol;      ///< 音频采集音量

	int dac_sam_rate; ///< 音频播放采样率
	int dac_ch_mode;  ///< 音频播放声道数
	int dac_sam_bit;  ///< 音频播放采样深度
	int dac_vol;      ///< 音频播放音量

	int ready;        ///< 音频模块准备状态 

	/**
	 * @fn int (*adc_param_set) (struct media_a *audio, int rate, int ch_mode, int sam_bit)
	 * @brief 设置音频采集模块参数，包括采样率，声道数，采样深度
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @param rate [in] 采样率
	 * @param ch_mode [in] 声道数
	 * @param sam_bit [in] 采样深度
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @remark 如果不设置，默认参数为8K采样率，16位深度，单声道，每帧50ms
	 * @attention  该操作必须在 \ref get_media_audio 操作成功之后才可使用；只能在停止录音状态下使用该接口
	 */
	int (*adc_param_set) (struct media_a *audio, int rate, int ch_mode, int sam_bit);
	
	/**
	 * @fn int (*adc_param_get) (struct media_a *audio, int *rate, int *ch_mode, int *sam_bit)
	 * @brief 获取音频采集模块参数，包括采样率，声道数，采样深度
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @param rate [out] 采样率
	 * @param ch_mode [out] 声道数
	 * @param sam_bit [out] 采样深度
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用
	 */
	int (*adc_param_get) (struct media_a *audio, int *rate, int *ch_mode, int *sam_bit);
	
	/**
	 * @fn int (*dac_param_set) (struct media_a *audio, int rate, int ch_mode, int sam_bit)
	 * @brief 设置音频播放模块参数，包括采样率，声道数，采样深度
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @param rate [in] 采样率
	 * @param ch_mode [in] 声道数
	 * @param sam_bit [in] 采样深度
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @remark 如果不设置，默认参数为8K，16位深度，单声道
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用，只能在停止播放状态下设置该接口
	 */
	int (*dac_param_set) (struct media_a *audio, int rate, int ch_mode, int sam_bit);
	
	/**
	 * @fn int (*dac_param_get) (struct media_a *audio, int *rate, int *ch_mode, int *sam_bit)
	 * @brief 设置音频播放模块参数，包括采样率，声道数，采样深度
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @param rate [out] 采样率
	 * @param ch_mode [out] 声道数
	 * @param sam_bit [out] 采样深度
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用
	 */
	int (*dac_param_get) (struct media_a *audio, int *rate, int *ch_mode, int *sam_bit);
	
	/**
	 * @fn int (*adc_vol_set) (struct media_a *audio, int value)
	 * @brief 设置音频采集模块音量
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @param value [in] 音量
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @remark 如果不设置，默认为60，软件上不对音量做任何处理，不增加，不减少
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用
	 */
	int (*adc_vol_set) (struct media_a *audio, int value);
	
	/**
	 * @fn int (*adc_vol_get) (struct media_a *audio, int *value)
	 * @brief 获取音频采集模块音量
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @param value [out] 音量
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h 
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用
	 */
	int (*adc_vol_get) (struct media_a *audio, int *value);
	
	/**
	 * @fn int (*dac_vol_set) (struct media_a *audio, int value)
	 * @brief 设置音频播放模块音量
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @param value [in] 音量
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @remark 如果不设置，默认为60，软件上不对音量做任何处理，不增加，不减少
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用

	 */	
	int (*dac_vol_set) (struct media_a *audio, int value);
	
	/**
	 * @fn int (*dac_vol_get) (struct media_a *audio, int *value)
	 * @brief 获取音频播放模块音量
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @param value [out] 音量
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h 
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用
	 */
	int (*dac_vol_get) (struct media_a *audio, int *value);

	/**
	 * @fn int (*record_start) (struct media_a *audio)
	 * @brief 开始录音
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用
	 */
	int (*record_start) (struct media_a *audio);
	
	/**
	 * @fn int (*record_stop) (struct media_a *audio)
	 * @brief 停止录音
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用
	 */
	int (*record_stop) (struct media_a *audio);
	
	/**
	 * @fn int (*get_audio) (struct media_a *audio, char *buf, int *len)
	 * @brief 获取音频数据
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @param buf [out] 存放音频数据的缓存
	 * @param len [in][out] 调用该接口时，将 len 设置为 buf 长度，如果因为buf长度不够而导致调用失败，\n
	 * 接口将用需求的缓存长度写到 len 中
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @remark 该接口每次返回上层3个帧，默认为150ms（每个帧50ms）
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用，该接口使能在 \ref record_start 之\n
	 * 后才可使用
	 */
	int (*get_audio) (struct media_a *audio, char *buf, int *len);
	
	/**
	 * @fn int (*talk_start) (struct media_a *audio)
	 * @brief 开启播放语言
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用
	 */
	int (*talk_start) (struct media_a *audio);
	
	/**
	 * @fn int (*talk_stop) (struct media_a *audio)
	 * @brief 关闭播放语音
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用
	 */
	int (*talk_stop) (struct media_a *audio);
	
	/**
	 * @fn int (*play_voice) (struct media_a *audio, char *buf, int len)
	 * @brief 播放语言
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @param buf [in] 语音数据
	 * @param len [in] 语音数据长度
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @remark 播放语言的采样率，声道，采样深度要一样，而且播放的数据长度应该是50ms的整数倍
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用，该操作必须成功调用 \ref talk_start 之后\n
	 * 才可使用
	 */
	int (*play_voice) (struct media_a *audio, char *buf, int len);
	
	/**
	 * @fn int (*play_file) (struct media_a *audio, char *path, int cnt, void (*call_back)(void))
	 * @brief 播放音频文件
	 * @param audio [in] 必须是 \ref get_media_audio 返回的指针
	 * @param path [in] 音频文件绝对路径
	 * @param cnt [in] 音频文件播放次数
	 * @param call_back [in] 播放完音频文件需要调用的函数
	 * @retval 0 成功
	 * @retval -1 失败，其他错误请参考文件 \ref common.h
	 * @attention 该操作必须在 \ref get_media_audio 操作成功之后才可使用，音频文件格式要求为 \a .wav ，采样率和采样深度和音频播放模块\n
	 * 的参数一样
	 */
	int (*play_file) (struct media_a *audio, char *path, int cnt, int interval, void (*call_back)(void));

//	void (*clear_record)(void);
//	void (*clear_play)(void);
};

/**
 * @fn struct media_a *get_media_audio(void)
 * @brief 获取指向 media_a 的指针，应用层在使用音频模块接口之前，需要先调用该接口，且要成功才对音频模块进行操作
 * @param void
 * @return 指向 media_a 的指针，返回 NULL 表示失败
 * @attention 应用层调用该接口，意味着对音频模块进行初始化，底层会进行相关资源的申请，以及初始化内部函数指针等信息\n
 * 所有在对音频模块进行操作之前，必须先要调用该接口，且要返回成功
 */
struct media_a *get_media_audio(void);


/* event module*/
/*****************************************************/
/**
 * @typedef _EVENT_TYPE
 * @brief 事件类型
 */

#define MAX_EVENT_HANDLE_NUM	8	

typedef enum _EVENT_TYPE {  
	EVENT_CALL = 0,				///< 呼叫
	EVENT_SMART_LINK,			///< 一键配置
	EVENT_NORMAL_ALARM,			///< 普通报警
	EVENT_RESET,				///< 复位
	EVENT_REBOOT,				///< 重启
	EVENT_TAMPER,				///< 防拆
	EVENT_LDR,					///< LDR检测
	EVENT_DOORKEY,				///< 开门
//	EVENT_MOVE, 				///< 移动侦测
//	EVENT_TF_IN,				///< 插入TF卡
//	EVENT_TF_OUT,				///< 拔出TF卡
//	EVENT_WIRE_IN,				///< 插入网线
//	EVENT_WIRE_OUT,				///< 拔出网线
//	EVENT_BAT_LOW,				///< 低电
//	EVENT_BAT_NORMAL,			///< 正常放电状态
//	EVENT_BAT_CHARGE,			///< 正在充电
//	EVENT_BAT_CHARGE_FULL,		///< 充满电
//	EVENT_WIRELESS_SIGNAL_LOW,	///< 无线信号弱
//	EVENT_WIRELESS_SIGNAL_FULL,	///< 无线信号强
//	EVENT_LDR_DAY,				///< LDR 白天
//	EVENT_LDR_NIGHT				///< LDR 晚上
}EVENT_TYPE;

struct event
{
//	int bat_cur_vol;				//电池实时电量
//	EVENT_TYPE event_type;					//pir, move-detect, call

//	struct net_info *net;
//	struct media_v *video;
	/* data */
	//该结构中的所有回调函数原型均需上层实现,上传使用该模块必须首先初始化本结构。
	//该模块仅处理需上报上层的被动事件，包括io状态，卡插拔，移动侦测事件，网络连接，电池电量，wifi信号等。
	
//	int (*set_key_value) (struct event *m_event, int key_code, int value);
//	int (*get_key_value) (struct event *m_event, int key_code);

//	int (*get_bat_vol) (struct event *m_event);			//return bat_cur_vol
//	EVENT_TYPE (*get_bat_status) (struct event *m_event);

	//注册事件处理函数
	int (*registe_event_handle) (EVENT_TYPE eventType, void (*handle)(int value));

	//插入新的事件
	int (*chain_insert_new_event) (EVENT_TYPE eventType, int value);
//	int (*cb_key_event) (int key_code, int value);
	/* 发现插拔事件，更新tf_ject状态位，进行mount,unmount 操作，然后执行回调 */
//	int (*cb_tf_event) (struct event *m_event);
	/* 发现移动侦测事件，更新事件类型，马上拍照保存，拍照时需要传入事件类型，执行回调 */
//	int (*cb_motion_event) (struct event *m_event);
	/* 检测到插入拔出，更新事件类型，然后是否切换网络需要跟上层讨论，执行回调  ？？？？？ */
//	int (*cb_wire_event) (struct event *m_event);		//有线
	/* 检测到低电、充电、充电满后触发，上报事件类型*/
//	int (*cb_bat_event) (struct event *m_event);
	/* 检测光敏电阻 */
//	int (*cb_ldr_event) (int value);

};
struct event *get_mw_event(void);

/*****************************************************/
//GPIO 接口//gpio_num: 当前gpio在cpu总gpio中的顺序编号，例如：申请PB(10) = 1 * 32 + 10 = 42
#define GPIO_LOW  0
#define GPIO_HIGH 1
#define GPIO_IN   0
#define GPIO_OUT  1

int mw_gpio_apply (int gpio_num);	//已经申请过的gpio在sysfs gpio再次申请会失败。
//int mw_gpio_probe (int gpio_num, int default_level, int direct);
int mw_gpio_probe (int gpio_num, int direct, int initValue);
int mw_gpio_set (int gpio_num, int value);
int mw_gpio_get (int gpio_num, int *value);
int mw_gpio_release (int gpio_num);


/* protocal module*/
/*****************************************************/
struct i2c_dev {
	int dev_num;						//区分i2c总线类型, 0 or 1
	int bit_rate;						//总线频率
	int reg_bit;						//设备寄存器bit数，7 or 9

	int (*init) (struct i2c_dev *dev);

	int (*open) (struct i2c_dev *dev);
	int (*read) (struct i2c_dev *dev, int addr, short reg);
	int (*write) (struct i2c_dev *dev, int addr, short reg, char value);
	int (*close) (struct i2c_dev *dev);

};
struct i2c_dev *get_mw_i2c_dev(void);

struct uart_dev {
	int dev_num;						//区分uart总线类型, 0 or 1
	int rate;							//总线频率
	int data_bit;
	int parity;
	int stop_bit;
	int flow_ctrl;

	int (*init) (struct uart_dev *dev);

	int (*open) (struct uart_dev *dev);
	int (*read) (struct uart_dev *dev, char *buf, int len);
	int (*write) (struct uart_dev *dev, char *buf, int len);
	int (*close) (struct uart_dev *dev);

};
struct uart_dev *get_mw_uart_dev(void);

/* misc module*/
/*****************************************************/
typedef enum _MODULE_TYPE {  
	MODULE_WIFI = 1,
	MODULE_SENSOR,
	MODULE_AUDIO,
	MODULE_VIDEO,
	MODULE_ECHO
}MODULE_TYPE;

typedef enum _MODULE_STAT {  
	MODULE_STADBY = 1,
	MODULE_INITING,
	MODULE_INITED,
	MODULE_ABNORMAL,
	MODULE_UNKOWN
}MODULE_STAT;

struct pm_fun
{
	/* data */
	int enabel;							//使能本模块

	int stat_wifi;
	int stat_sensor;
	int stat_audio;
	int stat_video;
	int stat_echo;

	int (*set) (struct pm_fun *pm, int flag);
	int (*get) (struct pm_fun *pm);		//获取本模块当前使能状态

	/* 上层在调用下述接口前，必须先进行有关数据保存和处理上下文*/
	int (*suspend) (struct pm_fun *pm);		//调用后设备会立即进入休眠,函数正常退出后说明系统已经唤醒.		
	/*note: 执行休眠时，将自动将有关模块进行断电操作，以降低功耗，故调用前需保存数据*/
	int (*reboot) (struct pm_fun *pm);							
	int (*shutdown) (struct pm_fun *pm);	//系统关机

	/*  唤醒后上层需调用该接口查看指定模块是否就绪 */
	int (*pm_get_status) (struct pm_fun *pm, MODULE_TYPE module_type);
};
struct pm_fun *get_mw_pm(void);


/*****************************************************/
int middleware_init(void);
int middleware_uninit(void);

/**
 * @}
 */
#endif 	//__MIDDLEWARE_H__


