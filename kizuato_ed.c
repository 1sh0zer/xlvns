/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: kizuato_ed.c,v 1.17 2001/08/02 20:05:30 go Exp $
 *
 */

/*
 * �� ����ǥ���
 */

#include <stdio.h>
#include "kizuato.h"

#define EDCOL 4
#define EDSHA 0

static int
tputs(Lvns *lvns, LvnsScript *scr, void *param1, void *param2)
{
    char *str = param1;
    int x = (WIDTH2 - strlen(str) * CHARSIZE/2)/2;
    int y = (int)param2 * EDYOFF;
    KizuatoPutsVRAM(lvns, x+1, y+1, (u_char*)param1, EDSHA);
    KizuatoPutsVRAM(lvns, x+2, y+2, (u_char*)param1, EDSHA);
    KizuatoPutsVRAM(lvns, x  , y  , (u_char*)param1, EDCOL);
    return True;
}

static int
DispMoji(Lvns *lvns, LvnsScript *scr, 
         void *param1, void *param2)
{
    lvns->drawWindow(lvns);
    lvns->flushWindow(lvns);
    return True;
}

/* �� ����ǥ��󥰥�����ץ� */
static LvnsScriptData eddata[] = {
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_BG, (void*)1 },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "���", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "�ⶶ��ζ��", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)6 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "����", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "�ⶶ��ζ��", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)7 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "����饯��������", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "��ɧ��ζ��", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG,(void*) 8 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR,(void*) LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "����", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "��̵�Ű", (void*) 7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)9 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "�ץ����", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "�ȣ��ʣɣͣš��ΣɣΣϣͣ���", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)12 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "�ӥ��奢�륰��ե��å���", (void*)4 },
    { LVNS_SCRIPT_FUNC, tputs, "Ļ�����", (void*)6 },
    { LVNS_SCRIPT_FUNC, tputs, "�顼���٣ϣ�", (void*)7 },
    { LVNS_SCRIPT_FUNC, tputs, "����̴", (void*)8 },
    { LVNS_SCRIPT_FUNC, tputs, "�ģϣڣ�", (void*)9 },
    { LVNS_SCRIPT_FUNC, tputs, "�ˣţΣˣţ�", (void*)10 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)15 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "����饯��������ե��å���", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "Ļ�����", (void*)6 },
    { LVNS_SCRIPT_FUNC, tputs, "�顼���٣ϣ�", (void*)7 },
    { LVNS_SCRIPT_FUNC, tputs, "����̴", (void*)8 },
    { LVNS_SCRIPT_FUNC, tputs, "�ˣţΣˣţ�", (void*)9 },
    { LVNS_SCRIPT_FUNC, tputs, "�ģϣڣ�", (void*)10 }, 
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)16 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "�طʥ���ե��å���", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "Ļ�����", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)17 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "����", (void*)4 },
    { LVNS_SCRIPT_FUNC, tputs, "���ľ��", (void*)6 },
    { LVNS_SCRIPT_FUNC, tputs, "�������", (void*)7 },
    { LVNS_SCRIPT_FUNC, tputs, "��塡�±�", (void*)8 },
    { LVNS_SCRIPT_FUNC, tputs, "�ⶶ��ζ��", (void*)9 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)19 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP,(void*) LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "��������", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "�շͥ��", (void*)7 },
    { LVNS_SCRIPT_FUNC, tputs, "��̵�Ű", (void*)8 },
    { LVNS_SCRIPT_FUNC, tputs, "��塡�±�", (void*)9 },
    { LVNS_SCRIPT_FUNC, tputs, "����̴", (void*)10 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)24 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 }, 
    { LVNS_SCRIPT_FUNC, tputs, "���ꥸ�ʥ�ե����", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "�졡��", (void*)7 },
    { LVNS_SCRIPT_FUNC, tputs, "��̵�Ű", (void*)8 },
    { LVNS_SCRIPT_FUNC, tputs, "�ȣ��ͣͣţ�", (void*)9 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)27 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP,(void*) LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "���硼�ȥ��ʥꥪ����", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "�ġ���", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)45 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "�ԣţӣԡ��У̣���", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "���̡̣��̣ţ��ơ��ӣԣ��ƣ�", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)11 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR,(void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "��ȯ", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "�����������̣ţ���", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },
    { LVNS_SCRIPT_WAIT, (void*)2000 },

    { LVNS_SCRIPT_WAIT_CLICK },
    { LVNS_SCRIPT_CLICK_JUMP },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_END }
};

void
KizuatoEnding(Lvns *lvns)
{
	LvnsScriptRun(lvns, eddata);
}
