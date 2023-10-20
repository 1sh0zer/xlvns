/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: LvnsText.c,v 1.14 2001/08/03 01:55:25 go Exp $
 *
 */

/*
 * Lvns �ƥ����Ƚ����ޤ��
 */

#include <stdio.h>
#include <stdlib.h>
#include "Lvns.h"
#include "LvnsEffect.h"

/*
 * ʸ�����襫����������ѹ�
 */
void
LvnsLocate(Lvns *lvns, int x, int y)
{
    lvns->tvram[lvns->current_tvram].cur_x = x;
    lvns->tvram[lvns->current_tvram].cur_y = y;
}

static void
drawChar(Lvns *lvns, int x, int y, int code, int attr)
{
}

/*
 * ʸ��ɽ�������롼����
 */
static void
PutChar(Lvns *lvns, int code, int attr)
{
	int i,j;
	struct TextVram *c;

	c = &lvns->tvram[lvns->current_tvram].row[i = lvns->tvram[lvns->current_tvram].cur_y].column[j = lvns->tvram[lvns->current_tvram].cur_x];
    c->code      = code;
    c->attribute = attr;

    lvns->tvram[lvns->current_tvram].cur_x++;

	if (code) {
		int x = XPOS(j, i);
		int y = YPOS(i);
		LVNS->drawChar(lvns, x, y, code, attr);
		lvns->flushWindowArea(lvns, x, y, CHARDRAWSIZE, CHARDRAWSIZE);
	}

#ifdef USE_MGL
    if (lvns->tvram[lvns->current_tvram].cur_x >= TEXT_WIDTH - 1) {
      lvns->tvram[lvns->current_tvram].cur_x = 0;
      lvns->tvram[lvns->current_tvram].cur_y++;
    }
#endif
}

/* 
 * ��ʸ��ɽ�� (LeafCode��)
 */
void
LvnsPutChar(Lvns *lvns, int c, int attr)
{
    PutChar(lvns, c, attr);

    /* ʸ���ѹ� */
    lvns->text_written = True;

    /* ʸ��ñ�̤��Ԥ����ַ��� */
    if (!lvns->skip &&	
		!(lvns->select && lvns->key_click_fast) &&
		!lvns->fast_text) {
		LvnsWait(lvns, lvns->char_wait_time);
    }
}


/* 
 * ��ʸ��ɽ�� (EUC��)
 */
void
LvnsPutCharNormal(Lvns *lvns, int c, int attr)
{
    PutChar(lvns, lvns->jis_to_leaf[EucToJisPack(c)], attr);

    /* ʸ���ѹ� */
    lvns->text_written = True;

    /* ʸ��ñ�̤��Ԥ����ַ��� */
    if (!lvns->skip &&
		!(lvns->select && lvns->key_click_fast) &&
		!lvns->fast_text) {
		LvnsWait(lvns, lvns->char_wait_time);
    }
}

/*
 * ʸ����ɽ��(EUCʸ��������)
 */
void
LvnsPuts(Lvns *lvns, const u_char *str, int attr)
{
    int code;
    while (*str) {
        code = ((str[0]&0x7f)-33) * 94 + (str[1]&0x7f)-33;
        PutChar(lvns, lvns->jis_to_leaf[code], attr);
        str += 2;
    }
    lvns->text_written = True;
}

/*
 * �ƥ����Ȥξõ�
 */
void
LvnsClearText(Lvns *lvns)
{
    int i, j;
    for (i=0; i<TEXT_HEIGHT;i++) {
	lvns->tvram[lvns->current_tvram].row[i].offset = 16;
	for (j=0; j<TEXT_WIDTH; j++) {
	    struct TextVram *c = 
		&lvns->tvram[lvns->current_tvram].row[i].column[j];
	    c->code = 0;
	    c->attribute = 0;
	}
    }
    lvns->tvram[lvns->current_tvram].cur_x = 0;
    lvns->tvram[lvns->current_tvram].cur_y = 0;
    lvns->text_written = False;
}

/*
 * �Ԥ�ɽ�����ե��åȤ����
 */
void
LvnsSetTextOffset(Lvns *lvns, int offset)
{
    int i;
#ifdef USE_MGL
    offset /= 2;
#endif
    for (i= lvns->tvram[lvns->current_tvram].cur_y; i < TEXT_HEIGHT; i++)
        lvns->tvram[lvns->current_tvram].row[i].offset = offset;
}

/*
 * ���Խ���
 */
void
LvnsNewLineText(Lvns *lvns)
{
    lvns->tvram[lvns->current_tvram].cur_x = 0;
    lvns->tvram[lvns->current_tvram].cur_y++;
    lvns->char_wait_time = 1;

#ifdef USE_MGL
    if (lvns->skip)
      MglGetEvent(lvns, 5);
#endif
}





