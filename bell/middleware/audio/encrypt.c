#include "encrypt.h"
#include <imp/imp_audio.h>
#include <imp/imp_log.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

/* My G711 Encoder */
#define SIGN_BIT    (0x80)      /* Sign bit for a A-law byte. */
#define QUANT_MASK  (0xf)       /* Quantization field mask. */
#define NSEGS       (8)         /* Number of A-law segments. */
#define SEG_SHIFT   (4)         /* Left shift for segment number. */
#define SEG_MASK    (0x70)      /* Segment field mask. */
#define BIAS        (0x84)      /* Bias for linear code. */

#define TAG "encrypt"

/*
**MY_G711U_Decoder 
*/
/*=============================================start========================================================*/

/* My G711 Encoder */
static short seg_end[8] = {0xFF, 0x1FF, 0x3FF, 0x7FF,0xFFF, 0x1FFF, 0x3FFF, 0x7FFF};

/* My 5350 Encoder */
static int g_nEnAudioPreSample = 0;
static int g_nEnAudioIndex = 0;
static int g_nDeAudioPreSample[4];
static int g_nDeAudioIndex = 0;

static int gs_index_adjust[8] = { -1, -1, -1, -1, 2, 4, 6, 8};
static int gs_step_table[89] =
{
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
    50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371,
    408, 449, 494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493,
    10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};

static int search(int val, short *table, int	size)
{
	int	i;

	for (i = 0; i < size; i++) {
		if (val <= *table++)
			return (i);
	}
	return (size);
}


/*
**MY_G711U_Encoder
*/
/*=============================================start========================================================*/
static unsigned char linear2ulaw(int pcm_val)
{
	int	mask;
	int	seg;
	unsigned char uval;

	if (pcm_val < 0) {
		pcm_val = BIAS - pcm_val;
		mask = 0x7F;
	} else {
		pcm_val += BIAS;
		mask = 0xFF;
	}

	seg = search(pcm_val, seg_end, 8);
	if (seg >= 8)		/* out of range, return maximum value. */
		return (0x7F ^ mask);
	else {
		uval = (seg << 4) | ((pcm_val >> (seg + 3)) & 0xF);
		return (uval ^ mask);
	}
}

static int my_g711u_encode(unsigned char g711_data[], const short amp[], int len)
{
	int i;

	for (i = 0;  i < len;  i++) {
		g711_data[i] = linear2ulaw(amp[i]);
	}

	return len;
}

static int MY_Audio_PCM2G711u(char *InAudioData, char *OutAudioData, int DataLen)
{
	int Retuen = 0;

	if((NULL == InAudioData) && (NULL == OutAudioData) && (0 == DataLen)) {
		IMP_LOG_ERR(TAG, "Error, empty data or transmit failed, exit !\n");
		return -1;
	}

	Retuen = my_g711u_encode((unsigned char *)OutAudioData, (short*)InAudioData, DataLen/2);

	return Retuen;
}

int MY_G711U_Encode_Frm(void *encoder, IMPAudioFrame *data, unsigned char *outbuf,int *outLen)
{
	*outLen = MY_Audio_PCM2G711u((char *)data->virAddr, (char *)outbuf, data->len);
	return 0;
}
/*=============================================end========================================================*/




/*
**MY_G711A_Encoder
*/
/*=============================================start========================================================*/
static unsigned char linear2alaw(int pcm_val)
{
	int		mask;
	int		seg;
	unsigned char	aval;

	if (pcm_val >= 0) {
		mask = 0xD5;		/* sign (7th) bit = 1 */
	} else {
		mask = 0x55;		/* sign bit = 0 */
		pcm_val = -pcm_val - 8;
	}

	seg = search(pcm_val, seg_end, 8);
	if (seg >= 8)		/* out of range, return maximum value. */
		return (0x7F ^ mask);
	else {
		aval = seg << SEG_SHIFT;
		if (seg < 2)
			aval |= (pcm_val >> 4) & QUANT_MASK;
		else
			aval |= (pcm_val >> (seg + 3)) & QUANT_MASK;
		return (aval ^ mask);
	}
}

static int my_g711a_encode(unsigned char g711_data[], const short amp[], int len)
{
	int i;

	for (i = 0;  i < len;  i++) {
		g711_data[i] = linear2alaw(amp[i]);
	}

	return len;
}

