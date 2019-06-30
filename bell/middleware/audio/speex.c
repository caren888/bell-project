
#include <stdio.h>     
#include <string.h>  
  
#include <speex/speex.h>    
 

#include <speex/speex_preprocess.h>    
#include <speex/speex_echo.h>   

static SpeexPreprocessState *m_st;	

#define FRAME_SIZE 160


void speex_preprocess_uninit(void)
{
	speex_preprocess_state_destroy(m_st);	
}

void speex_preprocess_init(void)
{		    	
    int ret;    
    int i=0;
	float f;
    m_st=speex_preprocess_state_init(FRAME_SIZE, 8000);
	
	int denoise = 1;
	int noiseSuppress = -25;
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_DENOISE, &denoise); //降噪
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress); //设置噪声的dB
	i=0;
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_AGC, &i);
	i=8000;
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_AGC_LEVEL, &i);
	i=0;
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_DEREVERB, &i);
	f=.0;
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &f);
	f=.0;
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &f);
	
	int vad = 1; 
	int vadProbStart = 80; 
	int vadProbContinue = 65; 
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_VAD, &vad); //静音检测 
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_PROB_START , &vadProbStart); //Set probability required for the VAD to go from silence to voice 
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_PROB_CONTINUE, &vadProbContinue); //Set probability required for the VAD to stay in the voice state (integer percent) 
        
	return;	
}


void speex_preprocess_init_old(void)
{		    	
    int ret, p_iTemp; 
	float p_fTemp;
    int j=0;    
    m_st=speex_preprocess_state_init(FRAME_SIZE, 8000);

	p_iTemp = 1;
	speex_preprocess_ctl (m_st, SPEEX_PREPROCESS_SET_DENOISE, &p_iTemp);//打开噪音抑制
	p_iTemp = -20;
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &p_iTemp); //设置噪声的dB    
/*	
	p_iTemp = 1;
	speex_preprocess_ctl (m_st, SPEEX_PREPROCESS_SET_DEREVERB, &p_iTemp);//打开混响消除
*/	

	p_iTemp = 1;
	speex_preprocess_ctl (m_st, SPEEX_PREPROCESS_SET_VAD, &p_iTemp);//打开VAD语音活动检测
	p_iTemp = 80;
	speex_preprocess_ctl (m_st, SPEEX_PREPROCESS_SET_PROB_START, &p_iTemp);//设置VAD语音活动检测时，从静音到有声音的判断标准
	p_iTemp = 65;
	speex_preprocess_ctl (m_st, SPEEX_PREPROCESS_SET_PROB_CONTINUE, &p_iTemp);//设置VAD语音活动检测时，从有声音到静音的判断标准


	p_iTemp = 1;
	speex_preprocess_ctl (m_st, SPEEX_PREPROCESS_SET_AGC, &p_iTemp);//打开自动增益控制
	p_fTemp = 24000;
//	p_fTemp = 16000;
	speex_preprocess_ctl (m_st, SPEEX_PREPROCESS_SET_AGC_LEVEL, &p_fTemp);//设置自动增益控制等级

//	speex_preprocess_ctl (m_st, SPEEX_PREPROCESS_SET_ECHO_STATE, g_stSpeexEchoState);//打开残余回音消除

/*
    int denoise = 1;    
    int noiseSuppress = -25;    
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_DENOISE, &denoise); //降噪    
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress); //设置噪声的dB    
            
    int agc = 1;    
    int agcLevel =24000;    
    //actually default is 8000(0,32768),here make it louder for voice is not loudy enough by default. 8000    
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_AGC, &agc);//增益    
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_AGC_LEVEL,&agcLevel);
	
    int vad = 1;    
    int vadProbStart = 80;    
    int vadProbContinue = 65;    
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_VAD, &vad); //静音检测    
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_PROB_START , &vadProbStart); //Set probability required for the VAD to go from silence to voice     
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_PROB_CONTINUE, &vadProbContinue); //Set probability required for the VAD to stay in the voice state (integer percent)     
*/       
	return;	
}


void speex_preprocess_audio(char *inBuffer, int len)
{
	int ret, i, nbBytes;
	
	if (speex_preprocess_run(m_st, (short *)inBuffer))//预处理 打开了静音检测和降噪	 
	{	 
		printf("speech, len = %d\n", len);
//		memcpy(outBuffer, inBuffer, inBufferLen);
	}	 
	else	
	{	 
		printf("slience,\n");
		memset(inBuffer, 0, len);
//		fwrite(out, sizeof(short), FRAME_SIZE, fout3);	  
	}	 
	

	return;
}

