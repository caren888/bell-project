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
 * @brief ����ļ�Ϊ�ϲ�Ӧ�ÿ����ṩ��һЩ�ӿڣ���Щ�ӿڰ��չ��ܷ�װ���˲�ͬ�Ĺ��ܽṹ���С�
 *
 * Ӧ�ÿ������ýӿڰ�����
 * -# ��Ƶģ��
 * -# ��Ƶģ��
 * -# ����ģ��
 * -# �¼�ģ��
 * -# I2Cģ��
 * -# UARTģ��
 * -# PMģ��
 * 
 * ����ÿ��ģ���ṩ�ӿڵľ�����Ϣ������ϸ�Ķ�ÿ��ģ��ṹ���˵����Ϣ
 * @attention ʹ����Щģ����ʹ��ģ���ڲ��ĳ�Ա������ �벻Ҫʹ�ó�Ա��������Ҫ�õ��ĳ�Ա����\n
 * ������ͨ��ģ���ṩ�Ľӿں������в���
 * @{
 */

/* network module*/
/*****************************************************/
/**
 * @typedef NET_MODE
 * @brief ����ģʽ
*/
typedef enum _NET_MODE {  
	NET_MODE_AP = 1, 	///< APģʽ
	NET_MODE_WIRE, 		///< wifiģʽ
	NET_MODE_WIRELESS, 	///< ����ģʽ
	NET_MODE_YIJIAN		///< һ������
}NET_MODE;

/**
 * @typedef _NET_CONN_STATUS
 * @brief ����״̬
 */
typedef enum _NET_CONN_STATUS {  
	NET_CONNING = 1,	///< �ɳ�����
	NET_UNCONN,			///< �Ͽ�
	NET_CONNED			///< ����
}NET_CONN_STATUS;

/**
 *@struct net_para
 *@brief �������������
 */
struct net_para {
	char ip_addr[16]; ///< IP��ַ
	char net_gate[16];/// ����
	char net_mask[16];///��������
	char dns1[16];///DNS
	char dns2[16];///DNS
};

/**
 * @struct wifi_scan_res
 * @brief wifi��Ϣ�ṹ��
 */
struct wifi_scan_res{
	char bssid[20]; ///< bssid���൱��Ӳ����ַ
	int freq; ///< �ŵ���Ƶ��
	int sig_level; ///< �ź�ǿ��
	char flags[128]; ///< ���ܺͰ�ȫģʽ
	char ssid[128];///< wifi��
};

/**
 * @struct net_info
 * @brief ����ģ�鹦�ܽṹ��
 *
 * ��ģ���ṩ�Ĳ����У�
 * -# ����ģʽ���л���AP, ���ߺ�����ֱ�ӵ��л���ģ��������ʼ��ʱĬ���л�����Чģʽ
 * -# ����ģʽ�Ļ�ȡ
 * -# ������������ã�������ֻ������ߺ�����ģʽ��Ч��������DHCP��Ҳ�ɾ�̬����\n
 * ��̬������Ҫ���ò�������ip��ַ�����أ��������룬����dns
 * -# ���������ȡ�� �ò���ֻ������ߺ�����ģʽ��Ч
 * -# ��ȡ��������״�����ò���ֻ�����Ч������ģʽ��Ч����ͬʱ��ȡ����������������״̬
 * -# wifi���ã�����wifi�������룬�ò���ֻ����wifiģʽ��ʹ��
 * -# wifiɨ�裬��ȡ������wifi�б� �ò���ֻ����wifiģʽ��ʹ��
 * -# wifi�ź�ǿ�Ȼ�ȡ�� �ò���ֻ����wifiģʽ��ʹ�ã�����ֻ�ܻ�ȡ����ǰ����wifi���ź�ǿ��
 * -# ap���ã� ����ap�����ƺ�����
 * -# ap��ȡ�� ��ȡap���ƺ�����
 * @example network_test.c
 */
struct net_info
{
	NET_MODE mode; /**< ģ�鵱ǰ����������ģʽ�����ײ�ʹ�ã�Ӧ�ò��벻Ҫֱ�Ӳ��� */
	NET_CONN_STATUS con_status; /**< ��ǰ������״̬�� ���ײ�ʹ�ã� Ӧ�ò��벻Ҫֱ�Ӳ��� */
	int wifi_sign; /**< wifi�ź�ǿ�ȣ� ���ײ�ʹ�ã� Ӧ�ò��벻Ҫֱ�Ӳ���*/
	int ready; /**< ģ��׼���������ready=1��ʾģ�������������� ����������ģ�飬 ���ײ�ʹ��\n
	Ӧ�ò��벻Ҫֱ�Ӳ���*/
	
	/**
	 * @fn int (*mode_set) (struct net_info *net, NET_MODE mode)
	 * @brief ���õ�ǰ������ģʽ
	 * @param net [in] ������ \ref get_mw_network ���ص�ָ��
	 * @param mode [in] ��Ҫ���õ�����ģʽ
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������鿴 \ref common.h
	 * @attention �ò������� \ref get_mw_network �����ɹ��ſ�ʹ��
	 */
	int (*mode_set) (struct net_info *net, NET_MODE mode);
	
	/**
	 * @fn int (*mode_get) (struct net_info *net)
	 * @brief ��ȡ��ǰ������ģʽ
	 * @param[in] net ������ \ref get_mw_network ���ص�ָ��
	 * @return ��ǰ������ģʽ��1 AP�� 2 wifi�� 3 ����
	 * @attention �ò������� \ref get_mw_network �����ɹ��ſ�ʹ��
	 */
	int (*mode_get) (struct net_info *net);
	
	/**
	 * @fn int (*set_mode_para) (struct net_info *net, NET_MODE mode, int dhcp_flag, struct net_para net_para)
	 * @brief �������������ֻ���wifi������ģʽ��Ч��������DHCP���ѿɾ�̬���ã���̬������Ҫ�������ṹ�� net_para
	 *
	 * DHCP���ã�����Ҫ�������ṹ�壬DHCP���Զ�����\n
	 * ��̬���ã���Ҫ�������ṹ�壬�������IP��ַ���������룬���أ�����DNS
	 * @param net [in] ������ \ref get_mw_network ���ص�ָ��
	 * @param mode [in] ��ǰ������ģʽ
	 * @param dhcp_flag [in] dhcp��־��1Ϊdhc��0λ��̬����
	 * @param net_para [in] �����ṹ��
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò������� \ref get_mw_network �����ɹ��ſ�ʹ��
	 */
	int (*set_mode_para) (struct net_info *net, NET_MODE mode, int dhcp_flag, struct net_para net_para);

	/**
	 * @fn int (*get_mode_para) (struct net_info *net, NET_MODE mode, struct net_para *net_para)
	 * @brief ��ȡ��������� ֻ���wifi������ģʽ��Ч
	 * @param net [in] ������ \ref get_mw_network ���ص�ָ��
	 * @param mode [in] ��ǰ����ģʽ
	 * @param net_para [out] ��������ṹ��ָ��
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h	 
	 * @attention �ò������� \ref get_mw_network �����ɹ��ſ�ʹ��
	 */
	int (*get_mode_para) (struct net_info *net, NET_MODE mode, struct net_para *net_para);
	
	/**
	 * @fn int (*get_conn_sta) (struct net_info *net, int *intra_sta, int *inter_sta)
	 * @brief ��ȡ��ǰ������״̬��ֻ���wifi������ģʽ��Ч
	 * @param net [in]������ \ref get_mw_network ���ص�ָ��
	 * @param intra_sta [out]��������������״̬
	 * @param inter_sta [out]��������������״̬
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò������� \ref get_mw_network �����ɹ��ſ�ʹ��
	 */
	int (*get_conn_sta) (struct net_info *net, int *intra_sta, int *inter_sta);
	
	/**
	 * @fn int (*wifi_set) (struct net_info *net, short *ssid, short *password, char *security)
	 * @brief ����wifi�������룬�ò���ֻ����wifiģʽ��ʹ��
	 * @param net [in]������ \ref get_mw_network ���ص�ָ��
	 * @param ssid [in]ָ��wifi����ָ��
	 * @param password [in] ָ��wifi�����ָ��
	 * @param security [in] ָ�����ģʽ��ָ��
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò������� \ref get_mw_network �����ɹ��ſ�ʹ��
	 */
	int (*wifi_set) (struct net_info *net, short *ssid, short *password, char *security);	//utf16 for special character.
	
	/**
	 * @fn int (*wifi_scan) (struct net_info *net, struct wifi_scan_res *buf_list, int *count)
	 * @brief wifiɨ�裬��ȡ������wifi�б��ò���ֻ����wifiģʽ��ʹ��
	 * @param net [in] ������ \ref get_mw_network ���ص�ָ��
	 * @param buf_list [out] ��ȡ����wifi�б�ṹ��
	 * @param count [in][out]����ʱ��Ҫ������Ҫ��ȡ��wifi�б�ĸ�������������ɹ�������ʱ���򽫸�ֵ����Ϊ������ȡ���ĸ���
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @remark ���鲻Ҫ����32��
	 * @attention �ò������� \ref get_mw_network �����ɹ��ſ�ʹ��
	 */
	int (*wifi_scan) (struct net_info *net, struct wifi_scan_res *buf_list, int *count);
	
	/**
	 * @fn int (*get_wifi_sig_lev) (struct net_info *net, int *sig_lev)
	 * @brief ��ȡwifi�ź�ǿ�ȣ��ò���ֻ����wifiģʽ��ʹ��
	 * @param net [in] ������ \ref get_mw_network ���ص�ָ��
	 * @param sig_lev [out] �����ź�ǿ��
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò������� \ref get_mw_network �����ɹ��ſ�ʹ�ã��ò���ֻ�ɻ�ȡ����ǰ����wifi���ź�ǿ��
	 */
	int (*get_wifi_sig_lev) (struct net_info *net, int *sig_lev);
	
	/**
	 * @fn int (*ap_set) (struct net_info *net, char *ssid, char *passwd)
	 * @brief ap���ã�����ap���ƺ�����
	 * @param net [in] ������ \ref get_mw_network ���ص�ָ��
	 * @param ssid [in] ap����
	 * @param passwd [in] ap����
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @remark ��ʼAP����ΪRT2860AP��������
	 * @attention �ò������� \ref get_mw_network �����ɹ��ſ�ʹ��
	 */
	int (*ap_set) (struct net_info *net, char *ssid, char *passwd);
	
	/**
	 * @fn int (*ap_get) (struct net_info *net, char *ssid, char *passwd)
	 * @brief ��ȡap���ƺ�����
	 * @param net [in]������ \ref get_mw_network ���ص�ָ��
	 * @param ssid [out] ָ��洢��ȡ���� \a ssid �����ȸ���ʵ���������������
	 * @param passwd [out] ָ������ȡ���� \a passwd �����ȸ���ʵ��������Լ�����
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò������� \ref get_mw_network �����ɹ��ſ�ʹ��
	 */
	int (*ap_get) (struct net_info *net, char *ssid, char *passwd);
};

/**
 * @fn struct net_info *get_mw_network(void)
 * @brief ��ȡָ�� net_info ��ָ�룬Ӧ�ò���ʹ������ģ��ӿ�֮ǰ����Ҫ�ȵ��øýӿڣ���Ҫ�ɹ��Ŷ�����ģ����в���
 * @param void
 * @return ָ�� net_info ��ָ�룬����NULL��ʾʧ��
 * @attention Ӧ�ò���øýӿڣ���ζ�Ŷ�����ģ����г�ʼ�����ײ����������Դ�����룬�Լ���ʼ���ڲ�����ָ�����Ϣ\n
 * �����ڶ�����ģ����в���֮ǰ��������Ҫ���øýӿڣ���Ҫ���سɹ�
 */
struct net_info *get_mw_network(void);

/* media data module */
/*****************************************************/

/**
 * @define MAX_MOTION_AREA_NUM
 * @brief �ƶ������������������������������ײ���ص����ã������޸�
 */
#define MAX_MOTION_AREA_NUM 4
/* video part ...*/

/**
 * @struct point
 * @brief ������ṹ��
 */
struct point
{
	int x_coordinate;	///< ������
	int y_axis; ///< ������
};

/**
 * @struct area_size
 * @brief ����Χ�ṹ��
 */
struct area_size
{
	struct point left_top_corner; ///< ���Ͻǵ�����
	struct point right_low_corner; ///< ���½ǵ�����
};

/**
 * @struct area_property
 * @brief ����ṹ��
 */
struct area_property
{
	struct area_size area; ///< ����Χ
	int area_sense; ///< ���������ȣ�0-4
};

/**
 * @struct motion_dec_param
 * @brief �ƶ��������ṹ��
 */
struct motion_dec_param
{
	struct area_property area_pro[MAX_MOTION_AREA_NUM]; ///< �ƶ��������ṹ������
	int motion_area_num; ///< ��Ҫ���õ��ƶ����������� ��ֵ����С��MAX_MOTION_AREA_NUM�����ǲ��ܴ���MAX_MOTION_AREA_NUM
};

/**
 * @struct mw_resolution
 * @brief �ֱ��ʽṹ��
 */
struct mw_resolution
{
	int width;  ///< ���
	int heigth; ///< �߶�
};

/**
 * @struct sensor_param
 * @brief ��Ƶģ������ṹ��
 */
struct  sensor_param
{
	struct mw_resolution videoResolution; ///< ��Ƶ�ֱ���		
	struct mw_resolution photoResolution; ///< ͼƬ�ֱ���
	int workMode;							///< ����ģʽ 0=normal, 1=night
	int flip_direct;						///< ���·�ת 0=normal, 1=flip(up and down)  
	int mirror_direct;						///< ���ҷ�ת0=normal, 1=mirror(left and right)
//	int md_witch;					///< �ƶ���⿪�� 0=close , 1=open
};

struct event;


#define VIDEOSMOOTHQULITY		0
#define VIDEOSTANDARDQULITY		1
#define VIDEOHIGHQULITY			2
/**
 * @struct media_v
 * @brief ��Ƶģ��ӿڽṹ��
 *
 * ��ģ���ṩ�Ľӿ���:
 * -# ������Ƶģ�������������Ƶ��ͼƬ�ķֱ��ʣ�ͼ�����º����ҷ�ת������ģʽ���ƶ���⿪��\n
 * ����ͼ�����º����ҷ�ת���Լ�����ģʽһ�����ã�ͬʱ����Ƶ��ͼƬ����Ч
 * -# ��ȡ��Ƶģ�����
 * -# ��ʼ¼��
 * -# ֹͣ¼��
 * -# ��ȡ¼��֡����
 * -# ��ȡͼƬ����
 * -# �ƶ�����������ã���������Χ��������
 * -# �ƶ�����������Ի�ȡ�� ��������Χ�Լ�ÿ�������������
 * -# �ƶ���⿪����ر�
 * -# �ƶ���⿪��״̬��ȡ
 * @example video_test.c
 */
struct media_v
{
	struct sensor_param param; ///< ��Ƶģ��������������ƶ����
	struct motion_dec_param mv_param; ///< �ƶ�������
	int md_status;					///< �ƶ���⵱ǰ����״̬
	int ready;						///< ģ��׼��״̬
	
	/**
	 * @fn int (*sensor_param_set)(struct media_v *video, struct sensor_param param)
	 * @brief ������Ƶģ��Ĳ�����������Ƶ��ͼƬ�ķֱ��ʣ�ͼ�����º����ҷ�ת������ģʽ\n
	 * ����ͼ�����º����ҷ�ת���Լ�����ģʽһ�����ã�ͬʱ����Ƶ��ͼƬ����Ч
	 * @param video [in] ������ \ref get_media_video ����ָ��
	 * @param param [in] ��Ƶģ����ز��������ݲο��ṹ��
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @remark ��������ã�Ĭ�ϲ���Ϊ����������ģʽ��ͼ�񲻷�ת����Ƶ�ֱ���720P��1280*720����ͼƬ\n
	 * �ֱ���ΪVGA��640*480��
	 * @attention �ò��������� \ref get_media_video �����ɹ�֮��ſ�ʹ�ã����ò���ʱ���뱣֤��ֹͣ¼��
	 */
	int (*sensor_param_set)(struct media_v *video, struct sensor_param param);
	
	/**
	 * @fn int (*sensor_param_get)(struct media_v *video, struct sensor_param *param)
	 * @brief ��ȡ��Ƶģ��Ĳ���
	 * @param video [in]������ \ref get_media_video ����ָ��
	 * @param param [out] ��ȡ������Ƶģ����ز��������ݲο��ṹ��
	 * @retval 0 �ɹ�
	 * @attention �ò��������� \ref get_media_video �����ɹ�֮��ſ�ʹ��
	 */
	int (*sensor_param_get)(struct media_v *video, struct sensor_param *param);

	int (*change_video_qulity)(struct media_v *video, int type);
	
	/**
	 * @fn int (*start)(struct media_v *video)
	 * @brief ��ʼ¼��
	 * @param video [in]������ \ref get_media_video ����ָ��
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò��������� \ref get_media_video �����ɹ�֮��ſ�ʹ��
	 */
	int (*start)(struct media_v *video);
	
	/**
	 * @fn int (*stop)(struct media_v *video)
	 * @brief ֹͣ¼��
	 * @param video [in] ������ \ref get_media_video ����ָ��
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò��������� \ref get_media_video �����ɹ�֮��ſ�ʹ��
	 */
	int (*stop)(struct media_v *video);
	
	/**
	 * @fn int (*get_frame) (struct media_v *video, char *buf, int *len, int *frame_flag)
	 * @brief ��ȡ��Ƶ֡���ݣ�����
	 * @param video [in]������ \ref get_media_video ����ָ��
	 * @param buf [out] ��ȡ������Ƶ֡������仺��
	 * @param len [in][out] �ӿڵ���ʱ������ buf ���ȣ������Ϊ buf ���Ȳ��������µ���ʧ�ܣ�\n
	   ���������󻺴泤��д��ñ���
	 * @param frame_flag [out] ֡����, 0ΪI֡
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò���������\ref start �����ɹ�֮��ſ�ʹ��
	 */
	int (*get_major_frame) (struct media_v *video, char *buf, int *len, int *frame_flag, int *channel);
	int (*get_minor_frame) (struct media_v *video, char *buf, int *len, int *frame_flag, int *channel);
//	int (*get_i_video) (struct media_v *video, char *buf, int *len); 
	
	/**
	 * @fn int (*get_photo) (struct media_v *video, char *buf, int *len)
	 * @brief ��ȡͼƬ���ݣ�����
	 * @param video [in] ������ \ref get_media_video ����ָ��
	 * @param buf [out] ��ȡ����ͼƬ����д�뻺��
	 * @param len [in][out] �ӿڵ���ʱ������ buf ���ȣ������Ϊ buf ���Ȳ��������µ���ʧ�ܣ�\n
	   �����������󻺴泤��д��ñ���
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò��������� \ref get_media_video �����ɹ�֮��ſ�ʹ��
	 */
	int (*get_photo) (struct media_v *video, char *buf, int *len);

	/**
	 * @fn int (*md_para_set)(struct media_v *video, struct motion_dec_param md_para)
	 * @brief �ƶ����������ã��������ü��������Լ�ÿ�������������
	 * @param video [in] ������ \ref get_media_video ����ָ��
	 * @param md_para [in] �ƶ�������ò���
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @remark Ĭ�ϲ���Ϊ4������ÿ��������ȫ���������Ⱦ�Ϊ4
	 * @attention �ò��������� \ref get_media_video �����ɹ�֮��ſ�ʹ��
	 */
	int (*md_para_set)(struct media_v *video, struct motion_dec_param md_para);
	
	/**
	 * @fn int (*md_para_get)(struct media_v *video, struct motion_dec_param *md_para)
	 * @brief �ƶ���������ȡ
	 * @param video [in] ������ \ref get_media_video ����ָ��
	 * @param md_para [out] �洢��ȡ�����ƶ���������
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò��������� \ref get_media_video �����ɹ�֮��ſ�ʹ��
	 */
	int (*md_para_get)(struct media_v *video, struct motion_dec_param *md_para);
	
	/**
	 * @fn int (*md_set)(struct media_v *video, int flag)
	 * @brief �����ƶ���⿪��
	 * @param video [in] ������ \ref get_media_video ����ָ��
	 * @param flag [in] flag = 1������flag = 0�ر�
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @remark �ƶ����Ĭ���ǹرյģ���Ҫ�Լ���
	 * @attention �ò��������� \ref get_media_video �����ɹ�֮��ſ�ʹ��
	 */
	//int (*md_set)(struct media_v *video, int flag);
	
	/**
	 * @fn int (*md_get)(struct media_v *video, int *flag)
	 * @brief ��ȡ�ƶ���⿪��״̬
	 * @param video [in] ������ \ref get_media_video ����ָ��
	 * @param flag [out] �洢�ƶ���⿪��״̬
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò��������� \ref get_media_video �����ɹ�֮��ſ�ʹ��
	 */
	//int (*md_get)(struct media_v *video, int *flag);
//	int (*get_mov_detc_result)(struct media_v *video, int *ret);   i think need 


	int (*md_result_get)(struct media_v *video, int *result, int *zone);

	//�ƶ������������

};

/**
 * @fn struct media_v *get_media_video(void)
 * @brief ��ȡָ�� \ref media_v ��ָ�룬Ӧ�ò���ʹ����Ƶģ��ӿ�֮ǰ����Ҫ�ȵ��øýӿڣ���Ҫ�ɹ��Ŷ���Ƶģ����в���
 * @param void
 * @return ָ�� media_v ��ָ�룬����NULL��ʾʧ��
 * @attention Ӧ�ò���øýӿڣ���ζ�Ŷ���Ƶģ����г�ʼ�����ײ����������Դ�����룬�Լ���ʼ���ڲ�����ָ�����Ϣ\n
 * �����ڶ���Ƶģ����в���֮ǰ��������Ҫ���øýӿڣ���Ҫ���سɹ�
 */
struct media_v *get_media_video(void);

/* audio part ...*/
/**
 * @struct media_a
 * @brief ��Ƶģ��ӿڽṹ��
 *
 * ��ģ���ṩ�Ľӿ���:
 * -# ����/��ȡ��Ƶ�ɼ��Ͳ���ģ��Ĳ����ʣ����������������ȣ�����
 * -# ������Ƶ�ļ�
 * -# �����͹ر�¼����Ƶ�� ��ȡ��Ƶ����
 * -# �����͹رղ������ԣ� ������������
 * -# ������Ƶ�ļ�
 * @example audio_test.c
 */
struct media_a
{
	int adc_sam_rate; ///< ��Ƶ�ɼ�ģ�������
	int adc_ch_mode;  ///< ��Ƶ�ɼ�������
	int adc_sam_bit;  ///< ��Ƶ�ɼ��������
	int adc_vol;      ///< ��Ƶ�ɼ�����

	int dac_sam_rate; ///< ��Ƶ���Ų�����
	int dac_ch_mode;  ///< ��Ƶ����������
	int dac_sam_bit;  ///< ��Ƶ���Ų������
	int dac_vol;      ///< ��Ƶ��������

	int ready;        ///< ��Ƶģ��׼��״̬ 

	/**
	 * @fn int (*adc_param_set) (struct media_a *audio, int rate, int ch_mode, int sam_bit)
	 * @brief ������Ƶ�ɼ�ģ����������������ʣ����������������
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @param rate [in] ������
	 * @param ch_mode [in] ������
	 * @param sam_bit [in] �������
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @remark ��������ã�Ĭ�ϲ���Ϊ8K�����ʣ�16λ��ȣ���������ÿ֡50ms
	 * @attention  �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ�ã�ֻ����ֹͣ¼��״̬��ʹ�øýӿ�
	 */
	int (*adc_param_set) (struct media_a *audio, int rate, int ch_mode, int sam_bit);
	
	/**
	 * @fn int (*adc_param_get) (struct media_a *audio, int *rate, int *ch_mode, int *sam_bit)
	 * @brief ��ȡ��Ƶ�ɼ�ģ����������������ʣ����������������
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @param rate [out] ������
	 * @param ch_mode [out] ������
	 * @param sam_bit [out] �������
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ��
	 */
	int (*adc_param_get) (struct media_a *audio, int *rate, int *ch_mode, int *sam_bit);
	
	/**
	 * @fn int (*dac_param_set) (struct media_a *audio, int rate, int ch_mode, int sam_bit)
	 * @brief ������Ƶ����ģ����������������ʣ����������������
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @param rate [in] ������
	 * @param ch_mode [in] ������
	 * @param sam_bit [in] �������
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @remark ��������ã�Ĭ�ϲ���Ϊ8K��16λ��ȣ�������
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ�ã�ֻ����ֹͣ����״̬�����øýӿ�
	 */
	int (*dac_param_set) (struct media_a *audio, int rate, int ch_mode, int sam_bit);
	
	/**
	 * @fn int (*dac_param_get) (struct media_a *audio, int *rate, int *ch_mode, int *sam_bit)
	 * @brief ������Ƶ����ģ����������������ʣ����������������
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @param rate [out] ������
	 * @param ch_mode [out] ������
	 * @param sam_bit [out] �������
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ��
	 */
	int (*dac_param_get) (struct media_a *audio, int *rate, int *ch_mode, int *sam_bit);
	
	/**
	 * @fn int (*adc_vol_set) (struct media_a *audio, int value)
	 * @brief ������Ƶ�ɼ�ģ������
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @param value [in] ����
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @remark ��������ã�Ĭ��Ϊ60������ϲ����������κδ��������ӣ�������
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ��
	 */
	int (*adc_vol_set) (struct media_a *audio, int value);
	
	/**
	 * @fn int (*adc_vol_get) (struct media_a *audio, int *value)
	 * @brief ��ȡ��Ƶ�ɼ�ģ������
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @param value [out] ����
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h 
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ��
	 */
	int (*adc_vol_get) (struct media_a *audio, int *value);
	
	/**
	 * @fn int (*dac_vol_set) (struct media_a *audio, int value)
	 * @brief ������Ƶ����ģ������
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @param value [in] ����
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @remark ��������ã�Ĭ��Ϊ60������ϲ����������κδ��������ӣ�������
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ��

	 */	
	int (*dac_vol_set) (struct media_a *audio, int value);
	
	/**
	 * @fn int (*dac_vol_get) (struct media_a *audio, int *value)
	 * @brief ��ȡ��Ƶ����ģ������
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @param value [out] ����
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h 
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ��
	 */
	int (*dac_vol_get) (struct media_a *audio, int *value);

	/**
	 * @fn int (*record_start) (struct media_a *audio)
	 * @brief ��ʼ¼��
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ��
	 */
	int (*record_start) (struct media_a *audio);
	
	/**
	 * @fn int (*record_stop) (struct media_a *audio)
	 * @brief ֹͣ¼��
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ��
	 */
	int (*record_stop) (struct media_a *audio);
	
	/**
	 * @fn int (*get_audio) (struct media_a *audio, char *buf, int *len)
	 * @brief ��ȡ��Ƶ����
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @param buf [out] �����Ƶ���ݵĻ���
	 * @param len [in][out] ���øýӿ�ʱ���� len ����Ϊ buf ���ȣ������Ϊbuf���Ȳ��������µ���ʧ�ܣ�\n
	 * �ӿڽ�������Ļ��泤��д�� len ��
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @remark �ýӿ�ÿ�η����ϲ�3��֡��Ĭ��Ϊ150ms��ÿ��֡50ms��
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ�ã��ýӿ�ʹ���� \ref record_start ֮\n
	 * ��ſ�ʹ��
	 */
	int (*get_audio) (struct media_a *audio, char *buf, int *len);
	
	/**
	 * @fn int (*talk_start) (struct media_a *audio)
	 * @brief ������������
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ��
	 */
	int (*talk_start) (struct media_a *audio);
	
	/**
	 * @fn int (*talk_stop) (struct media_a *audio)
	 * @brief �رղ�������
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ��
	 */
	int (*talk_stop) (struct media_a *audio);
	
	/**
	 * @fn int (*play_voice) (struct media_a *audio, char *buf, int len)
	 * @brief ��������
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @param buf [in] ��������
	 * @param len [in] �������ݳ���
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @remark �������ԵĲ����ʣ��������������Ҫһ�������Ҳ��ŵ����ݳ���Ӧ����50ms��������
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ�ã��ò�������ɹ����� \ref talk_start ֮��\n
	 * �ſ�ʹ��
	 */
	int (*play_voice) (struct media_a *audio, char *buf, int len);
	
	/**
	 * @fn int (*play_file) (struct media_a *audio, char *path, int cnt, void (*call_back)(void))
	 * @brief ������Ƶ�ļ�
	 * @param audio [in] ������ \ref get_media_audio ���ص�ָ��
	 * @param path [in] ��Ƶ�ļ�����·��
	 * @param cnt [in] ��Ƶ�ļ����Ŵ���
	 * @param call_back [in] ��������Ƶ�ļ���Ҫ���õĺ���
	 * @retval 0 �ɹ�
	 * @retval -1 ʧ�ܣ�����������ο��ļ� \ref common.h
	 * @attention �ò��������� \ref get_media_audio �����ɹ�֮��ſ�ʹ�ã���Ƶ�ļ���ʽҪ��Ϊ \a .wav �������ʺͲ�����Ⱥ���Ƶ����ģ��\n
	 * �Ĳ���һ��
	 */
	int (*play_file) (struct media_a *audio, char *path, int cnt, int interval, void (*call_back)(void));

//	void (*clear_record)(void);
//	void (*clear_play)(void);
};

/**
 * @fn struct media_a *get_media_audio(void)
 * @brief ��ȡָ�� media_a ��ָ�룬Ӧ�ò���ʹ����Ƶģ��ӿ�֮ǰ����Ҫ�ȵ��øýӿڣ���Ҫ�ɹ��Ŷ���Ƶģ����в���
 * @param void
 * @return ָ�� media_a ��ָ�룬���� NULL ��ʾʧ��
 * @attention Ӧ�ò���øýӿڣ���ζ�Ŷ���Ƶģ����г�ʼ�����ײ����������Դ�����룬�Լ���ʼ���ڲ�����ָ�����Ϣ\n
 * �����ڶ���Ƶģ����в���֮ǰ��������Ҫ���øýӿڣ���Ҫ���سɹ�
 */
struct media_a *get_media_audio(void);


/* event module*/
/*****************************************************/
/**
 * @typedef _EVENT_TYPE
 * @brief �¼�����
 */

#define MAX_EVENT_HANDLE_NUM	8	

typedef enum _EVENT_TYPE {  
	EVENT_CALL = 0,				///< ����
	EVENT_SMART_LINK,			///< һ������
	EVENT_NORMAL_ALARM,			///< ��ͨ����
	EVENT_RESET,				///< ��λ
	EVENT_REBOOT,				///< ����
	EVENT_TAMPER,				///< ����
	EVENT_LDR,					///< LDR���
	EVENT_DOORKEY,				///< ����
//	EVENT_MOVE, 				///< �ƶ����
//	EVENT_TF_IN,				///< ����TF��
//	EVENT_TF_OUT,				///< �γ�TF��
//	EVENT_WIRE_IN,				///< ��������
//	EVENT_WIRE_OUT,				///< �γ�����
//	EVENT_BAT_LOW,				///< �͵�
//	EVENT_BAT_NORMAL,			///< �����ŵ�״̬
//	EVENT_BAT_CHARGE,			///< ���ڳ��
//	EVENT_BAT_CHARGE_FULL,		///< ������
//	EVENT_WIRELESS_SIGNAL_LOW,	///< �����ź���
//	EVENT_WIRELESS_SIGNAL_FULL,	///< �����ź�ǿ
//	EVENT_LDR_DAY,				///< LDR ����
//	EVENT_LDR_NIGHT				///< LDR ����
}EVENT_TYPE;

struct event
{
//	int bat_cur_vol;				//���ʵʱ����
//	EVENT_TYPE event_type;					//pir, move-detect, call

//	struct net_info *net;
//	struct media_v *video;
	/* data */
	//�ýṹ�е����лص�����ԭ�;����ϲ�ʵ��,�ϴ�ʹ�ø�ģ��������ȳ�ʼ�����ṹ��
	//��ģ����������ϱ��ϲ�ı����¼�������io״̬������Σ��ƶ�����¼����������ӣ���ص�����wifi�źŵȡ�
	
//	int (*set_key_value) (struct event *m_event, int key_code, int value);
//	int (*get_key_value) (struct event *m_event, int key_code);

//	int (*get_bat_vol) (struct event *m_event);			//return bat_cur_vol
//	EVENT_TYPE (*get_bat_status) (struct event *m_event);

	//ע���¼�������
	int (*registe_event_handle) (EVENT_TYPE eventType, void (*handle)(int value));

	//�����µ��¼�
	int (*chain_insert_new_event) (EVENT_TYPE eventType, int value);
//	int (*cb_key_event) (int key_code, int value);
	/* ���ֲ���¼�������tf_ject״̬λ������mount,unmount ������Ȼ��ִ�лص� */
//	int (*cb_tf_event) (struct event *m_event);
	/* �����ƶ�����¼��������¼����ͣ��������ձ��棬����ʱ��Ҫ�����¼����ͣ�ִ�лص� */
//	int (*cb_motion_event) (struct event *m_event);
	/* ��⵽����γ��������¼����ͣ�Ȼ���Ƿ��л�������Ҫ���ϲ����ۣ�ִ�лص�  ���������� */
//	int (*cb_wire_event) (struct event *m_event);		//����
	/* ��⵽�͵硢��硢������󴥷����ϱ��¼�����*/
//	int (*cb_bat_event) (struct event *m_event);
	/* ���������� */
//	int (*cb_ldr_event) (int value);

};
struct event *get_mw_event(void);

/*****************************************************/
//GPIO �ӿ�//gpio_num: ��ǰgpio��cpu��gpio�е�˳���ţ����磺����PB(10) = 1 * 32 + 10 = 42
#define GPIO_LOW  0
#define GPIO_HIGH 1
#define GPIO_IN   0
#define GPIO_OUT  1

int mw_gpio_apply (int gpio_num);	//�Ѿ��������gpio��sysfs gpio�ٴ������ʧ�ܡ�
//int mw_gpio_probe (int gpio_num, int default_level, int direct);
int mw_gpio_probe (int gpio_num, int direct, int initValue);
int mw_gpio_set (int gpio_num, int value);
int mw_gpio_get (int gpio_num, int *value);
int mw_gpio_release (int gpio_num);


/* protocal module*/
/*****************************************************/
struct i2c_dev {
	int dev_num;						//����i2c��������, 0 or 1
	int bit_rate;						//����Ƶ��
	int reg_bit;						//�豸�Ĵ���bit����7 or 9

	int (*init) (struct i2c_dev *dev);

	int (*open) (struct i2c_dev *dev);
	int (*read) (struct i2c_dev *dev, int addr, short reg);
	int (*write) (struct i2c_dev *dev, int addr, short reg, char value);
	int (*close) (struct i2c_dev *dev);

};
struct i2c_dev *get_mw_i2c_dev(void);

struct uart_dev {
	int dev_num;						//����uart��������, 0 or 1
	int rate;							//����Ƶ��
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
	int enabel;							//ʹ�ܱ�ģ��

	int stat_wifi;
	int stat_sensor;
	int stat_audio;
	int stat_video;
	int stat_echo;

	int (*set) (struct pm_fun *pm, int flag);
	int (*get) (struct pm_fun *pm);		//��ȡ��ģ�鵱ǰʹ��״̬

	/* �ϲ��ڵ��������ӿ�ǰ�������Ƚ����й����ݱ���ʹ���������*/
	int (*suspend) (struct pm_fun *pm);		//���ú��豸��������������,���������˳���˵��ϵͳ�Ѿ�����.		
	/*note: ִ������ʱ�����Զ����й�ģ����жϵ�������Խ��͹��ģ��ʵ���ǰ�豣������*/
	int (*reboot) (struct pm_fun *pm);							
	int (*shutdown) (struct pm_fun *pm);	//ϵͳ�ػ�

	/*  ���Ѻ��ϲ�����øýӿڲ鿴ָ��ģ���Ƿ���� */
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


