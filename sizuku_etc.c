/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: sizuku_etc.c,v 1.29 2001/08/20 14:15:26 tf Exp $
 */

/* 
 * ����� �������������͏���������������������������
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sizuku.h"
#include "freetype_manager.h"

/* ����� ������������/���������������������͏�������� */
void
SizukuMergeCharacter(Lvns *lvns)
{
    SizukuState *state = (SizukuState*)lvns->system_state;
    lvnsimage_add2(state->vram_char, lvns->vram, 0, 0);
}

/* 
 * ����� ����������䏪󏪪�����
 */
void
SizukuClearScreen(Lvns *lvns)
{
    SizukuLoadBG(lvns, 0);
    SizukuClearCharacter(lvns, 'a');
}

/*
 * ����� ������������������������������������
 */
void
SizukuLoadCharacter(Lvns *lvns, int no, int pos)
{
#define XPOS1 160
#define XPOS2 320

    if (no != 0x99) {
        SizukuState *state = (SizukuState*)lvns->system_state;
        LvnsImage *image;
        char name[100];
        sprintf(name, "MAX_C%02x.LFG", no);
        image = LvnsLoadImage(lvns, name, NULL);
        switch (pos) {
        default:    
        case 'l':
        case 'L':
            if (image)
                lvnsimage_add2(image, state->vram_char, 0, 0);
            lvns->character_no[0] = no;
            break;
        case 'c':
        case'C':
            if (image)
                lvnsimage_add2(image, state->vram_char, XPOS1, 0);
            lvns->character_no[2] = no;
            break;
        case 'r':
        case'R':
            if (image)
                lvnsimage_add2(image, state->vram_char, XPOS2, 0);
		    lvns->character_no[1] = no;
            break;
        }                      
        lvnsimage_delete(image);
    }
#undef XPOS1
#undef XPOS2
}

/*
 * ����� ���������������������ď�󏪪�����
 */
void
SizukuClearCharacter(Lvns *lvns, int pos)
{
#ifndef USE_MGL
#define XPOS1 160
#define XPOS2 320
#define W1 320
#else
#define XPOS1 80
#define XPOS2 160
#define W1 160
#endif

    SizukuState *state = (SizukuState*)lvns->system_state;
    switch (pos) {
    case 'l':
    case 'L':
        lvnsimage_clear_area_transparent(state->vram_char, 0, 0, W1, HEIGHT);
        lvns->character_no[0] = NOCHARACTER;
        break;
    case 'c':
    case'C':
        lvnsimage_clear_area_transparent(state->vram_char, XPOS1, 0, W1, HEIGHT);
        lvns->character_no[2] = NOCHARACTER;
        break;
    case 'r':
    case'R':
        lvnsimage_clear_area_transparent(state->vram_char, XPOS2, 0, W1, HEIGHT);
        lvns->character_no[1] = NOCHARACTER;
        break;

    default:
        state->vram_char->palette_num = 15;
        state->vram_char->transparent = 15;
        lvnsimage_clear_transparent(state->vram_char);
        lvns->character_no[0] = NOCHARACTER;
        lvns->character_no[1] = NOCHARACTER;
        lvns->character_no[2] = NOCHARACTER;
        break;
    }                      
#undef XPOS1
#undef XPOS2
#undef W1
}

/**
 * �������������������������ӏ�����������������������
 */
u_char sizuku_haikei_palette[][4][3] = {
    /* ������ 01, 03, 13, 22, 24*/
    { {0x00,0x00,0x00}, {0x66,0x55,0x22}, {0x88,0x88,0x44}, {0xbb,0xcc,0x88} },
    /* ������������ (����)*/
    { {0x00,0x00,0x00}, {0x77,0x44,0x33}, {0xcc,0x88,0x44}, {0xee,0xcc,0x55} },
    /* ������   11,14,15,18,19 */
    { {0x00,0x00,0x00}, {0x22,0x44,0x66}, {0x55,0x88,0xbb}, {0xbb,0xcc,0xff} },
    /* ��ď�������� 39 */
    { {0x00,0x00,0x00}, {0x00,0x22,0x33}, {0x00,0x44,0x55}, {0x00,0x88,0x99} },
    /* ��ď������������¢��������������� */
    { {0x00,0x00,0x00}, {0x33,0x33,0x55}, {0x30,0x50,0x88}, {0x77,0x88,0xbb}},
    /* �������������돪󏫩 */
    { {0x00,0x00,0x00}, {0xbb,0x00,0x00}, {0xff,0x55,0x00}, {0xff,0x77,0x00} },
    /* ��������������������������������� */
    { {0x00,0x00,0x00}, {0x00,0x00,0x11}, {0x00,0x00,0x44}, {0x11,0x22,0x66} },
    /* ��������������돪󏫩 */
    { {0x00,0x00,0x00}, {0x22,0x22,0x77}, {0x44,0x55,0xbb}, {0x77,0x77,0xff} },
    /* ����������������¢��������� */
    { {0x00,0x00,0x00}, {0x77,0x33,0x33}, {0xaa,0x66,0x33}, {0xff,0xbb,0x66} },
};

