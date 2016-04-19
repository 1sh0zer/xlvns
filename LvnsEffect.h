#ifndef __LvnsEffect_h_
#define __LvnsEffect_h_

#include "Lvns.h"

typedef enum {
    LVNS_EFFECT_NONE,
	LVNS_EFFECT_NO_EFFECT,         /* �ʤˤ⤷�ʤ� */
    LVNS_EFFECT_NORMAL,            /* ñ��ɽ�� */
    LVNS_EFFECT_FADE_MASK,         /* �ޥ����ե����� */
    LVNS_EFFECT_WIPE_TTOB,         /* �夫��磻�� */
    LVNS_EFFECT_WIPE_LTOR,         /* ������磻�� */
    LVNS_EFFECT_FADE_PALETTE,      /* Palette �ե����� */
    LVNS_EFFECT_WIPE_MASK_LTOR,    /* ������磻��(�ޥ���) */
    LVNS_EFFECT_FADE_SQUARE,       /* �Ҥ������ե����� */
    LVNS_EFFECT_WIPE_SQUARE_LTOR,  /* �Ҥ��������磻�� */
    LVNS_EFFECT_SLIDE_LTOR,        /* ������ĥ��饤�� */
    LVNS_EFFECT_SLANTTILE,         /* �Ф��첣�⥶�������饤�� */
    LVNS_EFFECT_GURUGURU,          /* ���뤰�� */
    LVNS_EFFECT_VERTCOMPOSITION,   /* �������ǤϤ��ߤ��� */
    LVNS_EFFECT_CIRCLE_SHRINK,     /* �濴�����ˤ����� */
    LVNS_EFFECT_LEFT_SCROLL,       /* ���˥������� */
    LVNS_EFFECT_TOP_SCROLL,        /* ��˥������� */
    LVNS_EFFECT_RAND_RASTER,       /* �������ɽ�� */
    LVNS_EFFECT_BLOOD,             /* �줬�������   */
} LvnsEffectType;                  /* ���ե����ȼ��� */

/* LvnsEffect.c */
void LvnsClearLow(Lvns *lvns, LvnsEffectType effect_clear);
void LvnsDispLow(Lvns *lvns, LvnsEffectType effect_disp);
const char* LvnsEffectName(LvnsEffectType type);

void LvnsClear(Lvns *lvns, LvnsEffectType effect_clear);
void LvnsDisp(Lvns *lvns, LvnsEffectType effect_disp);

void LvnsLighten(Lvns *lvns);
void LvnsDarken(Lvns *lvns);
void LvnsVibrato(Lvns *lvns);
void LvnsWhiteOut(Lvns *lvns);
void LvnsWhiteIn(Lvns *lvns);


#endif
