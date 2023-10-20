/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: LvnsBackEffect.c,v 1.4 2001/07/25 14:36:48 tf Exp $
 */

/*
 * Lvns �ط��ü쥨�ե����Ƚ���
 * ����Ū�ˤϥ⥸�塼�벽
 *
 * ���Ū���طʤǼ�ưŪ�ˤ����ʤ��륨�ե����Ȥ�������롣
 *
 * ���� VRAM(lvns->vram)���ط�(lvns->background)�򥳥ԡ�����
 * �ݤ˥��ե����Ȥ�ä��뤳�Ȥ��Ǥ��롣
 * �طʽ�����Υ���饯���ι����� lvns->mergeCharacter
 * (lvns)�����ѤǤ���
 */

#include <stdio.h>
#include <stdlib.h>
#include "Lvns.h"

/*
 * ���ե����Ƚ����¹�(��������)
 */
void
LvnsBackEffectSetState(Lvns *lvns)
{
    if (lvns->effect_back) 
        lvns->effect_back->set(lvns);
}

/*
 * ���ե����Ƚ����¹�(�������)
 */
void
LvnsBackEffect(Lvns *lvns)
{
    if (lvns->effect_back) {
        lvns->effect_back->func(lvns);
    } else {
		lvnsimage_copy(lvns->background, lvns->vram);
        LVNS->mergeCharacter(lvns);
    }
}

/*
 * �طʥ��ե����Ȥμ���λ���
 */
void
LvnsSetBackEffect(Lvns *lvns, LvnsBackEffectInfo *info)
{
    lvns->effect_back = info;
    lvns->effect_back_state = 0;
}

/*
 * ����ɽ�������ߥ󥰤�����طʥ��ե����Ȥμ���λ���
 */
void
LvnsSetNextBackEffect(Lvns *lvns, LvnsBackEffectInfo *info)
{
    lvns->effect_back_next = info;
}
