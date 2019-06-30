#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory.h>
#include <unistd.h>
#include <dirent.h>
#include <dirent.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#include "common.h"
#include "../../protocol/json/cjson.h"

#define NON_NUM '0'

#include "msg_send.c"
static int G_msgid = -1;


unsigned short  		logcnt = 0;
char					snapflag = 0x00;
char					cameraflag = 0x00;
char					updateflag = 0x00;
char					dbgflag = 0x00;
char					wifiscanflag = 0x00;
sem_t				wifiscansem;

void web_dbg1( char* pbuf, int cmd )
{
    FILE* fp = NULL;

    if ( cmd == 0x00 )
    {
        fp = fopen( "/tmp/dbg.txt", "ab+" );

        if ( fp == NULL )
        {
            printf( "dbg file open failed\n" );
            return;
        }
    }

    else
    {
        fp = fopen( "/tmp/dbg.txt", "wb" );

        if ( fp == NULL )
        {
            printf( "dbg file open failed\n" );
            return;
        }
    }

    fwrite( pbuf, 1, strlen( pbuf ), fp );
    fclose( fp );
}

int CgiCommand( char* pcmd )
{
    char* pdst = NULL;
    DEBUG_IMPORTANT_INFO( "---main---%s,%d,%s---IE Command, cmd len=%d, cmd[%s]", __FILE__, __LINE__, __func__, strlen(pcmd), pcmd);

    pdst = strstr( pcmd, "openlock.cgi" );

    if ( pdst != NULL )
    {
		DEBUG_INFO("---main---%s,%d,%s---Return CGI_IEGET_OPENLOCK", __FILE__, __LINE__, __func__);
        return -1;
    }


    return 0;
}

//int  CgiProcess( unsigned short cgicmd, char* url, char* pbuf, int len, char auth, unsigned char pri )
int  CgiProcess( unsigned short cgicmd, char* url, char* pbuf, int index)
{
	char   			temp[2048];
	char			temp1[2048];
    char            temp2[64];
    int             iRet = 0;
    char            nexturl[64];
    char			decoderbuf[128];

	if (index != 0) {
		return 0;
	}


	int nResult = 0;

    cJSON *pContent = NULL;
    cJSON *pItem = NULL;

    //Set Reponse JSON
    ////////////////////////////////////////////////////
    cJSON *pJsonRoot = NULL;
    pJsonRoot = cJSON_CreateObject();
    if (NULL == pJsonRoot) {
        //error happend here
        return -1;
    }

	if (G_msgid == -1) {
		G_msgid = msg_init();
	}
	
	printf("=====================url = %s\n", url);
	
    //cJSON_AddStringToObject(pJsonRoot, "ret", "false");
	memset(temp, 0, sizeof(temp));
	memset(temp1, 0, sizeof(temp1));
	sprintf(temp1, "no use");
	//ascToUni(temp1, temp, strlen(temp));
	nResult = msg_send(G_msgid, url, strlen(url));

	if (nResult == 0) {
		cJSON_AddTrueToObject(pJsonRoot, "ret");
		cJSON_AddStringToObject(pJsonRoot, "msg", "success");    //Success
	} else {
		cJSON_AddFalseToObject(pJsonRoot, "ret");
		cJSON_AddStringToObject(pJsonRoot, "msg", "fail");    //Success		
	}
	
    char *p = cJSON_PrintUnformatted(pJsonRoot);   //cJSON_PrintUnformatted
    if (NULL == p) {
        //convert json list to string faild, exit
        //because sub json pSubJson han been add to pJsonRoot, so just delete pJsonRoot, if you also delete pSubJson, it will coredump, and error is : double free
        cJSON_Delete(pJsonRoot);
        return -1;
    }
	
    //strcpy(szResponse, p);
	iRet += sprintf( temp + iRet, p);
	//iRet += sprintf( temp + iRet, "today is friday" );
    memcpy( pbuf, temp, iRet );
    free(p);

    cJSON_Delete(pJsonRoot);
    	
    //iRet += sprintf( temp + iRet, "{\"ret\":false, \"msg\":\"错误信息\"}" );
	//iRet += sprintf( temp + iRet, "today is friday" );
    //memcpy( pbuf, temp, iRet );
	
    return iRet;
}


