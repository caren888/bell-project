#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>


#define MAX_MSG_LEN 512

struct msg_st
{
	long int msg_type;
	char data[MAX_MSG_LEN];
};

int msg_send(int msgid, char *buffer, int len)
{
    struct msg_st data;
    
    if (buffer==NULL || len>MAX_MSG_LEN) {
        return -1;
    }
    
    memset(&data, 0, sizeof(struct msg_st));
    data.msg_type = 1;
    memcpy(data.data, buffer, len);

    //向队列发送数据
    if(msgsnd(msgid, (void*)&data, len, 0) == -1)
    {
        fprintf(stderr, "msgsnd failed %s\n",strerror(errno));
        return -1;
    }
    
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
