/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: Lvns.c,v 1.30 2001/08/12 11:35:43 tf Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lvns.h"
#include "LvnsEffect.h"

/**
 * LVNS ����
 */
Lvns *
LvnsNew(void)
{
	Lvns *lvns;
    dprintf((stderr, "system initialize...\n"));

	if ((lvns = malloc(sizeof *lvns)) == NULL)
		return NULL;

    lvns->data_path     = strdup(".");
    lvns->savedata_path = NULL;

    /* ---------------------------------------------------------- */
    /* ���ʥꥪ�ѡ�������� */

    lvns->start_scn_num = -1;
    lvns->scn_data = NULL;
    lvns->scn_text = NULL;
    lvns->scn_current = 0;

    lvns->image_mode = False;
	
    lvns->fast_text     = False;
    lvns->skip          = False;
	
    lvns->char_wait_time  = 1; /* tick */
	
    lvns->force_skip      = False;

	lvns->fast_when_seen  = True;
    lvns->key_click_fast  = True;
    lvns->enable_effect = True;
    lvns->enable_effect_back = True;

    lvns->seen_mode = False;
    lvns->demo_mode = False;

    /* ------------------------------------------------------ */
    /* �ƥ����ȥ쥤������� */

    lvns->current_tvram = 1;
    LvnsClearText(lvns);
    lvns->current_tvram = 0;
    LvnsClearText(lvns);
	
    lvns->cursor_state = 0;
    lvns->text_cursor_state = 0;
    lvns->text_attr   = 0;

    /* -------------------------------------------------------- */
    /* ��������Ͻ���� */
    lvns->latitude  = 16;
    lvns->latitude_dark = 11;

    lvns->sepia_mode_next = False;
    lvns->sepia_mode = False;

    lvns->effect_back       = NULL;
    lvns->effect_back_next  = NULL;
    lvns->effect_back_state = 0;

	lvns->disp_func = NULL;

    /* -------------------------------------------------------- */
    /* ���ں����Ͻ���� */
    lvns->current_music        = 0;
    lvns->next_music           = 0;
    lvns->loop_music           = False;
    lvns->current_music_volume = 255;
    lvns->music_fade_mode      = False;
    lvns->music_fade_flag      = 0;
    lvns->music                = NULL;

    /* --------------------------------------------------------- */
    /* SE ��Ϣ����� */
    lvns->sound_loop  = False;
    lvns->sound_count = 0;
	lvns->sound       = NULL;

    /* --------------------------------------------------------- */
    /* �ҥ��ȥ��Ϣ����� */
	lvns->history      = NULL;
	lvns->history_pos  = 0;
	lvns->history_size = 0;

	return lvns;
}

/**
 * LVNS �����
 * @param type   ���������
 * @param depend �����ƥ��¸����
 */
