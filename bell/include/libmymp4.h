#ifndef __LIBMYMP4_H__
#define __LIBMYMP4_H__



int xMP4InitAudio(int nSampleRate, int nChannal,int bitsPerSample);
int xMP4InitVideo(int nWidth, int nHeight, int nFrameRate);

int xEventStartRecord(int nChannal,const char * szFileName);
int xEventStopRecord(int nChannal);
int xEventWriteAudio(int nChannal,unsigned char *buf, int nSize);
int xEventWriteVideo(int nChannal,unsigned char *buf, int nSize, int bKeyFrame);



#endif
