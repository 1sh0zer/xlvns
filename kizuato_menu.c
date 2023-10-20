/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: kizuato_menu.c,v 1.10 2001/08/05 11:46:02 go Exp $
 *
 */

/*
 * �� �����������˥塼ɽ��
 */

#include <stdio.h>
#include "kizuato.h"


static MenuLine load_confirm_menu_line[] = {
    MENULINE(4, "���ɤ��ޤ���", 0),
    MENULINE(5, "������Ǥ�����", 0),
    MENULINE(7, "�Ϥ�",1),
    MENULINE(8, "������", 2),
	MENULINE(0, NULL, 0),
};

static MenuData load_confirm_menu = {
	load_confirm_menu_line,
	2, 0, 
	NULL,
};

static MenuLine save_confirm_menu_line[] = {
	MENULINE(4, "�����֤��ޤ���", 0),
	MENULINE(5, "������Ǥ�����", 0),
    MENULINE(7, "�Ϥ�",1),
    MENULINE(8, "������", 2),
	MENULINE(0, NULL, 0),
};

static MenuData save_confirm_menu = {
	save_confirm_menu_line,
	2, 0, 
	NULL,
};

static MenuLine end_confirm_menu_line[] = {
    MENULINE(4, "��λ���ޤ���", 0),
    MENULINE(5, "������Ǥ�����", 0),
    MENULINE(7, "�Ϥ�",1),
    MENULINE(8, "������", 2),
	MENULINE(0, NULL, 0),
};

static MenuData end_confirm_menu = {
	end_confirm_menu_line,
	2, 0, 
	NULL,
};

/* 
 * ����˥塼����
 */
static MenuLine kizuato_menu_line[] = {
    MENULINE(3, "ʸ����ä�",1),
    MENULINE(4, "���ɤ���", 2),
    MENULINE(5, "�����֤���", 3),
    MENULINE(6, "���ʥꥪ����", 4),
    MENULINE(7, "����������������", 5),
    MENULINE(8, "�����ཪλ", 6),
	MENULINE(0, NULL, 0),
};

static int
KizuatoMenuEngine(Lvns *lvns, int select)
{
    KizuatoState *state = (KizuatoState *)lvns->system_state;
    switch (select) {

    case 1:
		/* ����ɽ���⡼�� */
		LvnsImageMode(lvns);
		break;

    case 2:
		/* ���� */
		switch (LvnsMenu(lvns, &load_confirm_menu, True)) {
		case -1: /* cancel */
			return 0;
		case 1:
			/* ���ɽ��� */
			KizuatoLoad(lvns);
			LvnsClear(lvns, LVNS_EFFECT_WIPE_TTOB);
			LvnsJump(lvns, LVNS_JUMP_RESTART);
		case 2:			
			break;
		}
		break;

    case 3:
        /* ������ */
		switch (LvnsMenu(lvns, &save_confirm_menu, True)) {
		case -1: /* cancel */
			return 0;
		case 1:
			/* �����ֽ��� */
			KizuatoSave(lvns);
		case 2:			
			break;
		}
		break;

	case 4:
		/* ���ۥ⡼�� */
		LvnsHistoryMode(lvns);
		break;

    case 5:
        /* ��������� */
        lvns->savepoint = lvns->selectpoint;
        memcpy(state->flag_save, state->flag_select, sizeof state->flag);
		LvnsClear(lvns, LVNS_EFFECT_WIPE_TTOB);
		LvnsJump(lvns, LVNS_JUMP_RESTART);

    case 6:
        /* �����ȥ����� */
		switch (LvnsMenu(lvns, &end_confirm_menu, True)) {
		case -1: /* cancel */
			return 0;
		case 1:
			LvnsClear(lvns, LVNS_EFFECT_WIPE_TTOB);
			LvnsJump(lvns, LVNS_JUMP_TITLE);
		case 2:			
			break;
		}
		break;

    default:;
    }
	return select;
}

MenuData kizuato_menu = {
	kizuato_menu_line,
	5, 0, 
	KizuatoMenuEngine
};



