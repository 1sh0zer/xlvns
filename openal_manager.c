#include "openal_manager.h"
#include <stdio.h>
#include <stdlib.h>

static ALCdevice *device = NULL;
static ALCcontext *context = NULL;

int OpenAL_Init() {
    if (device != NULL && context != NULL) {
        return 0;
    }
    device = alcOpenDevice(NULL);
    if (!device) {
        fprintf(stderr, "Failed to open OpenAL device\n");
        device = NULL;
        return -1;
    }

    context = alcCreateContext(device, NULL);
    if (!context || !alcMakeContextCurrent(context)) {
        fprintf(stderr, "Failed to create or set OpenAL context\n");
        context = NULL;
        alcCloseDevice(device);
        return -1;
    }

    return 0;
}

ALCcontext* OpenAL_GetContext() {
    return context;
}

ALCdevice* OpenAL_GetDevice() {
    return device;
}

void OpenAL_Cleanup() {
    if (context) {
        alcMakeContextCurrent(NULL);
        alcDestroyContext(context);
    }
    if (device) {
        alcCloseDevice(device);
    }
}
