/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: LvnsCoreMusic.c,v 1.21 2001/08/05 15:43:47 tf Exp $
 *
 */

/*
 * LvnsCore 音楽処理回り
 */

#include "mp3play.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef USE_MGL
#include "LvnsCoreP.h"
#else
#include "mgLvnsCore.h"
#endif

#ifndef PULSE
/* ボリューム設定 */
static void
SetMusicVolume(void* dep, int no)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    cdinfo_set_volume(LCW.cdinfo, no);
}

/* BGM 演奏開始 */
static void
StartMusic(void *dep, int no)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    cdinfo_play(LCW.cdinfo, no);
}

static void
StopMusic(void *dep)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    cdinfo_stop(LCW.cdinfo);
}

/* BGM 一時停止 */
static void
PauseMusic(void* dep)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    cdinfo_pause(LCW.cdinfo);
}

static int
GetMusicState(void *dep)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    CDTimeInfo cur;
    return cdinfo_get_current_time(LCW.cdinfo, &cur);
}

static void
CloseMusic(void* dep)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    cdinfo_delete(LCW.cdinfo);
	LCW.cdinfo = NULL;
}

static void
OpenMusic(void *dep)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    // char *disc = getenv("CDDEVICE");
    // if (!disc) 
    //     disc = CDDEVICE;
    if ((LCW.cdinfo = cdinfo_new("CD")) == NULL) {
    } 
}

void 
LvnsCoreInitMusic(LvnsCoreWidget lcw)
{
	LCW.music.depend     = lcw;
	LCW.music.open       = OpenMusic;
	LCW.music.close      = CloseMusic;
	LCW.music.start      = StartMusic;
	LCW.music.stop       = StopMusic;
	LCW.music.pause      = PauseMusic;
	LCW.music.setVolume  = SetMusicVolume;
	LCW.music.getState   = GetMusicState;

	LCW.cdinfo = NULL;
}
#else
/* ボリューム設定 */
static void
SetMusicVolume(void* dep, int no)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    MP3PlayVolume(LCW.mp3, no);
}

/* BGM 演奏開始 */
static void
StartMusic(void *dep, int no)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    char name[256];
    snprintf(name, sizeof(name), "CD/%d.mp3", no);
    MP3PlayStart(LCW.mp3, name);
}

static void
StopMusic(void *dep)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    MP3PlayStop(LCW.mp3);
}

/* BGM 一時停止 */
static void
PauseMusic(void* dep)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    // cdinfo_pause(LCW.cdinfo);
}

static int
GetMusicState(void *dep)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    CDTimeInfo cur;
    // return cdinfo_get_current_time(LCW.cdinfo, &cur);
    return 0;   
}

static void
CloseMusic(void* dep)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    MP3PlayFree(LCW.mp3);
	LCW.mp3 = NULL;
}

static void
OpenMusic(void *dep)
{
    struct _LvnsCoreRec *lcw = (struct _LvnsCoreRec *)dep;
    if ((LCW.mp3 = MP3PlayNew()) == NULL) {
    } 
}

void 
LvnsCoreInitMusic(LvnsCoreWidget lcw)
{
	LCW.music.depend     = lcw;
	LCW.music.open       = OpenMusic;
	LCW.music.close      = CloseMusic;
	LCW.music.start      = StartMusic;
	LCW.music.stop       = StopMusic;
	LCW.music.pause      = PauseMusic;
	LCW.music.setVolume  = SetMusicVolume;
	LCW.music.getState   = GetMusicState;

	LCW.mp3 = NULL;
}
#endif
