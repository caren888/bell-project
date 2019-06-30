#ifndef __MAIL_H__
#define __MAIL_H__

typedef struct {
	 char szpic1[128];
	 char szpic2[128];
	 char szpic3[128];
}sZPIC;

//int xEmailInit(int bEnable,const char *szEmailBox1, const char *szEmailBox2);
int xEmailInit(int bEnable,const char *szEmailBox1, const char *szEmailBox2,const char *szSenderEmail, const char *szSenderPwd);
int xEmailSend(const char *szSubject, const char *szJpegFiles);

#endif