void
LvnsInitialize(Lvns *lvns, int type, void *depend)
{
	lvns->system_depend = depend;

	/* �⥸�塼���̽��� */

    switch (type) {
    case 0:
        SizukuInitialize(lvns);
        break;
    case 1:
        KizuatoInitialize(lvns);
        break;
    case 2:
        ToHeartInitialize(lvns);
        break;
    }

	/* �������̥Хåե����� */
    lvns->background = lvnsimage_new(WIDTH, HEIGHT, 0, 0);
    lvns->vram       = lvnsimage_new(WIDTH, HEIGHT, 0, 0);
    LVNS->clearScreen(lvns);

    dprintf((stderr, "data loading...\n"));
    {
        /* �ǡ����ɤ߽Ф��ѥѥ� */
        char path[1024];
    
        /* �������Ѵ��ơ��֥���ɤ߹��� */
        {
            FILE *fp;
            size_t size;
            int i;
        
            if ((fp = fopen(LVNS->fonttable_name, "r")) == NULL) {
                snprintf(path, sizeof path, "%s/%s", lvns->data_path, LVNS->fonttable_name);
                if ((fp = fopen(path, "r")) == NULL) {
                    perror(path);
                    exit(1);
                }
            }
            fseek(fp, 0, SEEK_END);
            size = ftell(fp);
            rewind(fp);
        
            /* �Ѵ��ѥơ��֥��ߤ��� */
            lvns->leaf_to_euc = malloc(size);
            if (fread(lvns->leaf_to_euc, 1, size, fp) != size) {
                perror(LVNS->fonttable_name);
                free(lvns->leaf_to_euc);
                exit(1);
            }
            fclose(fp);
        
            /* ���Ѵ��ѥơ��֥���� */
            size /= 2;  /* ʸ���� */
            lvns->jis_to_leaf = (u_short *)
                malloc(sizeof(u_short) * 94 * 94);

            /* ����� */
            for (i=0; i < 94 * 94; i++) {
                lvns->jis_to_leaf[i] = 0;
            }
        
            /* �ޥåԥ� */
            for (i=0; i < size; i++) {
                int code = ((lvns->leaf_to_euc[i*2  ] & 0x7f) - 33) * 94 +
                    (lvns->leaf_to_euc[i*2+1] & 0x7f) - 33;
                lvns->jis_to_leaf[code] = i;
            }
        }

        /* �ǡ����ѥå����������ץ� */
        snprintf(path, sizeof path, "%s/%s", lvns->data_path, LVNS->leafpack_name);
        if ((lvns->leafpack = leafpack_new(path)) == NULL) {
            if ((lvns->leafpack = leafpack_new(LVNS->leafpack_name)) == NULL) {
                fprintf(stderr, "Can't open package %s.\n", LVNS->leafpack_name);
                exit(1);
            }
        }

        /* ���ʥꥪ�ѥå����������ץ� */
        if ((LVNS->scnpack_name)) {
            snprintf(path, sizeof path, "%s/%s", lvns->data_path, LVNS->scnpack_name);
            if ((lvns->scnpack = leafpack_new(path)) == NULL) {
                if ((lvns->scnpack = leafpack_new(LVNS->scnpack_name)) == NULL) {
                    fprintf(stderr, "Can't open package %s.\n", 
                            LVNS->scnpack_name);
                    exit(1);
                }
            }
        } else {
            lvns->scnpack = lvns->leafpack;
        }

        /* �ե���ȥǡ����ɤ߹��� */
        if (!(lvns->leaf_font_data=LvnsLoadData(lvns, "KNJ_ALL.KNJ", NULL))){
            fprintf(stderr, "Can't open KNJ_ALL.KNJ.\n");
            exit(1);
        }
    }
}

/**
 * LVNS �˴�
 */
void
LvnsDestroy(Lvns *lvns)
{
	if (lvns) {
		/* ���ʥꥪ�ǡ������� */
		free(lvns->scn_data);
		free(lvns->scn_text);
		
		/* �ǡ����ѥå��������� */
		leafpack_delete(lvns->leafpack);
		
		/* ���ʥꥪ�ѥå��������� */
		if (LVNS->scnpack_name)
			leafpack_delete(lvns->scnpack);
		
		/* �ե���ȥǡ������� */
		free(lvns->leaf_font_data);

		/* �����֥ǡ����ѥ������� */
		if (lvns->savedata_path) {
			free(lvns->savedata_path);
		}

		/* �ǡ����ѥ������� */
		if (lvns->data_path) {
			free(lvns->data_path);
		}

		/* �ҥ��ȥ�����Ѵ� */
		if (lvns->history) {
			free(lvns->history);
		}

		/* ��ʬ���Ȥ��˴� */
		free(lvns);
	}
}

/**
 * �����ؿ����󥿡��Х�¹Խ���
 */
static void
Interval(Lvns *lvns, Bool redisp_ok)
{
	/* BGM �ν��� */
	if (lvns->current_music) {
		/* �ե����ɽ��� */
		if (lvns->music_fade_mode) {
			int vol, fade;
			if (lvns->music_fade_flag) {
				fade =   256 * 2 * 10 / INTERVAL;  /* 0.5�ð��� */
			} else {
				fade = - 256 * 2 * 10 / INTERVAL;  /* 0.5�ð��� */
			}
			
			vol = lvns->current_music_volume + fade;
			
			if (vol <= 0) {
				vol = 0;
			}
			if (vol >= 255) {
				vol = 255;
			}
			
			if (lvns->music_fade_flag) {
				if (vol == 255) {
					lvns->music_fade_mode = False;
					lvns->music_fade_flag = False;
				}
			}  else {
				if (vol == 0) {
					lvns->music_fade_mode = False;
					lvns->music_fade_flag = True;
				}
			}
			
			LvnsSetMusicVolume(lvns, vol);
			if (vol == 0)
				LvnsPauseMusic(lvns);
		}

		if (lvns->loop_music)
			LvnsLoopMusic(lvns);
	}
	
	if (lvns->sound_loop || lvns->sound_count) 
		LvnsLoopSound(lvns);

	/* �طʲ������� */
	if (lvns->effect_back && lvns->enable_effect_back) {
		LvnsBackEffectSetState(lvns);
#ifdef USE_MGL
		MglGetEvent(lvns, 1);
#endif
		if (redisp_ok) {
			LvnsBackEffect(lvns);
			LvnsDispWindow(lvns);
		} else {
			LvnsBackEffect(lvns);
		}
	}
}