#ifdef USE_MGL
int sizuku_haikei_palette_hsb[][3][16] = {
    /* ������ 01, 03, 13, 22, 24*/
  {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
  },
    /* ������������ (����)*/
  {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,  1},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
  },
    /* ������   11,14,15,18,19 */
  {{8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  8,  8,  8,  8,  8,  8},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   {0, 1, 2, 2, 3, 4, 5, 6, 6, 7,  8,  9, 10, 10, 11, 12}
  },
    /* ��ď�������� 39 */
  {{8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  8,  8,  8,  8,  8,  8},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   {0, 1, 2, 2, 3, 4, 5, 6, 6, 7,  8,  9, 10, 10, 11, 12}
  },
    /* ��ď������������¢��������������� */
  {{8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  8,  8,  8,  8,  8,  8},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   {0, 1, 2, 2, 3, 4, 5, 6, 6, 7,  8,  9, 10, 10, 11, 12}
  },
    /* �������������돪󏫩 */
  {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
  },
    /* ��������������������������������� */
  {{8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  8,  8,  8,  8,  8,  8},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   {0, 1, 2, 2, 3, 4, 5, 6, 6, 7,  8,  9, 10, 10, 11, 12}
  },
    /* ��������������돪󏫩 */
  {{8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  8,  8,  8,  8,  8,  8},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   {0, 1, 2, 2, 3, 4, 5, 6, 6, 7,  8,  9, 10, 10, 11, 12}
  },
    /* ����������������¢��������� */
  {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,  1},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
  },
};
#endif

/*
 * ���������������������� �����������������������ď����Џ����
 */
static int
bgmap(int no)
{
    switch (no) {
    case 4:
        return 2; /* ����������(������������) */
    case 5:
        return 2; /* ����������(��ď��������) */
    case 6:
        return 3; /* ��������������Џ�����(������������) */
    case 32:
        return 31; /* ���돪󏫩(������������)   */
    case 33:
        return 31; /* ���돪󏫩(������)     */
    case 35:
        return 34; /* ���돪󏫩(������)������������ */
    case 36:
        return 34; /* ���돪󏫩(������)������   */
    case 38:
        return 11;/* �����¡돫������� (������������) */
    case 41:
        return 15; /* ������������ (������������) */
    case 42:
        return 15; /* ������������ (������) */
    case 43:
        return 10;  /* ��������������(������������) */
    case 44:
        return 10;  /* ��������������(��ď��������) */
    case 45:
        return 30; /* ��ď�ޡ��������(������������) */
    case 46:
        return 30; /* ��ď�ޡ��������(��ď��������) */
    case 47:
        return 22; /* ���������� (������������) */
    case 48:
        return 22; /* ����������������ď��������) */
    case 49:
        return 12; /* ����������������Џ�������(������������) */
    case 50:
        return 12; /* ����������������Џ�������(��ď��������) */
    case 51:
        return 24; /* �����¡돫����������������(������������) */
    case 52:
        return 24; /* �����¡돫����������������(������)   */
    case 53:
        return 12; /* �������������������(��ď��������돪�) */
    case 54:
        return 18; /* ��������������������������������(������������) */
    case 55:
	return 19; /* ������������������������������(������������) */
    default:
        return no;
    }
    return no;
}

/*
 * ����������������������������������������������������� (0: ����������������������������������)
 */
