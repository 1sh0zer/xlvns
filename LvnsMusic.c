/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: LvnsMusic.c,v 1.4 2001/07/21 22:05:00 go Exp $
 *
 */

/*
 * Lvns ���ڽ������
 */

#include <stdio.h>
#include "Lvns.h"

/* �ܥ�塼������ */
void
LvnsSetMusicVolume(Lvns *lvns, int no)
{
    lvns->current_music_volume = no;
	MusicSetVolume(lvns->music, no);
    dprintf((stderr, "set volume: %d\n", no));
}

/* BGM ���ճ��� */
void
LvnsStartMusic(Lvns *lvns, int no)
{
    LvnsSetMusicVolume(lvns, 255);
    if (no != lvns->current_music) {
		MusicStart(lvns->music, no);
    }
    lvns->music_fade_mode = False;
    lvns->music_fade_flag = False; /* Fade out */
    lvns->current_music = no;
    lvns->loop_music = False;
}

/* BGM ��� */
void
LvnsStopMusic(Lvns *lvns)
{
	MusicStop(lvns->music);
    lvns->current_music = 0;
}

/* BGM ������ */
void
LvnsPauseMusic(Lvns *lvns)
{
	MusicPause(lvns->music);
    lvns->current_music = 0;
}

/* BGM �롼�ױ��ճ��� */
void
LvnsStartMusicLoop(Lvns *lvns, int no)
{
    LvnsSetMusicVolume(lvns, 255);
    if (no != lvns->current_music) {
		MusicStart(lvns->music, no);
	}
    lvns->music_fade_mode = False;
    lvns->music_fade_flag = False; /* Fade out */
    lvns->current_music = no;
    lvns->loop_music = True;
}

/* BGM �롼�ױ��ճ��� */
void
LvnsStartMusicLoop2(Lvns *lvns, int no)
{
    LvnsSetMusicVolume(lvns, 0);
    if (no != lvns->current_music) {
		MusicStart(lvns->music, no);
    }
    lvns->music_fade_mode = False;
    lvns->music_fade_flag = True; /* Fade in */
    lvns->current_music = no;
    lvns->loop_music = True;
}

/* ���Υ�����Ǥ� BGM ����� */
void
LvnsSetNextMusic(Lvns *lvns, int no)
{
    lvns->next_music = no;
    lvns->loop_next_music = False;
}

/* ���Υ������ BGM (�롼�ױ���) ����� */
void
LvnsSetNextMusicLoop(Lvns *lvns, int no)
{
    lvns->next_music = no;
    lvns->loop_next_music = True;
}

/* BGM FadeOut */
void
LvnsFadeMusic(Lvns *lvns)
{
    if (lvns->current_music) {
        lvns->music_fade_mode = True;
    }
}

/* ---------------------------------------------------------------- */

/* ���Υ������ BGM �򳫻Ϥ��� */
void
LvnsStartNextMusic(Lvns *lvns)
{
    if (lvns->next_music) {
        if (lvns->loop_next_music) {
            LvnsStartMusicLoop(lvns, lvns->next_music);
        } else {
            LvnsStartMusic(lvns, lvns->next_music);
        }
        lvns->next_music = 0;
    }
}

/* BGM ��λ�Ԥ����� */
void
LvnsWaitMusic(Lvns *lvns)
{
	if (lvns->current_music == 0)
		return;
	lvns->loop_music = False;

	while (!(lvns->current_music == 0 ||
			 lvns->current_music_volume <= 0 ||
			 MusicGetState(lvns->music))) {
		LvnsFlip(lvns, True);
	}
}

/* BGM Fade ������λ�Ԥ����� */
void
LvnsWaitMusicFade(Lvns *lvns)
{
	if (lvns->current_music == 0)
		return;
	lvns->loop_music = False;

	while (!(lvns->current_music == 0 ||
			 lvns->music_fade_mode == False)) {
		LvnsFlip(lvns, True);
    } 
}

/* BGM �롼�׽��� */
void
LvnsLoopMusic(Lvns *lvns)
{
    if (lvns->current_music &&
		MusicGetState(lvns->music)) {
		MusicStart(lvns->music, lvns->current_music);
    } 
}

