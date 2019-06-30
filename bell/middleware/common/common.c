#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/wait.h>    
#include <fcntl.h> 

#include "../../include/common.h"

static pthread_mutex_t mutexPrint = PTHREAD_MUTEX_INITIALIZER;

static pid_t    *childpid = NULL;  
                        /* ptr to array allocated at run-time */  
static int      maxfd;  /* from our open_max(), {Prog openmax} */  
  
#define SHELL   "/bin/sh"   

static long openmax = 0;  
//#endif  
   
/* 
 * If OPEN_MAX is indeterminate, we're not 
 * guaranteed that this is adequate. 
 */  
#define OPEN_MAX_GUESS 1024  

void append_pthread_pid(char *pthreadName)
{

/*	
	int fd, pid;

	char fileName[128] = {0};

	pid = (int)syscall(4222);

	sprintf(fileName, "/tmp/%d", pid);
	fd = open(fileName, O_RDWR | O_TRUNC | O_CREAT, 0666);
	if (fd == -1) {
		return;
	}

	write(fd, pthreadName, strlen(pthreadName));

	close(fd);
*/

	return;
}

long my_open_max(void)  
{  
    if (openmax == 0) {      /* first time through */  
        errno = 0;  
        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {  
           if (errno == 0)  
               openmax = OPEN_MAX_GUESS;    /* it's indeterminate */  
           else  
               printf("sysconf error for _SC_OPEN_MAX");  
        }  
    }  
   
    return(openmax);  
}  

FILE *my_popen(const char *cmdstring, const char *type)  
{  
    int     i, pfd[2];  
    pid_t   pid;  
    FILE    *fp;  
  
            /* only allow "r" or "w" */  
    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {  
        errno = EINVAL;     /* required by POSIX.2 */  
        return(NULL);  
    }  
  
    if (childpid == NULL) {     /* first time through */  
                /* allocate zeroed out array for child pids */  
        maxfd = my_open_max();  
        if ( (childpid = calloc(maxfd, sizeof(pid_t))) == NULL) {
			DEBUG_ERR("---main---%s,%d,%s---calloc failed", __FILE__, __LINE__, __func__);			
            return(NULL);
        }
    }  

	//DEBUG_INFO("---main---%s,%d,%s---maxfd = %d", __FILE__, __LINE__, __func__, maxfd);			
  
    if (pipe(pfd) < 0)  
        return(NULL);   /* errno set by pipe() */  
  
    if ( (pid = vfork()) < 0) {
		DEBUG_ERR("---main---%s,%d,%s---fork failed", __FILE__, __LINE__, __func__);
        return(NULL);   /* errno set by fork() */  
    }
    else if (pid == 0) {                            /* child */  
        if (*type == 'r') {  
            close(pfd[0]);  
            if (pfd[1] != STDOUT_FILENO) {  
                dup2(pfd[1], STDOUT_FILENO);  
                close(pfd[1]);  
            }  
        } else {  
            close(pfd[1]);  
            if (pfd[0] != STDIN_FILENO) {  
                dup2(pfd[0], STDIN_FILENO);  
                close(pfd[0]);  
            }  
        }  
            /* close all descriptors in childpid[] */  
        for (i = 0; i < maxfd; i++)  
            if (childpid[ i ] > 0)  
                close(i);  
		  
        execl(SHELL, "sh", "-c", cmdstring, (char *) 0);
        _exit(127);  
    }  
                                /* parent */  
    if (*type == 'r') {  
        close(pfd[1]);  
        if ( (fp = fdopen(pfd[0], type)) == NULL)  
            return(NULL);  
    } else {  
        close(pfd[0]);  
        if ( (fp = fdopen(pfd[1], type)) == NULL)  
            return(NULL);  
    }  
    childpid[fileno(fp)] = pid; /* remember child pid for this fd */  
    return(fp);  
}  
  
