#ifndef __YIDUIYI_H__
#define __YIDUIYI_H__

#define	DEFAULT_UUID		"GBELL-000000-NEJSE"

#define	PUBLISH_MONTH	7

#define MAJOR_VERSION 17
#define MINOR_VERSION 1
#define CUSTOMER_ID 131
#define LANGUAGE_ID 606


#undef CUSTOMER_ID
#define CUSTOMER_ID 132
#undef LANGUAGE_ID
#define LANGUAGE_ID 721

//------------------------------------
//取消打印
//修改用户管理---改为管理员和游客
//免打扰
//设置wifi,扫描wifi
//布撤防
//音量设置
//固件升级
#undef CUSTOMER_ID
#define CUSTOMER_ID 133
#undef LANGUAGE_ID
#define LANGUAGE_ID 731


//------------------------------------
//在上一版本基础上添加一键配置失败语音
#undef CUSTOMER_ID
#define CUSTOMER_ID 134
#undef LANGUAGE_ID
#define LANGUAGE_ID 801

//------------------------------------
//wifi名中文支持
#undef CUSTOMER_ID
#define CUSTOMER_ID 135
#undef LANGUAGE_ID
#define LANGUAGE_ID 804

//------------------------------------
//呼叫状态下再次呼叫， 结束上一次呼叫，并通知所有用户无人接听
//声音封顶做了限制
//优化8189ES一键配置功能
#undef CUSTOMER_ID
#define CUSTOMER_ID 136
#undef LANGUAGE_ID
#define LANGUAGE_ID 811

//------------------------------------
//修改语言包
#undef CUSTOMER_ID
#define CUSTOMER_ID 137
#undef LANGUAGE_ID
#define LANGUAGE_ID 812

//------------------------------------
//signal_sw控制错误的问题
#undef CUSTOMER_ID
#define CUSTOMER_ID 138
#undef LANGUAGE_ID
#define LANGUAGE_ID 826

//------------------------------------
//修改呼叫，报警获取图片，改为固定图片call.jpg
//修改视频供电逻辑， 确保获取视频的过程中不会断电
#undef CUSTOMER_ID
#define CUSTOMER_ID 139
#undef LANGUAGE_ID
#define LANGUAGE_ID 904

//------------------------------------
//帧间隔改为15
//码率改为1M
#undef CUSTOMER_ID
#define CUSTOMER_ID 140
#undef LANGUAGE_ID
#define LANGUAGE_ID 905

//------------------------------------
//关闭video0
#undef CUSTOMER_ID
#define CUSTOMER_ID 141
#undef LANGUAGE_ID
#define LANGUAGE_ID 912

#endif

