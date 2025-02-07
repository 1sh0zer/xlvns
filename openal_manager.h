#ifndef OPENAL_MANAGER_H
#define OPENAL_MANAGER_H

#include <AL/al.h>
#include <AL/alc.h>

int OpenAL_Init();

ALCcontext* OpenAL_GetContext();

ALCdevice* OpenAL_GetDevice();

void OpenAL_Cleanup();

#endif // OPENAL_MANAGER_H