static int MY_Audio_PCM2G711a(char *InAudioData, char *OutAudioData, int DataLen)
{
	int Retaen = 0;
//	char outBuffer[1024];

	if((NULL == InAudioData) && (NULL == OutAudioData) && (0 == DataLen)) {
		IMP_LOG_ERR(TAG, "Error, empty data or transmit failed, exit !\n");
		return -1;
	}

//	memset(outBuffer, 0, sizeof(outBuffer));
//	memcpy(outBuffer, InAudioData, DataLen);
//	speex_preprocess_audio(outBuffer, DataLen);	
//	Retaen = my_g711a_encode((unsigned char *)OutAudioData, (short*)outBuffer, DataLen/2);
	Retaen = my_g711a_encode((unsigned char *)OutAudioData, (short*)InAudioData, DataLen/2);

	return Retaen;
}

int MY_G711A_Encode_Frm(void *encoder, IMPAudioFrame *data, unsigned char *outbuf,int *outLen)
{
	*outLen = MY_Audio_PCM2G711a((char *)data->virAddr, (char *)outbuf, data->len);
	return 0;
}
/*=============================================end========================================================*/





/*
**MY_G711A_Decoder
*/
/*=============================================start========================================================*/
static int alaw2linear( unsigned char a_val )
{
	int	t;
	int	seg;

	a_val ^= 0x55;

	t = (a_val & QUANT_MASK) << 4;
	seg = ( (unsigned)a_val & SEG_MASK ) >> SEG_SHIFT;
	switch (seg)
	{
		case 0:
			t += 8;
			break;
		case 1:
			t += 0x108;
			break;
		default:
			t += 0x108;
			t <<= seg - 1;
	}
	return ((a_val & SIGN_BIT) ? t : -t);
}

static int my_g711a_decode( short amp[], const unsigned char g711a_data[], int g711a_bytes )
{
	int i;
	int samples;
	unsigned char code;
	int sl;

	for ( samples = i = 0; ; )
	{
		if (i >= g711a_bytes)
			break;
		code = g711a_data[i++];

		sl = alaw2linear( code );

		amp[samples++] = (short) sl;
	}
	return samples*2;
}

static int MY_Audio_G711a2PCM(char *InAudioData, char *OutAudioData, int DataLen)
{
	int Retade = 0;

	if((NULL == InAudioData) && (NULL == OutAudioData) && (0 == DataLen)) {
		IMP_LOG_ERR(TAG, "Error, empty data or transmit failed, exit !\n");
		return -1;
	}

	Retade = my_g711a_decode((short*)OutAudioData, (unsigned char *)InAudioData, DataLen);

	return Retade;
}


int MY_G711A_Decode_Frm(void *decoder, unsigned char *inbuf,int inLen, unsigned short *outbuf,int *outLen,int *chns)
{
	*outLen = MY_Audio_G711a2PCM((char *)inbuf, (char *)outbuf, inLen);
	return 0;
}
/*=============================================end========================================================*/



/*
**MY_G711U_Decoder 
*/
/*=============================================start========================================================*/
static int ulaw2linear(unsigned char u_val)
{
	int	t;

	u_val = ~u_val;

	t = ((u_val & QUANT_MASK) << 3) + BIAS;
	t <<= ((unsigned)u_val & SEG_MASK) >> SEG_SHIFT;

	return ((u_val & SIGN_BIT) ? (BIAS - t) : (t - BIAS));
}

static int my_g711u_decode(short amp[], const unsigned char g711u_data[], int g711u_bytes)
{
	int i;
	int samples;
	unsigned char code;
	int sl;

	for (samples = i = 0;;)
	{
		if (i >= g711u_bytes)
			break;
		code = g711u_data[i++];

		sl = ulaw2linear(code);

		amp[samples++] = (short) sl;
	}
	return samples*2;
}

static int MY_Audio_G711u2PCM(char *InAudioData, char *OutAudioData, int DataLen)
{
	int Retude = 0;

	if((NULL == InAudioData) && (NULL == OutAudioData) && (0 == DataLen)) {
		IMP_LOG_ERR(TAG, "Error, empty data or transmit failed, exit !\n");
		return -1;
	}

	Retude = my_g711u_decode((short*)OutAudioData, (unsigned char *)InAudioData, DataLen);

	return Retude;
}


int MY_G711U_Decode_Frm(void *decoder, unsigned char *inbuf,int inLen, unsigned short *outbuf,int *outLen,int *chns)
{
	*outLen = MY_Audio_G711u2PCM((char *)inbuf, (char *)outbuf, inLen);
	return 0;
}
/*=============================================end========================================================*/