static int
palmap(int no)
{
    switch (no) {
    case 4:
        return SIZUKU_PAL_YUUGATA2;  /* ����������(������������) */
    case 5:
        return SIZUKU_PAL_MIDNIGHT; /* ����������(��ď��������) */
    case 6:
        return SIZUKU_PAL_YUUGATA2;  /* ��������������Џ�����(������������) */
    case 32:
        return SIZUKU_PAL_YUUYAKE; /* ���돪󏫩(��������󏩡����)  */
    case 33:
        return SIZUKU_PAL_NIGHT2; /* ���돪󏫩(������)     */
    case 35:
        return SIZUKU_PAL_YUUYAKE; /* ���돪󏫩(������)��������󏩡���� */
    case 36:
        return SIZUKU_PAL_NIGHT2; /* ���돪󏫩(������)������   */
    case 38:
        return SIZUKU_PAL_YUUGATA;/* �����¡돫������� (������������) */
    case 41:
        return SIZUKU_PAL_YUUGATA; /* ������������ (������������) */
    case 42:
        return SIZUKU_PAL_DAY; /* ������������ (������������) */
    case 43:
        return SIZUKU_PAL_YUUGATA;  /* ��������������(������������) */
    case 44:
        return SIZUKU_PAL_MIDNIGHT; /* ��������������(��ď��������) */
    case 45:
        return SIZUKU_PAL_YUUGATA;  /* ��ď�ޡ��������(������������) */
    case 46:
        return SIZUKU_PAL_MIDNIGHT2; /* ��ď�ޡ��������(��ď��������) */
    case 47:
        return SIZUKU_PAL_YUUGATA;  /* ���������� ������������ */
    case 48:
        return SIZUKU_PAL_MIDNIGHT; /* ����������������ď��������) */
    case 49:
        return SIZUKU_PAL_YUUGATA;  /* ����������������Џ�������(������������) */
    case 50:
        return SIZUKU_PAL_MIDNIGHT; /* ����������������Џ�������(��ď��������) */
    case 51:
        return SIZUKU_PAL_YUUGATA; /* �����¡돫����������������(������������) */
    case 52:
        return SIZUKU_PAL_NIGHT;   /* �����¡돫����������������(������)   */
    case 53:
        return SIZUKU_PAL_DARK;    /* �����ԏ������������������� (��ď��������돪�)   */ 
    case 54:
	return SIZUKU_PAL_YUUGATA; /* ��������������������������������(������������)*/
    case 55:
	return SIZUKU_PAL_YUUGATA; /* ������������������������������(������������)*/
    default:
        return -1;
    }
    return no;
}

/*
 * ������������������������������������
 */
void
SizukuLoadBG(Lvns *lvns, int no)
{
#ifndef USE_MGL
    static u_char pal_default[][3] = {
        {0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},{0x00,0x00,0x00},
        {0xff,0xff,0xff},{0x33,0x44,0x44},{0x22,0x77,0x77},{0xaa,0xbb,0xff}, 
        {0x77,0x88,0xee},{0xff,0xff,0xaa},{0xee,0x00,0x33},{0x66,0x33,0x33},
        {0xbb,0x77,0x55},{0xff,0xbb,0xbb},{0xff,0xdd,0xdd},{0xff,0xff,0xff}};
#endif

    lvns->bg_type = LVNS_BACKGROUND;
    lvns->bg_no   = no;

    if (no == 0) {
        lvnsimage_clear(lvns->background);
#ifndef USE_MGL
        lvnsimage_set_palette(lvns->background, pal_default, 16);
#endif
    } else {
        /* ��������������������� */
        LvnsLoadBackground(lvns, "MAX_S%02d.LFG", bgmap(no));
        /* ��ġ�������������͏�������� */
#ifndef USE_MGL
        if ((no = palmap(no)) >= 0)
            lvnsimage_set_palette2(lvns->background, sizuku_haikei_palette[no], 4);
#else
        if ((no = palmap(no)) >= 0)
	    mglimage_set_palette(lvns->background, sizuku_haikei_palette_hsb[no]);
#endif
    }
    SizukuClearCharacter(lvns, 'a');
}

/*
 * ����� �������������������������������������������������������������
 */
