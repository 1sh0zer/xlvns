#ifndef mp3play_h
#define mp3play_h

#include <sndfile.h>
#include "pthread.h"
#include "AL/al.h"

#define BUFFER_SIZE 4096

typedef struct 
{
    SNDFILE *sf;
    int playing;
    ALint source;
    pthread_t thread;
    SF_INFO info;
}
MP3Play;

MP3Play* MP3PlayNew();

int MP3PlayStart(MP3Play* player, const char* name);

void MP3PlayStop(MP3Play* player);

void MP3PlayFree(MP3Play* player);

void MP3PlayVolume(MP3Play* player, int no);

#endif