/**
 * ���̥ե�å��Ԥ�
 */
void
LvnsFlip(Lvns *lvns, Bool redisp_ok)
{
	Interval(lvns, redisp_ok);
	lvns->flip(lvns);
}

/*
 * �����ᥤ�󥨥󥸥�
 */
void
LvnsMain(Lvns *lvns)
{
	/* BGM�Ͻ���� */
	MusicOpen(lvns->music);
	
	/* SE�Ͻ���� */
	SoundOpen(lvns->sound);

	/* �Ƽ����� */
	LVNS->start(lvns);

	/* ��襸������ */
	switch ((LvnsJumpParameter)setjmp(lvns->jump)) {
	case LVNS_JUMP_NONE: /* ��� */
		break;
	case LVNS_JUMP_RESTART:
		goto main_start;
	case LVNS_JUMP_TITLE:
		goto title;
	case LVNS_JUMP_END:
		goto end;
	}

	if (lvns->start_scn_num >= 0) {
		/* �ǥХå��ѥ��ʥꥪ���굯ư */
		LvnsInitSavePoint(lvns, &lvns->savepoint);
		lvns->savepoint.scn        = lvns->start_scn_num;
		lvns->savepoint.blk        = 1;
		lvns->savepoint.scn_offset = 0;
	} else {
		/* ��ư�����󥰥� & �����ץ˥� */
		LVNS->jingle(lvns);
		LVNS->opening(lvns);
	}

 main_start:
	while (1) {
		lvns->history_pos = 0;
		lvns->latitude    = 16;
		LVNS->main(lvns);

		/* BGM��� */
		LvnsStopMusic(lvns);
		LvnsStopSound(lvns);

		/* ���̾õ� */
		lvns->current_tvram = 0;
		LvnsClearText(lvns);
		LvnsClear(lvns, LVNS_EFFECT_WIPE_TTOB);

	title:
		lvns->latitude    = 16;
		LVNS->title(lvns);
	}

 end:

	/* ������ɷϽ�λ */
	SoundClose(lvns->sound);
	
	/* BGM�Ͻ�λ */
	MusicClose(lvns->music);
}

/**
 * �����Ԥ�
 * ���ꤷ������Υե�å�ʬ�Ԥ�
 */
void
LvnsWait(Lvns *lvns, int flip_cnt)
{
	while (flip_cnt-- > 0) {
		LvnsFlip(lvns, True);
	}
}

/**
 * ��襸���׽���
 */
void
LvnsJump(Lvns *lvns, LvnsJumpParameter param)
{
	LvnsSetDispFunc(lvns, NULL, NULL);
	longjmp(lvns->jump, (int)param);
	fprintf(stderr, "warning: bad jump!: %d\n", param);
	exit(1);
}

/**
 * �ե�������¸�ѥ�����
 */
void
LvnsSetSaveDataPath(Lvns *lvns, const char *path)
{
    if (lvns->savedata_path) {
        free(lvns->savedata_path);
    }
    lvns->savedata_path = strdup(path);
}

void
LvnsSetDataPath(Lvns *lvns, const char *path)
{
	if (lvns->data_path) {
		free(lvns->data_path);
	} 
	lvns->data_path = strdup(path);
}

void
LvnsSetDispFunc(Lvns *lvns, void (*func)(Lvns *lvns, void *data), void *data)
{
	lvns->disp_func = func;
	lvns->disp_func_param = data;
}
