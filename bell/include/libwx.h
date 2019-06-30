#ifndef __WX__H__
#define __WX__H__

int wxStart(char *szDeviceID, char *szRegCode, char *szWxServerAddress, int nListenPort, void (*OnRecvCallBack)(int nMessage, int wParam, int lParam));
int wxPush(int nMessage, char *szUserName, char *sImageData, int nImageDataSize);

#endif