/*
int main(int argc, char **argv)    
{    
        
    char *inFile;    
        
    FILE *fin,*fout1,*fout2,*fout3;    
        
    short in[FRAME_SIZE];    
    short out[FRAME_SIZE];      
    float input[FRAME_SIZE];    
    float output[FRAME_SIZE];       
    char cbits[200];    
        
    int nbBytes;    
        
        
        
    void *stateEncode;    
    void *stateDecode;    
        
        
        
        
        
    SpeexBits bitsEncode;    
    SpeexBits bitsDecode;       
    int i, tmp;    
        
    //新建一个新的编码状态在窄宽(narrowband)模式下    
        
    stateEncode = speex_encoder_init(&speex_nb_mode);    
    stateDecode = speex_decoder_init(&speex_nb_mode);    
    //设置质量为8(15kbps)    
        
    tmp=0;    
    speex_encoder_ctl(stateEncode, SPEEX_SET_VBR, &tmp);    
    float q=4;    
    speex_encoder_ctl(stateEncode, SPEEX_SET_VBR_QUALITY, &q);    
    speex_encoder_ctl(stateEncode, SPEEX_SET_QUALITY, &tmp);    
        
    inFile = argv[1];    
        
    fin = fopen("./1c16b8k.pcm", "rb");    
    fout1 = fopen("./demo_speex.raw", "wb");    
    fout2 = fopen("./demo1.pcm", "wb");    
    fout3 = fopen("./demo_slience.pcm", "wb");    
    //初始化结构使他们保存数据    
    
    speex_bits_init(&bitsEncode);    
    speex_bits_init(&bitsDecode);    
    int ret;    
    int j=0;    
    SpeexPreprocessState * m_st;    
    SpeexEchoState *echo_state;     
    m_st=speex_preprocess_state_init(160, 8000);    
//  echo_state = speex_echo_state_init(160, 8000);     
    int denoise = 1;    
    int noiseSuppress = -25;    
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_DENOISE, &denoise); //降噪    
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress); //设置噪声的dB    
    
        
    int agc = 1;    
    q=24000;    
    //actually default is 8000(0,32768),here make it louder for voice is not loudy enough by default. 8000    
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_AGC, &agc);//增益    
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_AGC_LEVEL,&q);    
    int vad = 1;    
    int vadProbStart = 80;    
    int vadProbContinue = 65;    
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_VAD, &vad); //静音检测    
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_PROB_START , &vadProbStart); //Set probability required for the VAD to go from silence to voice     
    speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_PROB_CONTINUE, &vadProbContinue); //Set probability required for the VAD to stay in the voice state (integer percent)     
        
    
    while (1)    
    {    
        memset(out,0,FRAME_SIZE*sizeof(short));    
        //读入一帧16bits的声音    
        j++;    
        int r=fread(in, sizeof(short), FRAME_SIZE, fin);    
            
        if (r<FRAME_SIZE)    
            break;    
            
        //把16bits的值转化为float,以便speex库可以在上面工作    
    spx_int16_t * ptr=(spx_int16_t *)in;    
        
    if (speex_preprocess_run(m_st, ptr))//预处理 打开了静音检测和降噪    
    {    
        printf("speech,");    
        fwrite(in, sizeof(short), FRAME_SIZE, fout3);    
    }    
    else    
    {    
        printf("slience,");    
        fwrite(out, sizeof(short), FRAME_SIZE, fout3);    
    }    
        for (i=0;i<FRAME_SIZE;i++)    
                
            input[i]=in[i];    
            
        //清空这个结构体里所有的字节,以便我们可以编码一个新的帧    
            
        speex_bits_reset(&bitsEncode);    
            
        //对帧进行编码    
            
        ret=speex_encode(stateEncode, input, &bitsEncode);    
        //把bits拷贝到一个利用写出的char型数组    
        nbBytes = speex_bits_write(&bitsEncode, cbits, 200);    
        fwrite(cbits, sizeof(char), nbBytes, fout1);    
        printf("speex_bits_write:%d\n",nbBytes);    
            
    
        //清空这个结构体里所有的字节,以便我们可以编码一个新的帧    
        speex_bits_reset(&bitsDecode);    
        //将编码数据如读入bits    
        speex_bits_read_from(&bitsDecode, cbits, nbBytes);      
        //对帧进行解码    
        ret = speex_decode(stateDecode, &bitsDecode,output);    
        for (i=0;i<FRAME_SIZE;i++)    
            out[i]=output[i];    
        fwrite(out, sizeof(short), FRAME_SIZE, fout2);    
    }    
        
    //释放编码器状态量    
        
    speex_encoder_destroy(stateEncode);    
        
    //释放bit_packing结构    
        
    speex_bits_destroy(&bitsEncode);    
    speex_decoder_destroy(stateDecode);    
        
    //释放bit_packing结构    
        
    speex_bits_destroy(&bitsDecode);    
    fclose(fin);    
    fclose(fout1);    
    fclose(fout2);    
    fclose(fout3);    
 return 0;    
        
}    
*/
