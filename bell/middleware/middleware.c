/*
 middleware.h , for g827 project.
 author: xfli.
 date: 09-09-2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>

#include "middleware.h"
#include "audio/mw_audio.h"
#include "common.h"

struct media_a *mw_audio = NULL;


static void mw_free_memory(void)
{

	if(mw_audio)
		free(mw_audio);
	
	return;
}

/***************************************************************/
/* 是否考虑只有当模块已经就绪后才能返回正确值？？*/

struct media_a *get_media_audio(void)
{
	if(mw_audio != NULL) {
//		if(mw_audio->module_stat == MODULE_STANDBY)			//need discuss
			return mw_audio;
	}	
	else
		return NULL;
}


/***************************************************************/
int middleware_init(void)
{
	int ret = -1;

	printf("Start middleware init. \n");
		
	mw_audio = (struct media_a *)malloc(sizeof(struct media_a));
	if(mw_audio != NULL){
		ret = mw_media_audio_open(mw_audio);
	}
//	if (ret != ERROR_SUCCESS) {
//		return ERROR_FALSE;
//	}
	
	return ERROR_SUCCESS;

}

int middleware_uninit(void)
{
	int ret;
	
	printf("Middleware uninit. \n");

	
	ret = mw_media_audio_close(mw_audio);
	

	mw_free_memory();
	
	return ret;
}
