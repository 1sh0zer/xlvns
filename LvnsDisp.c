/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: LvnsDisp.c,v 1.16 2001/08/11 19:59:36 tf Exp $
 *
 */

/*
 * Lvns ɽ����������
 */

#include <stdio.h>
#include <string.h>
#include "Lvns.h"
#include "LvnsEffect.h"

/**
 * ���ߤΥƥ����ȥ쥤������������褹��
 */
static void
DrawTextLayerAll(Lvns *lvns)
{
    int i, j;
    for (i=0; i<TEXT_HEIGHT;i++) {
        for (j=0; j<TEXT_WIDTH; j++) {
            struct TextVram *c = 
                &lvns->tvram[lvns->current_tvram].row[i].column[j];
            if (c->code) {
                int x = XPOS(j,i);
                int y = YPOS(i);
				LVNS->drawChar(lvns, x, y, c->code, c->attribute);
            }
        }
    }
}

/*
 * ��������ξõ�
 */
void
LvnsClearCursor(Lvns *lvns)
{
    if (lvns->cursor_state) {
        int x = XPOS(lvns->tvram[lvns->current_tvram].o_cur_x, 
                     lvns->tvram[lvns->current_tvram].o_cur_y);
        int y = YPOS(lvns->tvram[lvns->current_tvram].o_cur_y); 
        lvns->drawWindowArea(lvns, x, y, CHARDRAWSIZE, CHARDRAWSIZE, x, y);
        lvns->flushWindowArea(lvns, x, y, CHARDRAWSIZE, CHARDRAWSIZE);
        lvns->cursor_state = 0;
    }
}

/*
 * �������������
 */
void
LvnsDrawCursor(Lvns *lvns, LvnsCursorType cursor_type)
{       
    int cursor;

    if (cursor_type == LVNS_CURSOR_PAGE) {
        cursor = LVNS->cursor_page;
    } else
        cursor = LVNS->cursor_key;

    if (lvns->cursor_state == 0) {
        int x = XPOS(lvns->tvram[lvns->current_tvram].cur_x,
                     lvns->tvram[lvns->current_tvram].cur_y);
        int y = YPOS(lvns->tvram[lvns->current_tvram].cur_y);

        LVNS->drawChar(lvns, x, y, cursor, 0);
        lvns->flushWindowArea(lvns, x, y, CHARDRAWSIZE, CHARDRAWSIZE);

        lvns->tvram[lvns->current_tvram].o_cur_x = 
            lvns->tvram[lvns->current_tvram].cur_x;
        lvns->tvram[lvns->current_tvram].o_cur_y = 
            lvns->tvram[lvns->current_tvram].cur_y;
        
        lvns->cursor_state = 1;

    } else {
        LvnsClearCursor(lvns);
    }
}

/**
 * �ƥ����ȥ�������ξõ�
 */
void
LvnsClearTextCursor(Lvns *lvns)
{
    int i, j;
    for (i=0; i<TEXT_HEIGHT;i++) {
        for (j=0; j<TEXT_WIDTH; j++) {
            struct TextVram *c = 
                &lvns->tvram[lvns->current_tvram].row[i].column[j];
            if (c->code && c->attribute == lvns->text_cursor_state) {
                int x = XPOS(j,i);
                int y = YPOS(i);
                LVNS->drawChar(lvns, x, y, c->code, 1);
                lvns->flushWindowArea(lvns, x, y, CHARDRAWSIZE, CHARDRAWSIZE);
            }
        }
    }
}

/**
 * �ƥ����ȥ��������ɽ��
 */
void
LvnsDrawTextCursor(Lvns *lvns)
{
    int i, j;
    for (i=0; i<TEXT_HEIGHT;i++) {
        for (j=0; j<TEXT_WIDTH; j++) {
            struct TextVram *c = 
                &lvns->tvram[lvns->current_tvram].row[i].column[j];
            if (c->code && c->attribute == lvns->text_cursor_state) {
                int x = XPOS(j,i);
                int y = YPOS(i);
                LVNS->drawChar(lvns, x, y, c->code, 0);
                lvns->flushWindowArea(lvns, x, y, CHARDRAWSIZE, CHARDRAWSIZE);
            }
        }
    }
}

