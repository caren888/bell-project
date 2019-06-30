#ifndef __G711_H__
#define __G711_H__

void mw_g711a_encode(short *lin, int offset, char *enc, int frames);
void mw_g711a_decode(char *enc, short *lin, int frames);
void mw_g711a_init(void);

#endif
