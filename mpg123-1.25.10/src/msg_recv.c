#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>

/*
 播放 - play:
 http://ip:8080/play?play_file_name=
 暂停 - pause:
 http://ip:8080/pause, 再次请求暂停会继续播放
 停止 - stop:
 http://ip:8080/stop
 播放演示 - play-demo:
 http://ip:8080/play_demo
 设置音量 - set-volume:
 http://ip:8080/set_volume?player_volume=
 状态 - status:
 http://ip:8080/status
 */

#define MAX_MSG_LEN	512

struct msg_st
{
	long int msg_type;
	char data[MAX_MSG_LEN];
};

static char G_fileName[256];

int clear_fileName(void)
{
    memset(G_fileName, 0, sizeof(G_fileName));
    
    return 0;
}

int set_fileName(char *fileName)
{
    memset(G_fileName, 0, sizeof(G_fileName));
    
    if (fileName == NULL) {
        return -1;
    }
    
    if (strlen(fileName) >= sizeof(G_fileName)) {
        return -1;
    }
    
    sprintf(G_fileName, "%s", fileName);

    return 0;
}

int get_fileName(char *fileName)
{
    sprintf(fileName, "%s", G_fileName);
    
    return 0;
}

char parse_msg(char *buffer)
{
    char *ptr = NULL, *ptr1 = NULL;
    char changeBuffer[512];
    int ret;
    
    if (ptr = strstr(buffer, "play?play_file_name=")) {
        if (strstr(buffer+strlen("play?play_file_name="), "http:") == (ptr+strlen("play?play_file_name="))) {
            sprintf(changeBuffer, "http:/%s", ptr+strlen("play?play_file_name=")+strlen("http:"));
            set_fileName(changeBuffer);
        } else if (strstr(buffer+strlen("play?play_file_name="), "https:") == (ptr+strlen("play?play_file_name="))) {
            sprintf(changeBuffer, "https:/%s", ptr+strlen("play?play_file_name=")+strlen("https:"));
            set_fileName(changeBuffer);
        } else {
            set_fileName(ptr + strlen("play?play_file_name="));
        }
        return MPG123_NEXT_KEY;
    }
    
    if (ptr = strstr(buffer, "pause")) {
        return MPG123_STOP_KEY;
    }
    
    if (ptr = strstr(buffer, "stop")) {
        clear_fileName();
        return MPG123_QUIT_KEY;
    }
    
    if (ptr = strstr(buffer, "play_demo")) {
        return 0;
    }
    
    if (ptr = strstr(buffer, "set_volume?player_volume=")) {
        return MPG123_VOL_UP_KEY;
        //return MPG123_VOL_DOWN_KEY;
    }
    
    if (ptr = strstr(buffer, "status")) {
        return 0;
    }
    
    return 0;
}

int msg_recv(int msgid, char *buffer, int *len)
{
    int recvLen;
    struct msg_st data;
    long int msgtype = 0;
    
    if (buffer == NULL || len==NULL) {
        return -1;
    }
    
    memset(&data, 0, sizeof(struct msg_st));
    if((recvLen = msgrcv(msgid, (void*)&data, MAX_MSG_LEN, msgtype, IPC_NOWAIT)) == -1)
    {
        //fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
        return -1;
    } else {
        printf("recv buffer len = %d, is %s\n", recvLen, data.data);
    }
    
    if (recvLen > *len) {
        fprintf(stderr, "msgrcv buffer is not enough \n");
        return -1;
    }
    
    memcpy(buffer, data.data, recvLen);
    *len = recvLen;
    
    return 0;
}

int msg_uninit(int msgid)
{
    //删除消息队列
    if(msgctl(msgid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        return -1;
    }
    
    return 0;
}

int msg_init(void)
{
    int msgid = -1;
    
    //建立消息队列
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if(msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        return -1;
    }
    
    return msgid;
}
