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
//ȡ����ӡ
//�޸��û�����---��Ϊ����Ա���ο�
//�����
//����wifi,ɨ��wifi
//������
//��������
//�̼�����
#undef CUSTOMER_ID
#define CUSTOMER_ID 133
#undef LANGUAGE_ID
#define LANGUAGE_ID 731


//------------------------------------
//����һ�汾���������һ������ʧ������
#undef CUSTOMER_ID
#define CUSTOMER_ID 134
#undef LANGUAGE_ID
#define LANGUAGE_ID 801

//------------------------------------
//wifi������֧��
#undef CUSTOMER_ID
#define CUSTOMER_ID 135
#undef LANGUAGE_ID
#define LANGUAGE_ID 804

//------------------------------------
//����״̬���ٴκ��У� ������һ�κ��У���֪ͨ�����û����˽���
//�����ⶥ��������
#undef CUSTOMER_ID
#define CUSTOMER_ID 136
#undef LANGUAGE_ID
#define LANGUAGE_ID 811

//------------------------------------
//����״̬���ٴκ��У� ������һ�κ��У���֪ͨ�����û����˽���----֪ͨ���Ϊ12
#undef CUSTOMER_ID
#define CUSTOMER_ID 137
#undef LANGUAGE_ID
#define LANGUAGE_ID 814

//------------------------------------
//����ȥ���Ǳ�
//Ĭ�ϳ���
//ˢ��wifi��Ϊֱ�Ӷ�ȡ֮ǰ������
//�����չ�ӿ�
#undef CUSTOMER_ID
#define CUSTOMER_ID 138
#undef LANGUAGE_ID
#define LANGUAGE_ID 817

//------------------------------------
//��Ƶ��ΪPCM
#undef CUSTOMER_ID
#define CUSTOMER_ID 139
#undef LANGUAGE_ID
#define LANGUAGE_ID 819

//------------------------------------
//��Ƶ��ΪPCM
//�������������л�����ȡIP��ַ����ȷ
//��IRCUT
#undef CUSTOMER_ID
#define CUSTOMER_ID 140
#undef LANGUAGE_ID
#define LANGUAGE_ID 822


//���𱨾�������ʽ��Ϊ60s�͵�ƽ+2s�ߵ�ƽ����
//������֮��ᴥ��doorkey���ŵ�����
//��eeprom��ȡID��ʧ�ܣ� 30s������
//���ͷ��𱨾����ʹ��������
#undef CUSTOMER_ID
#define CUSTOMER_ID 141
#undef LANGUAGE_ID
#define LANGUAGE_ID 824


//���͸�Ϊ����ģʽ
#undef CUSTOMER_ID
#define CUSTOMER_ID 142
#undef LANGUAGE_ID
#define LANGUAGE_ID 828

//PIR��������߼�
#undef CUSTOMER_ID
#define CUSTOMER_ID 143
#undef LANGUAGE_ID
#define LANGUAGE_ID 830

//ȡ����������
//PIR���ղ���ʱ
//����ػ��ű�
//ali,xinge��ȡ��������ַ���̻߳�ȡ
#undef CUSTOMER_ID
#define CUSTOMER_ID 144
#undef LANGUAGE_ID
#define LANGUAGE_ID 914

//����SD����ع��ܣ������ʼ�������΢�Ź���
#undef CUSTOMER_ID
#define CUSTOMER_ID 145
#undef LANGUAGE_ID
#define LANGUAGE_ID 1019


//�ʼ�����ssl����,ѹ��ͼƬ�ڴ�й¶
#undef CUSTOMER_ID
#define CUSTOMER_ID 146
#undef LANGUAGE_ID
#define LANGUAGE_ID 1020


//�޸Ļ�ȡ��ͼ�Ľӿڣ���Ϊ����
//�޸Ļ�ȡ����SD���¼�������������ؽӿڣ���Դû׼����--ͼƬ����Ƶ���¼�δ�����꣬ ��������
#undef CUSTOMER_ID
#define CUSTOMER_ID 147
#undef LANGUAGE_ID
#define LANGUAGE_ID 1024

//¼���ʱ��ص�֮ǰ��¼��
#undef CUSTOMER_ID
#define CUSTOMER_ID 148
#undef LANGUAGE_ID
#define LANGUAGE_ID 1027


//��ʽ��SD������ʱ
//��дsystem����
#undef CUSTOMER_ID
#define CUSTOMER_ID 149
#undef LANGUAGE_ID
#define LANGUAGE_ID 1031


//���ռ����Ϊ2S
//¼��¼��Ƶ
#undef CUSTOMER_ID
#define CUSTOMER_ID 150
#undef LANGUAGE_ID
#define LANGUAGE_ID 1101

//����һ������ֹͣ����
//����һ�����ó�ʱ֮��������������
#undef CUSTOMER_ID
#define CUSTOMER_ID 151
#undef LANGUAGE_ID
#define LANGUAGE_ID 1102

//�Ż������ٶȣ���������ȥ����ӡ
//�����Ƶ��֡����---ֻ�����֡(���MP4��ʼ��������)
//SD����Ƶ¼��ÿ��ֻ����һ����ͣ����һ���ٿ�ʼ�µ�(���MP4��ʼ��������)
#undef CUSTOMER_ID
#define CUSTOMER_ID 152
#undef LANGUAGE_ID
#define LANGUAGE_ID 1107


//�޸����ݣ�
//һ����������޸�(�˴��޸�APP��daemon��������Ӧ���޸�)
//1.һ������ʱ����Ϊ3����  
//2.һ�����óɹ���������֮��ŻᲥ����ʾ��������ʧ��ֱ�Ӳ�����ʾ��
//3.һ�����ý����󣬷��ظ��ֻ�APP��������־����

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

//����ģʽ���Ѳ���wifi�ȵ�
#undef MINOR_VERSION
#define MINOR_VERSION 6
#undef CUSTOMER_ID
#define CUSTOMER_ID 154
#undef LANGUAGE_ID
#define LANGUAGE_ID 331

#endif





#ifdef YIJIA
//�޸����ݣ�
//�����ƶ���⹦��
#define MOVE_DETECT_ENABLE
#undef MINOR_VERSION
#define MINOR_VERSION 1
#undef CUSTOMER_ID
#define CUSTOMER_ID 154
#undef LANGUAGE_ID
#define LANGUAGE_ID 115


//û���κ��޸�
#define MOVE_DETECT_ENABLE
#undef MINOR_VERSION
#define MINOR_VERSION 1
#undef CUSTOMER_ID
#define CUSTOMER_ID 155
#undef LANGUAGE_ID
#define LANGUAGE_ID 307

//����ģʽ���Ѳ���wifi�ȵ�
#define MOVE_DETECT_ENABLE
#undef MINOR_VERSION
#define MINOR_VERSION 1
#undef CUSTOMER_ID
#define CUSTOMER_ID 156
#undef LANGUAGE_ID
#define LANGUAGE_ID 331

#endif



#endif