int my_pclose(FILE *fp)  
{  
  	int iRet;
    int     fd, stat;  
    pid_t   pid;  
  
    if (childpid == NULL)  
        return(-1);     /* popen() has never been called */  
  
    fd = fileno(fp);  
    if ( (pid = childpid[fd]) == 0)  
        return(-1);     /* fp wasn't opened by popen() */  
  
    childpid[fd] = 0;  
    if (fclose(fp) == EOF)  
        return(-1);  

	//DEBUG_IMPORTANT_INFO("===============begin waitpid");
    while ((iRet = waitpid(pid, &stat, 0)) < 0)  {
        if (errno != EINTR)  
            return(-1); /* error other than EINTR from waitpid() */  
		//DEBUG_IMPORTANT_INFO("===============iRet = %d, errno = %d, %s", iRet, errno, strerror(errno));
    }
  
    return(stat);   /* return child's termination status */  
}  



/*
int my_system(char *cmd)
{
	int ret;
	FILE *fp = NULL;

	if (cmd == NULL) {
		DEBUG_IMPORTANT_INFO("---main---%s,%d,%s---my_system cmd is null, %s", __FILE__, __LINE__, __func__, strerror(errno));
		return ERROR_FALSE;
	}

	DEBUG_IMPORTANT_INFO("---main---%s,%d,%s---begin my_system write cmd is %s, errno is %s", __FILE__, __LINE__, __func__, cmd, strerror(errno));

	fp = my_popen("sh", "r");
	if (fp == NULL) {
		DEBUG_IMPORTANT_INFO("---main---%s,%d,%s---my_system popen failed, %s", __FILE__, __LINE__, __func__, strerror(errno));
		return ERROR_FALSE;
	}

	ret = my_pclose(fp);

	DEBUG_IMPORTANT_INFO("---main---%s,%d,%s---end my_system write cmd is %s, errno is %s", __FILE__, __LINE__, __func__, cmd, strerror(errno));
	
	return ret;
}
*/

int my_system(char *cmd)
{
	int pid = 0;
	int status = 0;
	char *argv[4];
	extern char **environ;
	
	if (NULL == cmd) {
		return -1;
	}

	//DEBUG_IMPORTANT_INFO("---main---%s,%d,%s---start my_system write cmd is %s, errno is %s", __FILE__, __LINE__, __func__, cmd, strerror(errno));
	
	pid = vfork();		/* vfork() also works */
	if (pid < 0) {
		DEBUG_IMPORTANT_INFO("---main---%s,%d,%s---vfork is failed, %s", __FILE__, __LINE__, __func__, strerror(errno));
		return -1;
	}
	if (0 == pid) { 			/* child process */
		//_close_all_fds();		/* 这是我自己写的一个函数，用来关闭所有继承的文件描述符。可不用 */
		argv[0] = "sh";
		argv[1] = "-c";
		argv[2] = cmd;
		argv[3] = NULL;
		
		execve ("/bin/sh", argv, environ);	  /* execve() also an implementation of exec() */
		_exit (127);
	}
	
	// else
	/* wait for child process to start */
	do
	{
		if (waitpid (pid, &status, 0) < 0) {
			if (errno != EINTR) {
				return -1;
			}
			else {
				return status;
			}
		}
	} while (1);

	DEBUG_IMPORTANT_INFO("---main---%s,%d,%s---end my_system write cmd is %s, errno is %s", __FILE__, __LINE__, __func__, cmd, strerror(errno));
	
	return 0;
}


/*
int my_system(char *cmd)
{
	int ret;
	FILE *fp = NULL;

	if (cmd == NULL) {
		DEBUG_IMPORTANT_INFO("---main---%s,%d,%s---my_system cmd is null, %s", __FILE__, __LINE__, __func__, strerror(errno));
		return ERROR_FALSE;
	}

	fp = my_popen("sh", "w");
	if (fp == NULL) {
		DEBUG_IMPORTANT_INFO("---main---%s,%d,%s---my_system popen failed, %s", __FILE__, __LINE__, __func__, strerror(errno));
		return ERROR_FALSE;
	}

	DEBUG_IMPORTANT_INFO("---main---%s,%d,%s---begin my_system write cmd is %s, errno is %s", __FILE__, __LINE__, __func__, cmd, strerror(errno));
	ret = fwrite(cmd, 1, strlen(cmd), fp);
	DEBUG_IMPORTANT_INFO("---main---%s,%d,%s---write result = %d, %s", __FILE__, __LINE__, __func__, ret, strerror(errno));

	ret = my_pclose(fp);
	
	DEBUG_IMPORTANT_INFO("---main---%s,%d,%s---end my_system write cmd is %s, errno is %s", __FILE__, __LINE__, __func__, cmd, strerror(errno));
	
	return ret;
}
*/

