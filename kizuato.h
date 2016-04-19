/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: kizuato.h,v 1.23 2001/08/05 10:58:09 go Exp $
 *
 */

#ifndef __kizuato_h
#define __kizuato_h

#include "Lvns.h"
#include "LvnsEffect.h"
#include "LvnsAnim.h"
#include "LvnsScript.h"

#define KIZUATO_SCN_NO 177   /* ���ʥꥪ�θĿ� */
#define KIZUATO_FLAG_NO 28  /* �ե饰�θĿ�   */

#define KIZUATO_COL_BLACK 0
#define KIZUATO_COL_WHITE 16
#define KIZUATO_COL_GRAY  17

#define KIZUATO_COLOR_REVERSE 100

typedef struct {
    LvnsSystemState state;

    int seen_flag[KIZUATO_SCN_NO];  /* ���ʥꥪ���ɥե饰         */
    int ending[17];            /* ����ǥ��󥰤�ߤ����ɤ��� */
    int flag[KIZUATO_FLAG_NO];   /* ���ʥꥪ�����ѥե饰 */
    int flag_save[KIZUATO_FLAG_NO];   /* ���ʥꥪ�����ѥե饰 */
    int flag_select[KIZUATO_FLAG_NO];   /* ���ʥꥪ�����ѥե饰 */

    int change_palette;   /* �����ѥ�å��ִ��⡼�� */
    int reverse_palette;  /* ��ȿž */
    int change_palette2;  /* ɽ�����˥ѥ�å��ִ����ϡ� */

    LvnsImage *vram_char;  /* ����饯�����������    */
    int siori_no;          /* ���ߤΤ������ֹ�        */

    /* OP�Ѷ��̺�ȥǡ��� */
    LvnsImage *chizuru;
    LvnsImage *azusa;
    LvnsImage *kaede;
    LvnsImage *hatsune;
    LvnsImage *moon;
    LvnsImage *shadow;
    int moon_pos;

} KizuatoState;

void KizuatoScenarioInit(Lvns *lvns);
void KizuatoSioriInit(Lvns *lvns);
void KizuatoLoad(Lvns *lvns);
void KizuatoSave(Lvns *lvns);

void KizuatoStart(Lvns *lvns);
void KizuatoJingle(Lvns *lvns);
void KizuatoOpening(Lvns *lvns);
void KizuatoTitle(Lvns *lvns);
void KizuatoMain(Lvns *lvns);
void KizuatoEnding(Lvns *lvns);

void KizuatoMergeCharacter(Lvns *lvns);
void KizuatoClearScreen(Lvns *lvns);

void KizuatoLoadCharacter(Lvns *lvns, int no, int pos);
void KizuatoClearCharacter(Lvns *lvns, int pos);
void KizuatoLoadBG(Lvns *lvns, int no);
void KizuatoLoadVisual(Lvns *lvns, int no);
void KizuatoLoadHVisual(Lvns *lvns, int no);

void KizuatoDisp(Lvns *lvns, LvnsEffectType effect_disp);

void KizuatoLoadScenarioText(Lvns *lvns, int no);

void KizuatoPutsVRAM(Lvns *lvns, int x, int y, const u_char *str, int idx);

void KizuatoDispHistory(Lvns *lvns, int pos);

#endif

