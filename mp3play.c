#include "mp3play.h"
#include "openal_manager.h"
#include <AL/al.h>
#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

MP3Play* MP3PlayNew()
{
    MP3Play *player = (MP3Play*)malloc(sizeof(MP3Play));
    if(player != NULL)
    {
        player->sf = NULL;
        player->playing = 0;
        player->thread = 0;
        
        return player;
    }
    else
        return NULL;
}

void* MP3PlayThread(void* arg)
{
    
    MP3Play *player = (MP3Play*)arg;
    ALuint buffer, source;
    ALfloat source_pos[] = {0.0, 0.0, 0.0};
    ALfloat listener_pos[] = {0.0, 0.0, 0.0};
    ALfloat listener_vel[] = {0.0, 0.0, 0.0};

    ALCdevice* device = OpenAL_GetDevice();
    ALCcontext* context = OpenAL_GetContext();
    
    alGenBuffers(1, &buffer);
    alGenSources(1, &source);
    
    alListenerfv(AL_POSITION, listener_pos);    
    alListenerfv(AL_VELOCITY, listener_vel);
    
    int16_t read_buf[BUFFER_SIZE];
    int16_t* data = NULL;
    size_t data_size = 0;

    size_t read_size;
    while ((read_size = sf_read_short(player->sf, read_buf, BUFFER_SIZE)) > 0) {
        data = realloc(data, data_size + read_size * sizeof(int16_t));
        if (data == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            sf_close(player->sf);
        }
        memcpy(data + data_size / sizeof(int16_t), read_buf, read_size * sizeof(int16_t));
        data_size += read_size * sizeof(int16_t);
    }
    sf_close(player->sf);
    alBufferData(buffer, AL_FORMAT_STEREO16, data, data_size, player->info.samplerate);
    
    alSourcei(source, AL_BUFFER, buffer);
    player->source = source;
    
    alSourcePlay(source);
    
    while (player->playing) 
    {
        ALint state;
        alGetSourcei(source,AL_SOURCE_STATE,&state);
        if(state == AL_STOPPED)
            break;
        
    }   
    
    alSourceStop(source);
    player->source = 0;
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    
    free(data);
    return NULL;
}


int MP3PlayStart(MP3Play* player, const char* filename)
{
    if(OpenAL_Init() != 0) {
        return -1;
    }

    player->sf = sf_open(filename, SFM_READ, &player->info);
    if (!player->sf) {
        return -1;
    }
   
    if(player->playing)
        MP3PlayStop(player);
    
    player->playing = 1;
    
    if(pthread_create(&player->thread, NULL, MP3PlayThread, (void*)player))
    {
        player->sf = NULL;
        return -1;
    }
    
    return 0;
}

void MP3PlayStop(MP3Play* player)
{
    if(player->playing)
    {
        player->playing = 0;
        pthread_join(player->thread, NULL);
    }
}

void MP3PlayVolume(MP3Play* player, int no)
{
    if(player->source != 0)
    {
        float newVolume = (float)no / 255;
        alSourcef(player->source, AL_GAIN, newVolume);
    }
}

void MP3PlayFree(MP3Play *player) {
    if (player) {
        MP3PlayStop(player);

        if (player->sf) {
            sf_close(player->sf);
            player->sf = NULL;
        }

        OpenAL_Cleanup();
        free(player);
    }
}