void 
SizukuLoadVisual(Lvns *lvns, int no)
{
#ifndef USE_MGL
    static u_char pal21[][3] = {
        {0x00,0x00,0x00},{0x66,0x44,0x99},{0xaa,0x66,0xaa},{0xcc,0xaa,0xee},
        {0xff,0xff,0xff},{0xff,0xdd,0xee},{0x11,0x00,0x11},{0xee,0xbb,0x88}, 
        {0x11,0x00,0x33},{0x66,0x88,0xbb},{0x77,0x66,0xcc},{0xaa,0x99,0xff},
        {0xdd,0xdd,0xff},{0x11,0x00,0x55},{0xff,0xcc,0xdd},{0xff,0x00,0x00}};
#else
    static int pal21_hsb[3][16] = {
      {8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  8,  8,  8,  8,  8,  8},
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
    };
#endif
    lvns->bg_type = LVNS_VISUAL;
    lvns->bg_no   = no;
    if (no == 21) {
	LvnsLoadBackground(lvns, "VIS%02d.LFG", 2);
#ifndef USE_MGL
        lvnsimage_set_palette(lvns->background, pal21, 16);
#else
	mglimage_set_palette(lvns->background, pal21_hsb);
#endif
    } else {
	LvnsLoadBackground(lvns, "VIS%02d.LFG", no);
    }
    SizukuClearCharacter(lvns, 'a');
}

/*
 * ����� H���������������������������������������������������
 */
void
SizukuLoadHVisual(Lvns *lvns, int no)
{
    lvns->bg_type = LVNS_HCG;
    lvns->bg_no   = no;
    LvnsLoadBackground(lvns, "HVS%02d.LFG", no);
    SizukuClearCharacter(lvns, 'a');
}

/* 
 * ����� ������������������������������������������������������ 
 */
void
SizukuSetTextScenarioState(Lvns *lvns, int no)
{
    SizukuState *state = (SizukuState*)lvns->system_state;

    /* �����叩�������Ï����я���������� */
    if (!lvns->seen_mode && no >= state->seen_flag[lvns->scn_current]) {
        state->seen_flag[lvns->scn_current] = no+1;
		lvns->seen = False;          /* ����������� */
        if (!lvns->force_skip)
			lvns->skip      = False; /* �������������������ҏ�͏�� */
		lvns->fast_text = False;
    } else {
		lvns->seen = True;          /* �����叩���� */
		if (lvns->fast_when_seen)
			lvns->fast_text = True; /* ���������ď�������͏�����������������������͏�������������� */
	}
}

static
LvnsCommandInfo command[] = {
    { "select",  False },   /* ��������������쏪�������������������������������� */
    { "image",  False },    /* ����������䏪���͏�����             */
    { "save",   False },    /* �������������ď����򏪤���     */
    { "load",   False },    /* �������������ď�������������������� */
    { "before", False },    /* �������������쏪������������������������������ */
    { "title",  False },    /* ����������������������������������� */
    { NULL,  False }
};

LvnsCommandInfo *
SizukuGetCommandList(Lvns *lvns)
{
    command[0].enable =
	command[1].enable =
	command[2].enable =
	command[3].enable =
	command[4].enable =
	command[5].enable = False;
#if 0
    if (lvns->scn_func == SizukuScenarioEngine) {
        if (LVNSSTATE == LVNS_WAIT_KEY ||
            LVNSSTATE == LVNS_WAIT_PAGE ||
            LVNSSTATE == LVNS_WAIT_SELECT) {
            if (!lvns->select_mode) {
                command[0].enable = True;
            }
            command[1].enable = 
                command[2].enable =
                    command[3].enable =
                        command[4].enable =
                            command[5].enable = True;
        }
    }
#endif
    return command;
}

void
SizukuExecCommand(Lvns *lvns, const char *command)
{
    if (!strcmp(command, "select")) {
		LvnsSkipTillSelect(lvns);
    } else if (!strcmp(command, "image")) {
		LvnsImageMode(lvns);
    } else if (!strcmp(command, "save")) {
        SizukuSave(lvns);
    } else if (!strcmp(command, "load")) {
        SizukuLoad(lvns);
    } else if (!strcmp(command, "before")) {
        /* �����쏪������������������������� */
		SizukuState *state = (SizukuState*)lvns->system_state;
        lvns->savepoint = lvns->selectpoint;
        memcpy(state->flag_save, state->flag_select, sizeof state->flag);
    } else if (!strcmp(command, "title")) {
        /* ����������������������������������� */
		// SizukuToTitle(lvns);
    }
}

/*
 * ����󏪡��� VRAM ����������󏪢��� EUC ����������⏪������ҏ����������
 */