/**
 * ���ߤβ���(�ط� + ʸ��)�� ���� vram �������褵����
 */
void
LvnsDispWindow(Lvns *lvns)
{
	/* ����ե��å��� */
    lvns->setPaletteMulti(lvns, lvns->latitude);
    lvns->drawWindow(lvns);

	/* �ɲ�ɽ������(��˥塼�ʤɤ�����) */
	if (lvns->disp_func) 
		lvns->disp_func(lvns, lvns->disp_func_param);

	/* �ƥ�����ɽ�� */
    if (lvns->text_mode) {
		DrawTextLayerAll(lvns);
    }

	lvns->flushWindow(lvns);
}

/* ʸ���õ���� */
void
LvnsUndispText(Lvns *lvns)
{
	LvnsFlip(lvns, True); // ����Ʊ���Τ����
	lvns->text_mode = False;
	if (lvns->skip) {
		lvns->latitude = 16;
		LvnsDispWindow(lvns);
	} else {
		LvnsLighten(lvns);
	}
}

/* ʸ����ɽ������ */
void
LvnsDispText(Lvns *lvns)
{
	if (!lvns->text_mode) {
		lvns->text_mode = True;
		if (lvns->skip) {
			lvns->latitude  = lvns->latitude_dark;
		} else {
			LvnsDarken(lvns);
		}
		LvnsDispWindow(lvns);
		LvnsFlip(lvns, True); // ����Ʊ���Τ����
	}
}

/**
 * �طʤȥ���饯���ξ��֤���������������ɽ������
 * ɽ���夷�Ф餯�Ԥ�������
 */
void
LvnsDisp(Lvns *lvns, LvnsEffectType effect)
{
	/* BGM �ѹ������å� */
	LvnsStartNextMusic(lvns);

	/* �طʥ��ե����Ȼ��� */
	if (lvns->effect_back_next) {
		lvns->effect_back = lvns->effect_back_next;
		lvns->effect_back_next = NULL;
	}

	/* ������VRAM������ */
	lvnsimage_copy(lvns->background, lvns->vram);
	LVNS->mergeCharacter(lvns);

	/* ���ԥ�Ĵɽ���ʤ�ѥ�åȽ��� */
	if (lvns->sepia_mode) {
	  lvnsimage_set_sepia(lvns->vram);
	}

	if (lvns->skip) {
		LvnsDispWindow(lvns);
	} else {
		LvnsDispLow(lvns, effect);
		LvnsWait(lvns, 500 * INTERVAL / 1000);
	}
}

/** 
 * �õ������Ԥ�
 * 
 */
void
LvnsClear(Lvns *lvns, LvnsEffectType effect) 
{
	if (lvns->skip) {
		lvnsimage_clear(lvns->vram);
		LvnsDispWindow(lvns);
	} else {
		LvnsClearLow(lvns, effect);
	}
	LvnsSetBackEffect(lvns, NULL);
}

/**
 * ľ�ܥƥ���������
 */
void
LvnsDrawString(Lvns *lvns, int x, int y, const u_char *str, int attr)
{
    int code;  
    while (*str) {
        code = ((str[0]&0x7f)-33) * 94 + (str[1]&0x7f)-33;
        code = lvns->jis_to_leaf[code];
		if (code)
			LVNS->drawChar(lvns, x, y, code, attr);
        str += 2;
        x += CHARSIZE;
    }
}

/*
 * ľ�ܥƥ���������(���󥿥��)
 */
void
LvnsDrawStringCenter(Lvns *lvns, int y, const u_char *str, int attr)
{
	LvnsDrawString(lvns, (WIDTH - strlen(str)/2 * CHARSIZE)/2,
				   y, str, attr);
}

void
LvnsTextOff(Lvns *lvns)
{
	lvns->text_mode = False;
}

void
LvnsTextOn(Lvns *lvns)
{
	lvns->text_mode = True;
}
