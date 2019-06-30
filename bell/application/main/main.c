#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>  
#include <signal.h>

#include "../../include/common.h"
#include "../../include/middleware.h"
#include "../audio/my_audio.h"
#include "web.h"

int main(int argc, char *argv[])
{
	int iRet;
	char firstBoot = 0;

    signal( SIGPIPE, SIG_IGN );

    system("echo 63 > /sys/class/gpio/export");
    system("echo out > /sys/class/gpio/gpio63/direction");
    usleep(100 * 1000);
    system("echo 1 > /sys/class/gpio/gpio63/value");

#if 0    
	iRet = middleware_init();
	if (iRet != ERROR_SUCCESS) {
		DEBUG_ERR("---main---%15s,%5d,%15s---middleware_init failed", __FILE__, __LINE__, __func__);
		return -1;
	} else {
		DEBUG_IMPORTANT_INFO("---main---%15s,%5d,%15s---middleware_init success", __FILE__, __LINE__, __func__);
	}
	
				
	iRet = my_audio_init();
	if (iRet != ERROR_SUCCESS) {
		DEBUG_ERR("---main---%15s,%5d,%15s---my_audio_init failed",__FILE__, __LINE__, __func__ );
		return -4;
	} else {
		DEBUG_IMPORTANT_INFO("---main---%15s,%5d,%15s---my_audio_init success", __FILE__, __LINE__, __func__);
	}
#endif

	WebThread();
	
	while (1) {
//		StartPlayAlarmAudio(WF_CONFIGOK, NULL);	
		sleep(60);
	}
	return 0;
}