#ifndef USE_MGL
void
SizukuPutsVRAM(Lvns *lvns, int x, int y, const u_char *str, int idx) 
{
    int code;
    u_char *data;
    while (*str) {
        code = ((str[0]&0x7f)-33) * 94 + (str[1]&0x7f)-33;
        code = lvns->jis_to_leaf[code];
		if (code) {
			data = lvns->leaf_font_data + (code - 1) * 24 * 3;
			lvnsimage_put_pattern24_2(lvns->vram, x, y, idx, data);
		}
        x += 24;
        str += 2;
    }
}
#else
void
SizukuPutsVRAM(Lvns *lvns, int x, int y, const u_char *str, int idx) 
{
#define EDCOL 4
#define EDSHA 0
    int color;

    switch (idx) {
    case EDCOL:
      color = COLOR_WHITE;
      break;
    case EDSHA:
      color = COLOR_BLACK;
      break;
    default:
      dprintf((stderr, "Unknown index [%d]\n", idx));
    }
    mglimage_draw_string(lvns->vram, x, y, str, color);
}
#endif
#ifndef USE_MGL
static void
drawChar(Lvns *lvns, int x, int y, int code, int attr) 
{
    u_char *data = lvns->leaf_font_data + (code - 1) * 24 * 3;
    
    //getChar(&ft_manager,code);
    //u_char *data = ft_manager.bitmap_data;

    lvns->putPattern(lvns, x+1, y+1, SIZUKU_COL_BLACK, data);
    lvns->putPattern(lvns, x+2, y+1, SIZUKU_COL_BLACK, data);
    if (!attr)
        lvns->putPattern(lvns, x, y, SIZUKU_COL_WHITE, data);
    else
        lvns->putPattern(lvns, x, y, SIZUKU_COL_GRAY, data);
}
#else
static void
drawChar(Lvns * lvns, int x, int y, int code, int attr)
{
    static int black[] = {
	 COLOR_TRANSPARENT, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK
    };

    static int gray[] = {
	 COLOR_TRANSPARENT, 0x00000006, 0x00000007, 0x0000008
    };

    static int white[] = {
	 COLOR_TRANSPARENT, 0x00000008, 0x0000000c, 0x000000f
    };
#ifdef FONT_PLUS
    if( lvns->font != NULL){
        x += MGL_XOFFSET;
        y += MGL_YOFFSET;
        code--;

        lvnsfont_draw( lvns->font, x + 1, y + 1, code, black);
        if( !attr){
        	lvnsfont_draw( lvns->font, x, y, code, white);
        } else {
        	lvnsfont_draw( lvns->font, x, y, code, gray);
        }
     } else {
#endif /* FONT_PLUS */
    char buf[3];

#if 0
    if (lvns->skip && !attr)
      MglGetEvent(lvns, 1);
#endif

    buf[0] = lvns->leaf_to_euc[code * 2];
    buf[1] = lvns->leaf_to_euc[code * 2 + 1];
    buf[2] = 0;

    x += MGL_XOFFSET;
    y += MGL_YOFFSET;

    set_font(12, 0);

    set_color(COLOR_BLACK);
    draw_string(x + 1, y + 1, buf, DIR_NORTH);
    draw_string(x + 2, y + 1, buf, DIR_NORTH);

    if (!attr) {
	set_color(COLOR_WHITE);
    } else {
	set_color(COLOR_LIGHTGRAY);
    }
    draw_string(x, y, buf, DIR_NORTH);
#ifdef FONT_PLUS
    }
#endif /* FONT_PLUS */
}
#endif

extern MenuData sizuku_menu;

static LvnsSystemState sizuku_state_base = {

#ifndef USE_MGL
    640,
    400,
#else
    320,
    200,
#endif
    18,   /* XXX */
    "MAX_DATA.PAK",
    NULL,
    "scn%03d.dat",
    "sizfont.tbl",

	&sizuku_menu,

	SizukuStart,
	SizukuJingle,
	SizukuOpening,
	SizukuTitle,
	SizukuMain,

    SizukuMergeCharacter,
    SizukuClearScreen,
    SizukuLoadBG,
	SizukuGetCommandList,
    SizukuExecCommand,
    drawChar,
	SizukuDispHistory,

#if !defined(USE_MGL) || defined(FONT_PLUS)
    102, /* �����������Џ�������������� */
    103, /* ���������⏢������������������ */
#else
    1850, /* �����������Џ�������������� */
    1, /* ���������⏢������������������ */
#endif
};

void
SizukuInitialize(Lvns *lvns)
{
    lvns->system_state = malloc(sizeof(SizukuState));
    SizukuState *state = (SizukuState*)lvns->system_state;
    state->state     = sizuku_state_base;
    state->vram_char = lvnsimage_new(WIDTH, HEIGHT, 0, 0);
    SizukuSioriInit(lvns);
}