int strcmpEx( const char *str1, const char *str2 )
{
	int i = 0;
	char s1[32] = {0};
	char s2[32] = {0};
	
	if ( str1[0] != 0 ) {
		strcpy(s1, str1);
	}
	
	if ( str2[0] != 0 ) {
		strcpy(s2, str2);
	}
	
	for (i = 0; i < 32; i++) {
		if ( s1[i] >= 'a' && s1[i] <= 'z' ) {
			s1[i] = s1[i] - 'a' + 'A';
		}
		if ( s1[i] == 0 ) {
			break;
		}
	}
	
	for (i = 0; i < 32; i++) {
		if ( s2[i] >= 'a' && s2[i] <= 'z' ) {
			s2[i] = s2[i] - 'a' + 'A';
		}
		if ( s2[i] == 0 ) {
			break;
		}
	}
	
	return strcmp(s1, s2);
}

void print_to_remote(char *p)
{
	FILE  *fp = NULL;

	fp = fopen("/dev/pts/0", "w");
	if (fp != NULL) {
		fwrite(p, 1, strlen(p), fp);
		fclose(fp);
	}
}

void HexToStr(char *pbDest, char *pbSrc, int nLen)
{
	char	ddl,ddh;
	int i;

	for (i=0; i<nLen; i++) {
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] = ddl;
	}

	pbDest[nLen*2] = '\0';
}


void lock_print(void)
{
	pthread_mutex_lock(&mutexPrint);
}

void unlock_print(void)
{
	pthread_mutex_unlock(&mutexPrint);
}


void DEBUG_TEST(const char *format, ...)
{
#ifndef PRINTFDEBUGTEST
		return;
#endif	
	
	char fmtBuf[256];
	va_list ap;
	
	sprintf(fmtBuf, "\033[40;32m---%s\033[0m\n", format);	
	va_start(ap, format);    
	vprintf(fmtBuf, ap);    
	va_end(ap);

	return;
}

void DEBUG_ERR(const char *format, ...)
{
#ifndef PRINT_ERR
			return;
#endif	
	
	char fmtBuf[256];
	va_list ap;

	sprintf(fmtBuf, "\033[40;31m---%s\033[0m\n", format);	
	va_start(ap, format);    
	vprintf(fmtBuf, ap);    
	va_end(ap);

	return;
}

void DEBUG_INFO(const char *format, ...)
{
#ifndef PRINT_NORMAL_INFO
		return;
#endif	
	
	char fmtBuf[256];
	va_list ap;
	
	sprintf(fmtBuf, "\033[40;32m---%s\033[0m\n", format);	
	va_start(ap, format);    
	vprintf(fmtBuf, ap);    
	va_end(ap);

	return;
}

void DEBUG_IMPORTANT_INFO(const char *format, ...)
{
#ifndef PRINT_IMPORTANT_INFO
	return;
#endif
	
	char fmtBuf[256];
	va_list ap;
	
	sprintf(fmtBuf, "\033[40;35m---%s\033[0m\n", format);	
	va_start(ap, format);    
	vprintf(fmtBuf, ap);    
	va_end(ap);

	return;
}

/*
int main(void)
{
	printf("-----test my_system-----\n");

	char command[256];

	sprintf(command, "blkid /dev/mmcblk0p1 > /tmp/blkid_result");	

	while (1) {
		my_system(command);
//		sleep(1);
	}

	return 0;
}
*/
