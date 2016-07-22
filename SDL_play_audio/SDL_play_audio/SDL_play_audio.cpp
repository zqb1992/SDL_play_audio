// SDL_play_audio.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
extern "C"
{
	#include "SDL2/SDL.h"
}

struct audio_para{
	Uint8  *data;
	Uint32  len;
	Uint8  *pos;
}AUD;


/* Audio Callback
* The audio function callback takes the following parameters:
* stream: A pointer to the audio buffer to be filled
* len: The length (in bytes) of the audio buffer
*
*/
void  fill_audio(void *udata, Uint8 *stream, int len){
	//SDL 2.0
	SDL_memset(stream, 0, len);

	if (AUD.len == 0)
		return;

	len = (len > AUD.len ? AUD.len : len);	/*  Mix  as  much  data  as  possible  */

	SDL_MixAudio(stream, AUD.pos, len, SDL_MIX_MAXVOLUME);
	AUD.pos += len;
	AUD.len -= len;
}



int _tmain(int argc, _TCHAR* argv[])
{
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {           //��ʼ����ʱ������Ƶ 
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}
	
	SDL_AudioSpec desired;
	desired.freq = 44100;             //��Ƶ���ݵĲ�����
	desired.format = AUDIO_S16SYS;    //��Ƶ���ݵĸ�ʽ
	desired.channels = 2;             //������
	desired.silence = 0;              //����ʱ��ֵ
	desired.samples = 1024;           //��Ƶ�������еĲ�������
	desired.callback = fill_audio;    //�ص�����
	

	if (SDL_OpenAudio(&desired, NULL)<0){
		printf("can't open audio.\n");
		return -1;
	}

	FILE *fp = fopen("NocturneNo2inEflat_44.1k_s16le.pcm", "rb+");
	if (fp == NULL){
		printf("cannot open this file\n");
		return -1;
	}
	
	int pcm_buffer_size = 4096;
	char *pcm_buffer = (char *)malloc(pcm_buffer_size);
	int data_count = 0;

	while (1){
		if (fread(pcm_buffer, 1, pcm_buffer_size, fp) != pcm_buffer_size){   //��ȡpcm����
			// Loop
			//fseek(fp, 0, SEEK_SET);
			//fread(pcm_buffer, 1, pcm_buffer_size, fp);
			//data_count=0;
			break;
		}
		printf("Now Playing %10d Bytes data.\n", data_count);
		data_count += pcm_buffer_size;

		//Set audio buffer (PCM data)
		//Audio buffer length
		SDL_LockAudio();
			AUD.data = (Uint8 *)pcm_buffer;
			AUD.len = pcm_buffer_size;
			AUD.pos = AUD.data;
		SDL_UnlockAudio();

		//Play
		SDL_PauseAudio(0);

		while (AUD.len>0)//Wait until finish
			SDL_Delay(1);
	}

	free(pcm_buffer);
	SDL_Quit();

	return 0;
}

