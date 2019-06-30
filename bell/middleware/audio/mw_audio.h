#ifndef __MW_AUDIO_H__
#define __MW_AUDIO_H__

int mw_media_audio_open(struct media_a *audio);

int mw_media_audio_close(struct media_a *audio);

void mw_audio_clear_play(void);
void mw_audio_clear_record(void);
int mw_is_play_buffer_empty(void);

#endif