/*
**5350 audio module  encoder and decoder
*/
/*===========================================start==========================================================*/
static void ADPCMDecode( char* pDataCompressed, int nLenData, char* pDecoded)
{
	int usit = 0;
    int i;
    int code;
    int sb;
    int delta;
    short* pcm = ( short* )pDecoded;
    nLenData <<= 1;

    for ( i = 0; i < nLenData; i++ )
    {
        if ( i & 0x01 )
        {
            code = pDataCompressed[i >> 1] & 0x0f;
        }

        else
        {
            code = pDataCompressed[i >> 1] >> 4;
        }

        if ( ( code & 8 ) != 0 )
        {
            sb = 1;
        }

        else
        {
            sb = 0;
        }

        code &= 7;
        delta = ( gs_step_table[g_nDeAudioIndex] * code ) / 4 + gs_step_table[g_nDeAudioIndex] / 8;

        if ( sb )
        {
            delta = -delta;
        }

        g_nDeAudioPreSample[usit] += delta;

        if ( g_nDeAudioPreSample[usit] > 32767 )
        {
            g_nDeAudioPreSample[usit] = 32767;
        }

        else if ( g_nDeAudioPreSample[usit] < -32768 )
        {
            g_nDeAudioPreSample[usit] = -32768;
        }

        pcm[i] = g_nDeAudioPreSample[usit];
        g_nDeAudioIndex += gs_index_adjust[code];

        if ( g_nDeAudioIndex < 0 )
        {
            g_nDeAudioIndex = 0;
        }

        if ( g_nDeAudioIndex > 88 )
        {
            g_nDeAudioIndex = 88;
        }
    }
}


static void ADPCMEncode( unsigned char* pRaw, int nLenRaw, unsigned char* pBufEncoded )
{
    short* pcm = ( short* )pRaw;
    int cur_sample;
    int i;
    int delta;
    int sb;
    int code;
    nLenRaw >>= 1;

    for ( i = 0; i < nLenRaw; i++ )
    {
        cur_sample = pcm[i];
        delta = cur_sample - g_nEnAudioPreSample;

        if ( delta < 0 )
        {
            delta = -delta;
            sb = 8;
        }

        else
        {
            sb = 0;
        }

        code = 4 * delta / gs_step_table[g_nEnAudioIndex];

        if ( code > 7 )
        {
            code = 7;
        }

        delta = ( gs_step_table[g_nEnAudioIndex] * code ) / 4 + gs_step_table[g_nEnAudioIndex] / 8;

        if ( sb )
        {
            delta = -delta;
        }

        g_nEnAudioPreSample += delta;

        if ( g_nEnAudioPreSample > 32767 )
        {
            g_nEnAudioPreSample = 32767;
        }

        else if ( g_nEnAudioPreSample < -32768 )
        {
            g_nEnAudioPreSample = -32768;
        }

        g_nEnAudioIndex += gs_index_adjust[code];

        if ( g_nEnAudioIndex < 0 )
        {
            g_nEnAudioIndex = 0;
        }

        else if ( g_nEnAudioIndex > 88 )
        {
            g_nEnAudioIndex = 88;
        }

        if ( i & 0x01 )
        {
            pBufEncoded[i >> 1] |= code | sb;
        }

        else
        {
            pBufEncoded[i >> 1] = ( code | sb ) << 4;
        }
    }

//	return nLenRaw/4;
}

int MY_5350_Encode_Frm(void *encoder, IMPAudioFrame *data, unsigned char *outbuf,int *outLen)
{
	EncoderClr(0, 0);
	ADPCMEncode((unsigned char *)data->virAddr, data->len, (unsigned char *)outbuf);
	*outLen = data->len / 4;	
	return 0;
}

int MY_5350_Decode_Frm(void *decoder, unsigned char *inbuf,int inLen, unsigned short *outbuf,int *outLen,int *chns)
{
	ADPCMDecode((char *)inbuf, inLen, (char *)outbuf);
	*outLen = 4*inLen;
	return 0;
}

int GetEncoderAduioPreSample(void)
{
	return g_nEnAudioPreSample;
}

int GetEncoderAudioIndex(void)
{
	return g_nEnAudioIndex;
}

void DecoderClr( int sample, int index )
{
    //g_nDeAudioPreSample = sample;
    //g_nDeAudioIndex = index;
    int i;

    for ( i = 0; i < 4; i++ )
    {
        g_nDeAudioPreSample[i] = sample;
    }

    g_nDeAudioIndex = index;
}

void EncoderClr(int sample,int index)
{
	g_nEnAudioPreSample = sample;
	g_nEnAudioIndex = index;
}
/*===========================================end==========================================================*/

