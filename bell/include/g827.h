#ifndef __G827_H__
#define __G827_H__

#define	DEFAULT_UUID		"GBELL-000000-NEJSE"

//#define	PUBLISH_MONTH	7
#define	PUBLISH_MONTH	8

#define MAJOR_VERSION 20
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
#undef CUSTOMER_ID
#define CUSTOMER_ID 136
#undef LANGUAGE_ID
#define LANGUAGE_ID 811

//------------------------------------
//呼叫状态下再次呼叫， 结束上一次呼叫，并通知所有用户无人接听----通知码改为12
#undef CUSTOMER_ID
#define CUSTOMER_ID 137
#undef LANGUAGE_ID
#define LANGUAGE_ID 814

//------------------------------------
//推送去掉角标
//默认撤防
//刷新wifi改为直接读取之前的数据
//添加扩展接口
#undef CUSTOMER_ID
#define CUSTOMER_ID 138
#undef LANGUAGE_ID
#define LANGUAGE_ID 817

//------------------------------------
//音频改为PCM
#undef CUSTOMER_ID
#define CUSTOMER_ID 139
#undef LANGUAGE_ID
#define LANGUAGE_ID 819

//------------------------------------
//音频改为PCM
//有线无线有网切换，获取IP地址不正确
//打开IRCUT
#undef CUSTOMER_ID
#define CUSTOMER_ID 140
#undef LANGUAGE_ID
#define LANGUAGE_ID 822


//防拆报警触发方式改为60s低电平+2s高电平触发
//处理开机之后会触发doorkey开门的问题
//从eeprom读取ID号失败， 30s后重启
//发送防拆报警类型错误的问题
#undef CUSTOMER_ID
#define CUSTOMER_ID 141
#undef LANGUAGE_ID
#define LANGUAGE_ID 824


//推送改为生产模式
#undef CUSTOMER_ID
#define CUSTOMER_ID 142
#undef LANGUAGE_ID
#define LANGUAGE_ID 828

//PIR添加滞留逻辑
#undef CUSTOMER_ID
#define CUSTOMER_ID 143
#undef LANGUAGE_ID
#define LANGUAGE_ID 830

//取消重启按键
//PIR拍照不及时
//添加守护脚本
//ali,xinge获取服务器地址开线程获取
#undef CUSTOMER_ID
#define CUSTOMER_ID 144
#undef LANGUAGE_ID
#define LANGUAGE_ID 914

//增加SD卡相关功能，增加邮件，增加微信功能
#undef CUSTOMER_ID
#define CUSTOMER_ID 145
#undef LANGUAGE_ID
#define LANGUAGE_ID 1019


//邮件增加ssl加密,压缩图片内存泄露
#undef CUSTOMER_ID
#define CUSTOMER_ID 146
#undef LANGUAGE_ID
#define LANGUAGE_ID 1020


//修改获取大图的接口，改为数组
//修改获取更新SD卡事件更新数量的相关接口，资源没准备好--图片，视频，事件未处理完， 不算在内
#undef CUSTOMER_ID
#define CUSTOMER_ID 147
#undef LANGUAGE_ID
#define LANGUAGE_ID 1024

//录像的时候关掉之前的录像
#undef CUSTOMER_ID
#define CUSTOMER_ID 148
#undef LANGUAGE_ID
#define LANGUAGE_ID 1027


//格式化SD卡加延时
//重写system函数
#undef CUSTOMER_ID
#define CUSTOMER_ID 149
#undef LANGUAGE_ID
#define LANGUAGE_ID 1031


//拍照间隔改为2S
//录像不录音频
#undef CUSTOMER_ID
#define CUSTOMER_ID 150
#undef LANGUAGE_ID
#define LANGUAGE_ID 1101

//增加一键配置停止功能
//处理一键配置超时之后，联网慢的问题
#undef CUSTOMER_ID
#define CUSTOMER_ID 151
#undef LANGUAGE_ID
#define LANGUAGE_ID 1102

//优化联网速度，网卡驱动去掉打印
//解决视频丢帧问题---只丢最后帧(解决MP4开始灰屏问题)
//SD卡视频录像每次只处理一个，停掉上一个再开始新的(解决MP4开始灰屏问题)
#undef CUSTOMER_ID
#define CUSTOMER_ID 152
#undef LANGUAGE_ID
#define LANGUAGE_ID 1107


//修改内容：
//一键配置相关修改(此次修改APP与daemon均作了相应的修改)
//1.一键配置时长改为3分钟  
//2.一键配置成功并且有网之后才会播放提示音，配置失败直接播放提示音
//3.一键配置结束后，返回给手机APP的搜索标志不对

#undef CUSTOMER_ID
#define CUSTOMER_ID 153
#undef LANGUAGE_ID
#define LANGUAGE_ID 102




#define ACTOP
//#define YIJIA

#ifdef ACTOP

#undef MINOR_VERSION
#define MINOR_VERSION 6
#undef CUSTOMER_ID
#define CUSTOMER_ID 153
#undef LANGUAGE_ID
#define LANGUAGE_ID 125

//有线模式下搜不到wifi热点
#undef MINOR_VERSION
#define MINOR_VERSION 6
#undef CUSTOMER_ID
#define CUSTOMER_ID 154
#undef LANGUAGE_ID
#define LANGUAGE_ID 331

#endif





#ifdef YIJIA
//修改内容：
//增加移动侦测功能
#define MOVE_DETECT_ENABLE
#undef MINOR_VERSION
#define MINOR_VERSION 1
#undef CUSTOMER_ID
#define CUSTOMER_ID 154
#undef LANGUAGE_ID
#define LANGUAGE_ID 115


//没有任何修改
#define MOVE_DETECT_ENABLE
#undef MINOR_VERSION
#define MINOR_VERSION 1
#undef CUSTOMER_ID
#define CUSTOMER_ID 155
#undef LANGUAGE_ID
#define LANGUAGE_ID 307

//有线模式下搜不到wifi热点
#define MOVE_DETECT_ENABLE
#undef MINOR_VERSION
#define MINOR_VERSION 1
#undef CUSTOMER_ID
#define CUSTOMER_ID 156
#undef LANGUAGE_ID
#define LANGUAGE_ID 331

#endif



#endif

