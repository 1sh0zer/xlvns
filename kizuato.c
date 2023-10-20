/*
 * leaf Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: kizuato.c,v 1.52 2001/08/11 19:59:36 tf Exp $
 *
 */

/*
 * �� ���ʥꥪ�������󥸥�
 */

#include <stdio.h>
#include "kizuato.h"

/* ���ͤ��� sin_effect.h */
extern LvnsBackEffectInfo lvnsSinEffect2;

/* �ü쥨�ե����ȡ֤ȤӤ����� */
static LvnsAnimationData kizuato01[] = {
    { LVNS_ANIM_IMAGE, "BLDW0.LFG", 100, 0, 0 },
    { LVNS_ANIM_IMAGE, "BLDW1.LFG", 100, 0, 0 },
    { LVNS_ANIM_IMAGE, "BLDW2.LFG", 100, 0, 0 },
    { LVNS_ANIM_IMAGE, "BLDW3.LFG", 100, 0, 0 },
    { LVNS_ANIM_WAIT, NULL, 100 },
    { LVNS_ANIM_NONE, NULL, 0 }
};

/* �ü쥨�ե����ȡֵ����ޡ� */
static LvnsAnimationData kizuato02[] =  {
    { LVNS_ANIM_IMAGE, "CLAW01W.LFG", 100, 0, 0 },
    { LVNS_ANIM_IMAGE, "CLAW02W.LFG", 100, 0, 0 },
    { LVNS_ANIM_IMAGE, "CLAW03W.LFG", 100, 0, 0 },
    { LVNS_ANIM_IMAGE, "CLAW04W.LFG", 100, 0, 0 },
    { LVNS_ANIM_WAIT, NULL, 100 },
    { LVNS_ANIM_NONE, NULL, 0 }
};

/* �ü쥨�ե����ȡֿ᤭�Ф��� */
static LvnsAnimationData kizuato03[] =  {
    { LVNS_ANIM_IMAGE, "CLAW05W.LFG", 100, 0, 0 },
    { LVNS_ANIM_IMAGE, "CLAW06W.LFG", 100, 0, 0 },
    { LVNS_ANIM_IMAGE, "CLAW07W.LFG", 100, 0, 0 },
    { LVNS_ANIM_IMAGE, "CLAW08W.LFG", 100, 0, 0 },
    { LVNS_ANIM_IMAGE, "CLAW09W.LFG", 100, 0, 0 },
    { LVNS_ANIM_IMAGE, "CLAW10W.LFG", 100, 0, 0 },
    { LVNS_ANIM_IMAGE, "CLAW11W.LFG", 100, 0, 0 },
    { LVNS_ANIM_WAIT, NULL, 100 },
    { LVNS_ANIM_NONE, NULL, 0 }
};

/**
 * �ƥ������ѥ��ե����ȥѥ�᡼���Ѵ�
 */
static LvnsEffectType
text_effect(int no)
{
    static LvnsEffectType kizuato_effect[] =  {
/**/    LVNS_EFFECT_FADE_PALETTE,
/**/    LVNS_EFFECT_GURUGURU,
        LVNS_EFFECT_FADE_MASK,
/**/    LVNS_EFFECT_FADE_SQUARE,
        LVNS_EFFECT_WIPE_SQUARE_LTOR,
/**/    LVNS_EFFECT_FADE_MASK,
        LVNS_EFFECT_WIPE_TTOB,
/**/        LVNS_EFFECT_WIPE_LTOR,
/**/        LVNS_EFFECT_WIPE_MASK_LTOR,
        LVNS_EFFECT_VERTCOMPOSITION,

/**/        LVNS_EFFECT_SLIDE_LTOR,
/**/        LVNS_EFFECT_NORMAL,
        LVNS_EFFECT_FADE_MASK,
        LVNS_EFFECT_CIRCLE_SHRINK,
        LVNS_EFFECT_NONE,
        LVNS_EFFECT_NONE,
        LVNS_EFFECT_NONE,
        LVNS_EFFECT_LEFT_SCROLL,
/**/        LVNS_EFFECT_FADE_PALETTE, /* ��®��? */
        LVNS_EFFECT_RAND_RASTER,
    };

    LvnsEffectType ret;
    ret = kizuato_effect[no];
    dprintf((stderr, "(%2d)%s\n", no, LvnsEffectName(ret)));
    return ret;
}

/*
 * bgm �ѥ�᡼���Ѵ�
 */
static int
bgmmap(int no)
{
    switch (no) {
    default:
        return no+2;
    }
}

/*
 * �ե饰
  0: ���Ȳ��ä���
  1: ��ʬ���ȿͤǤϤʤ�
  2: ���ᤵ��򵿤�
  3: ���Ȼ��︽��ˤफ�ä�
  4: �����ʥꥪ�ٻ�������
  5: ����ӥ�����ä�
  6: ������ä�
  7: �鲻�����򤳤郎�餻�ʤ�
  8: ���ޤ���錄����
  9: ̤����?
  
  a-13  ����ǥ��󥰸�ե饰������ (�����ǽ���������)

  14: ����BAD����ɤ򸫤�
  15: ���ᥨ��ǥ��󥰤򸫤�
  16: �� Happy �򸫤�
  17: �� BAD �򸫤�
  18: �� HAPPY �򸫤� 
  19: �����ɤ򸫤�
  1a: �鲻����ɤ򸫤�

  73: ������ǥ��󥰤򸫤����ɤ���
*/
static int
flagmap(int no)
{
    if (no == 0x73)
        return 0x1b;
    else
        return no;
}

/**
 * ���ʥꥪ�����
 */
void
KizuatoScenarioInit(Lvns *lvns)
{
    int i;
    KizuatoState *state = (KizuatoState *)lvns->system_state;

    /* �����־��־��ֽ���� */
    LvnsInitSavePoint(lvns, &lvns->savepoint);

    /* ���ʥꥪ�ե饰����� */
    for (i=0; i<10;i++) {
        state->flag_save[i] = 0;
    }
}

/**
 * ���������� (���ե饰�õ�)
 */
void
KizuatoSioriInit(Lvns *lvns)
{
    int i;
    KizuatoState *state = (KizuatoState *)lvns->system_state;

    dprintf((stderr, "Siori Init\n"));
    
    /* ���ʥꥪ�ǡ�������� */
    KizuatoScenarioInit(lvns);

    /* ̤���ե饰���õ� */
    for (i=0; i<KIZUATO_SCN_NO;i++) {
        state->seen_flag[i] = 0;
    }
    
    /* �Ĥ������ե饰�ξõ� */
    for (i=0; i<KIZUATO_FLAG_NO;i++) {
        state->flag_save[i] = 0;
    }

    /* ����ǥ��󥰾��־õ� */
    for (i=0; i<sizeof state->ending;i++) {
	state->ending[i] = 0;
    }

    state->reverse_palette = 0;
    state->change_palette  = 0;
    state->change_palette2 = 0;
}

/*
 * �� ���ޥ�ɽ���
 */
#define c (*p)
static int
CommandParser(Lvns *lvns, const u_char **p, int history_mode) 
{
    KizuatoState *state = (KizuatoState *)lvns->system_state;

    switch (c[0]) {
    case 0x30:		/* �طʥ��ɤΤ� */
		dprintf((stderr, "[0x30: ����̵���طʥ��� %d]\n", c[1]));
		if (!history_mode) {
			KizuatoLoadBG(lvns, c[1]);
		}
        c += 2;
        break;
       
    case 0x31:
        dprintf((stderr, "[0x31: �� ���ե����Ȼ��� (%x)?\n",c[1]));
        c += 2;
        break;
        
    case 0x34:
        dprintf((stderr, "[0x34: ���̾õ�?(%x)\n", c[1]));
		if (!history_mode) {
			LvnsClear(lvns, text_effect(c[1]));
		}
        c += 2;
        break;

    case 0x35:
        dprintf((stderr, "[0x31: �� ���ե����Ȼ���? (%x)\n", c[1]));
        c += 2;
        break;

    case 0x36:
		dprintf((stderr, "[0x36: ����̵��Visual���� %d]\n", c[1]));
		if (!history_mode) {
			KizuatoLoadVisual(lvns, c[1]);
		}
        c += 2;
        break;
        
    case 0x37:
        dprintf((stderr, "[0x37: ����̵������饯������ %d]\n", c[2]));
		if (!history_mode) {
			KizuatoLoadCharacter(lvns, c[2], c[1]);
		}
        c += 3;
        break;
        
    case 0x38:
        dprintf((stderr, "[0x38: ľ��ɽ������ %d %d]\n", c[1], c[2]));
		if (!history_mode) {
			switch (c[1]) {
			case 0:
				KizuatoDisp(lvns, LVNS_EFFECT_NORMAL);
				break;
			case 1:
				KizuatoDisp(lvns, LVNS_EFFECT_NORMAL);
				break;
			case 2:
				KizuatoDisp(lvns, LVNS_EFFECT_FADE_MASK);
				break;
			}
		}
        c += 3;
        break;

    case 0x39:
        dprintf((stderr, "[0x39: ɽ������ %d]\n", c[1]));
		if (!history_mode) {
			KizuatoDisp(lvns, text_effect(c[1]));
		}
        c += 2;
        break;
        
    case 0x3a:
        dprintf((stderr, "[0x3a: �ѥ�åȰ���ѹ�����(¨��ȿ��)(%d)?\n",c[1]));
		if (!history_mode) {
			state->change_palette = c[1];
		}
        c += 2;
        break;
        
    case 0x3b:
        dprintf((stderr, "[0x3b: ɽ�����ѥ�å��ѹ�����(¨��ȿ�ǤϤ��ʤ�)(%d)?\n",c[1]));
		if (!history_mode) {
			state->change_palette2 = c[1];
		}
        c += 2;
        break;
        
    case 0x3c:
        dprintf((stderr, "[0x3c: ��ȿž����(%x)?\n",c[1]));
		if (!history_mode) {
			state->reverse_palette = c[1];
		}
        c += 2;
        break;
        
        /* ------------------------------------------------------ */
        
    case 0x60:
        dprintf((stderr, "[0x60: �� �ѥ�å�ȿ��?\n"));
        c++;
        break;
        
    case 0x61:
        dprintf((stderr, "[0x61: �� �ѥ�åȤ�0��?\n"));
        c++;
        break;
            
    case 0x62:
        dprintf((stderr, "[0x62: ��? ���ե����Ȼ���?(%x)\n",c[1]));
        c += 2;
        break;
            
    case 0x63:
        dprintf((stderr, "[0x63: ��? ���ե����Ȼ���? (%x)\n",c[1]));
        c += 2;
        break;
        
    case 0x64:
        dprintf((stderr, "0x64:: �� ���ե����Ȥ����?\n"));
        c++;
        break;
        
    case 0x65:
        dprintf((stderr, "0x65:: �� ���ե����Ȥ����?\n"));
        c++;
        break;
        
    case 0x66:
        dprintf((stderr, "0x66:: �� ���ե����Ȥ����?\n"));
        c++;
        break;      
        
    case 0x67:
        dprintf((stderr, "[0x67: ��? ���ե����Ȼ���?(%x)\n",c[1]));
        c += 2;
        break;

    case 0x68:
        dprintf((stderr, "[0x68: ��? ���ե����Ȼ���? (%x)\n",c[1]));
        c += 2;
        break;

    case 0x69:
        dprintf((stderr, "0x69: �ե����ɥ���?\n"));
        c++;
        break;
        
    case 0x6a:
        dprintf((stderr, "0x6a: �ե����ɥ�����?\n"));
        c++;
        break;

    case 0x6b:
        dprintf((stderr, "[0x6b: ��? ���ե����ȴ�Ϣ?\n"));
		if (!history_mode) {
			text_effect(c[1]);
		}
        c += 2;
        break;
        
    case 0x6c:
        dprintf((stderr, "[0x6c: ��? ���ե����ȴ�Ϣ?\n"));
		if (!history_mode) {
			text_effect(c[1]);
		}
        c += 2;
        break;
        
        /* ------------------------------------------------- */
        
    case 0x80: /* BGM���� */
        dprintf((stderr, "BGM ����(%d)\n", c[1]));
		if (!history_mode) {
			LvnsStartMusicLoop(lvns, bgmmap(c[1]));
		}
        c += 2;
        break;
        
    case 0x81: /* BGM FADE */
        dprintf((stderr, "BGM �ե���������\n"));
		if (!history_mode) {
			LvnsFadeMusic(lvns);
		}
        c++;
        break;
        
    case 0x82: /* BGM ��� */
        dprintf((stderr, "BGM���\n"));
		if (!history_mode) {
			LvnsPauseMusic(lvns);
		}
        c++;
        break;
        
    case 0x84: /* ���������BGM���� */
        dprintf((stderr, "BGM ��Ϣ����?(%d)\n", c[1]));
		if (!history_mode) {
			/* LvnsSetNextMusicLoop(lvns, bgmmap(c[1]));*/
		}
        c += 2;
        break;
        
    case 0x85: /* BGM�ե����ɽ�����λ�Ԥ� */
        dprintf((stderr, "BGM �ե����ɽ�λ�Ԥ�\n"));
		if (!history_mode) {
			LvnsWaitMusicFade(lvns);
		}
        c++;
        break;

    case 0x87:
        dprintf((stderr, "BGM ����(fadein)(%d)\n", c[1]));
		if (!history_mode) {
			LvnsStartMusicLoop2(lvns, bgmmap(c[1]));
		}
        c += 2;
        break;
       
        /* -------------------------------------------------- */

    case 0xa0:
        dprintf((stderr, "[A0 PCM�ɤ߹���(%02d)]\n", c[1]));
		if (!history_mode) {
			LvnsLoadSound(lvns, "KZ_VD%02d.P16", c[1]);
		}
        c += 2;
        break;

    case 0xa1:
        dprintf((stderr, "[A1 PCM��λ]\n"));
		if (!history_mode) {
			LvnsStopSound(lvns);
		}
        c++;
        break;
                
    case 0xa2:
        dprintf((stderr, "[A2 PCM����(%02x,%02d)]\n", c[1], c[2]));
		if (!history_mode) {
			LvnsStartSound(lvns, c[1]);
		}
        c += 3;
        break;

    case 0xa3:
        dprintf((stderr, "[A3 PCM����Ԥ�]\n"));
		if (!history_mode) {
			LvnsWaitSound(lvns);
		}
        c++;
        break;

    case 0xa6:
        dprintf((stderr, "[A6 PCM��Ϣ?]\n"));
        c++;
        break;
        
    case 0xaf:		/* ��å�������λ */
        c++;
        return True;

    case 0xb0:		/* ���� */
		LvnsNewLineText(lvns);
        c++;
        break;

    case 0xb2:		/* �����Ԥ� */
		if (!history_mode) {
			LvnsWaitKey(lvns);
		}
        c++;
        break;

    case 0xb3:              /* �ڡ��������ޤ� */
		if (!history_mode) {
			LvnsWaitPage(lvns);
			lvns->savepoint_flag = True;
		}
        c++;
        break;
        
    case 0xb6:		/* ʸ�������Ԥ����ֻ��� */
        dprintf((stderr, "ʸ������®�ٻ���(%d*10ms)\n", c[1]));
		if (!history_mode) {
			lvns->char_wait_time = c[1];
		}
        c += 2;
        break;

    case 0xb7:              /* ���֤ޤ� */
        dprintf((stderr, "�����Ԥ�(%d*10ms)\n", c[1]));
		if (!history_mode) {
			if (!lvns->fast_text && !lvns->skip) {
				LvnsWait(lvns, c[1]);
			}
		}
        c += 2;
		break;

    case 0xb9:		/* ʸ�����襪�ե��åȻ��� */
        dprintf((stderr, "ʸ�����襪�ե��åȻ��� %d\n", c[1]));
        LvnsSetTextOffset(lvns, c[1] * 8);
        c += 2;
        break;
        
    case 0xbb:		/* �ե�å��奨�ե����� */
		dprintf((stderr, "�ե�å���(0xbb)\n"));
		if (!history_mode) {
			LvnsWhiteOut(lvns);
			LvnsWhiteIn(lvns);
		}
		c++;
        break;
        
    case 0xbc:		/* ���̿�ư���ե����� */
        dprintf((stderr, "�֤ɤ��áײ��̿�ư(0xbc:%x)\n", c[1]));
		if (!history_mode) {
			LvnsClearCursor(lvns);
			LvnsVibrato(lvns);
		}
        c++;
        break;
        
    case 0xbd:		/* �̾��طʥ��� */
		dprintf((stderr, "�ط�:0xbd (%d) %d %d\n", c[1], c[2], c[3]));
		if (!history_mode) {
			LvnsUndispText(lvns);
			LvnsClear(lvns, text_effect(c[2]));
			KizuatoLoadBG(lvns, c[1]);
			KizuatoDisp(lvns, text_effect(c[3]));
		}
	    c += 4;
        break;
        
    case 0xbe:		/* �طʥ���? */
		dprintf((stderr, "�ط�2:0xbe %d %d %d\n", c[1], c[2], c[3]));
		if (!history_mode) {
			LvnsUndispText(lvns);
			LvnsClear(lvns, text_effect(c[2]));
			KizuatoLoadBG(lvns, c[1]);
			KizuatoDisp(lvns, text_effect(c[3]));
		}
	    c += 4;
        break;
        
    case 0xbf:		/* �ӥ��奢�륷������� */
		dprintf((stderr, "Visual:0xbf %d %d\n", c[2], c[3]));
		if (!history_mode) {
			LvnsUndispText(lvns);
			LvnsClear(lvns, text_effect(c[2]));
			KizuatoLoadVisual(lvns, c[1]);
			KizuatoDisp(lvns, text_effect(c[3]));
		}
	    c += 4;
        break;
        
    case 0xc0:		/* H������ӥ��奢����� */
		dprintf((stderr, "H������:0xc0 %d %d\n", c[2], c[3]));
		if (!history_mode) {
			LvnsUndispText(lvns);
			LvnsClear(lvns, text_effect(c[3]));
			KizuatoLoadHVisual(lvns, c[1]);
			KizuatoDisp(lvns, text_effect(c[3]));
		}
	    c += 4;
        break;
        
    case 0xc1:		/* ����饯���ѹ� */
		dprintf((stderr, "������ѹ� 0xC1\n"));
		if (!history_mode) {
			LvnsUndispText(lvns);
			KizuatoClearCharacter(lvns, c[1]);
			KizuatoLoadCharacter(lvns, c[2], c[1]);
			KizuatoDisp(lvns, LVNS_EFFECT_FADE_MASK);
		}
		c += 3;
        break;
        
    case 0xc2:		/* ����饯��ɽ�� */
		dprintf((stderr, "�����ɽ�� 0xC2\n"));
		if (!history_mode) {
			LvnsUndispText(lvns);
			KizuatoLoadCharacter(lvns, c[2], c[1]);
			KizuatoDisp(lvns, LVNS_EFFECT_FADE_MASK);
		}
		c += 3;
        break;
        
    case 0xc3:	/* ������饯����ä��Ƥ��饭��饯��ɽ�� */
		dprintf((stderr, "���õ�奭���ɽ�� 0xC3\n"));
		if (!history_mode) {
			LvnsUndispText(lvns);
			KizuatoClearCharacter(lvns, 3);
			KizuatoLoadCharacter(lvns, c[2], c[1]);      
			KizuatoDisp(lvns, LVNS_EFFECT_FADE_MASK);
		}
		c += 3;
        break;
        
    case 0xc4:	/* �ط��դ�����饯��ɽ�� */
		dprintf((stderr, "�طʤĤ�����饯��ɽ�� %d %d %d %d\n",
				 c[1], c[3],c[4],c[5]));
		if (!history_mode) {
			LvnsUndispText(lvns);
			LvnsClear(lvns, text_effect(c[4]));
			KizuatoLoadBG(lvns, c[3]);
			KizuatoLoadCharacter(lvns, c[2], c[1]);
			KizuatoDisp(lvns, text_effect(c[5]));
		}
		c += 6;
        break;
        
    case 0xc6: /* 3 ��ɽ�� */
		dprintf((stderr, "3�����Ʊ��ɽ��\n"));
		if (!history_mode) {
			LvnsUndispText(lvns);
			KizuatoClearCharacter(lvns, 3);
			KizuatoLoadCharacter(lvns, c[2], c[1]);
			KizuatoLoadCharacter(lvns, c[4], c[3]);
			KizuatoLoadCharacter(lvns, c[6], c[5]);
			KizuatoDisp(lvns, LVNS_EFFECT_FADE_MASK);
		}
		c += 7;
        break;

    case 0xc8:
        dprintf((stderr, "[0xc8: �� ���֤����]\n"));
        c += 3;
        break;

    case 0xc9:
        dprintf((stderr, "[0xc9: �� ���֤����]\n"));
        c++;
        break;

    case 0xca:
        dprintf((stderr, "[0xca: ������\n"));
		if (!history_mode) {
			LvnsUndispText(lvns);
			LvnsAnimation(lvns, kizuato02);
		}
        c++;
        break;

    case 0xcb:
        dprintf((stderr, "[0xcb: �� ���֤����]\n"));
        c++;
        break;

    case 0xcc: 
        dprintf((stderr, "[0xcc: ���ͤ��� ON %d %d]\n", c[1], c[2]));
		if (!history_mode) {
			if (c[1] == 1) {
				LvnsSetBackEffect(lvns, &lvnsSinEffect2);
			} else if (c[2] == 1) {
				LvnsSetBackEffect(lvns, NULL);
			}
		}
        c += 3;
        break;
        
    case 0xcd:
        dprintf((stderr, "[0xcd: �դ��Ǥ��]\n"));
		if (!history_mode) {
			LvnsAnimation(lvns, kizuato03);
		}
        c++;
        break;

    default:
        fprintf(stderr, "��������ʤ��ä����ޥ�ɤǤ�(%02x)!!\n", c[0]);
		return True;
    }

    return False;
}
#undef c

static void
KizuatoDispText(Lvns *lvns, const u_char *c, Bool history_mode)
{
	while (1) {
		int code = c[0];
		if (code >= 0 && code <= 0x20) {
			int FontCode = (code << 8) + c[1];
			LvnsDispText(lvns);
			c += 2;
			LvnsPutChar(lvns, FontCode, lvns->text_attr);
		} else  {
			if (CommandParser(lvns, &c, history_mode))
				return;
		}
    } /* While(1) */
}

/*
 * �ƥ����ȥ��ʥꥪ�ѡ���
 * ɽ������λ������ True ���֤�
 */
static void
TextParser(Lvns *lvns, int no, Bool add_history)
{
	KizuatoSetTextScenarioState(lvns, no);
	KizuatoDispText(lvns, LvnsGetScenarioText(lvns, no),  False);
	if (add_history)
		LvnsAddHistory(lvns, no);
}


/* ------------------------------------------------------------ */
/** ���ʥꥪ���󥸥󳫻��� */

static void
KizuatoStartScenario(Lvns *lvns)
{
    KizuatoState *state = (KizuatoState*)lvns->system_state;

    /* ���ʥꥪ�������� */
    LvnsLoadScenario(lvns, lvns->savepoint.scn, lvns->savepoint.blk);
    lvns->scn_cur = lvns->scn_cur_head + lvns->savepoint.scn_offset;

	LvnsClearText(lvns);

    /* �������� */
    if (lvns->savepoint.bg_no) {
		int i;
        switch (lvns->savepoint.bg_type) {
        case LVNS_VISUAL:
	    KizuatoLoadVisual(lvns, lvns->savepoint.bg_no);
            break;
        case LVNS_HCG:
            KizuatoLoadHVisual(lvns, lvns->savepoint.bg_no);
            break;
        default:
            KizuatoLoadBG(lvns, lvns->savepoint.bg_no);
            break;
        }
        KizuatoClearCharacter(lvns, 3);
		for (i=0;i<3;i++) {
			if (lvns->savepoint.character_no[i] != NOCHARACTER)
				KizuatoLoadCharacter(lvns, lvns->savepoint.character_no[i], i);
		}
		LvnsDisp(lvns, LVNS_EFFECT_WIPE_TTOB);
    } else {
        KizuatoLoadBG(lvns, 0);       
        KizuatoClearCharacter(lvns, 3);
    }

    /* BGM ���� */
    if (lvns->savepoint.current_music)
        LvnsSetNextMusicLoop(lvns, lvns->savepoint.current_music);

    /* �ե饰���� */
    memcpy(state->flag, state->flag_save, sizeof state->flag);

    /* �����������׽���� */
    lvns->selectpoint = lvns->savepoint;
    memcpy(state->flag_select, state->flag_save, sizeof state->flag);

}

/* 
 * �����ʥꥪ�ѡ��� 
 */
#define c lvns->scn_cur
void 
KizuatoMain(Lvns *lvns)
{
    KizuatoState *state = (KizuatoState *)lvns->system_state;
	KizuatoStartScenario(lvns);

	while(1) {

		lvns->inside_state = LVNS_MAIN;

		/* �����֥ݥ�������� */
		if (lvns->savepoint_flag) {
			LvnsSetSavePoint(lvns, &lvns->savepoint);
			memcpy(state->flag_save, state->flag, sizeof state->flag);
			lvns->savepoint_flag = False;
		}

		/* ��˥塼��ư */


        switch (c[0]) {
        case 0x20: /* ��λ */
            dprintf((stderr, "[END]\n"));
            c ++;
			return;

        case 0x24: /* ������̿�� */
            dprintf((stderr, "[������ SCN%03d.DAT - Block %d]\n", c[1], c[2]));
            LvnsLoadScenario(lvns, c[1], c[2]);
			break;

        case 0x25: /* ����� */
			dprintf((stderr, "[�����(%d)]-[��å�����:%d]\n", c[2], c[1]));
            {
                int i;
				TextParser(lvns, c[1], True);
                for (i = 0; i < c[2]; i++) {
                    dprintf((stderr,
                             "[����� %d]-[��å�����:%d]-[���ե��å�:%02x]\n",
                             i, c[3 + i*2], c[4 + i*2]));
					lvns->text_attr = i+1;
					TextParser(lvns, c[3+i*2], False);
                }
				lvns->text_attr = 0;
				i = LvnsWaitSelect(lvns, c[2]);
				LvnsAddHistory(lvns, c[3+i*2]);
				
				dprintf((stderr, "����ʬ��: %d (+%02x)\n", i, c[4 + i*2]));

				c = c + 3 + c[2] * 2 + c[4 + i*2];
            }
            break;

        case 0x27:
            dprintf((stderr, "[�������������ޡ�������]\n"));
			/* ���֤���¸�� */
            LvnsSetSavePoint(lvns, &lvns->selectpoint);
            memcpy(state->flag_select, state->flag, sizeof state->flag);
            c ++;
            break;

        case 0x41: /* if ʸ */
            dprintf((stderr, "[if flg:%02x == 0x%02x pc += %02x]\n", 
                     c[1], c[2], c[3]));
            if (state->flag[flagmap(c[1])] == c[2])
                lvns->scn_cur += c[3];
            c += 4;
            break;

        case 0x42: /* if ʸ (����) */
            dprintf((stderr, "[if flg:%02x != 0x%02x pc += %02x]\n", 
                     c[1], c[2], c[3]));
            if (state->flag[flagmap(c[1])] != c[2])
                lvns->scn_cur += c[3];
            c += 4;
            break;

        case 0x4b: /* �ե饰���å� */
            dprintf((stderr, "[flg:%02x = 0x%02x]\n", c[1], c[2]));
            state->flag[flagmap(c[1])] = c[2];
            c += 3;
            break;

        case 0x51: /* ��å��������� */
            dprintf((stderr, "[��å�����:%d]\n", c[1])); 
			TextParser(lvns, c[1], True);
            c += 2;
            break;

        case 0x52:		/* ������¸�ߤ��롣̵���Ƥ�����פ�? */
            dprintf((stderr, "[�� 0x%02x]\n", c[0]));
            c ++;
            break;

        case 0x94:		/* ������¿ʬ����0 */
            dprintf((stderr, "[����ǥ��󥰴�Ϣ 0x%02x]\n", c[0]));
            c ++;
            break;

        case 0x95: /* ����ǥ���BGM ���� */
            dprintf((stderr, "[����ǥ��� BGM ���� 0x%02x] %d\n", c[0], c[1]));
            /* ����ǥ��󥰥ơ��ޱ��ճ��� */
            LvnsStartMusic(lvns, bgmmap(c[1]));

            /* ����ǥ��󥰵�ư */
            KizuatoEnding(lvns);

            /* �ǽ�Ū�ʥե饰��ȿ�ǡ� */
            memcpy(state->flag_save, state->flag, sizeof state->flag);

            /* ����ǥ�����ã�ˤĤ����ʥꥪ�ǡ�������� */
            KizuatoScenarioInit(lvns);
            KizuatoSave(lvns);

            c += 2;
            break;

        case 0x96: /* ����ǥ����ֹ���� */
            dprintf((stderr, "[����ǥ����ֹ����0x%02x] %d\n", c[0], c[1]));
/*
   0  25 ���� True 
   8  24 �� Happy
   10 23 ���� END
   15 23 ������ԥ󥨥��
   16 15 ����
*/
            state->ending[c[1]] = 1;
            {
                int i;
                for (i=0; i<sizeof state->ending;i++) {
                    if (state->ending[i])
                        break;
                }
                if (i == sizeof state->ending) {
                    state->flag[flagmap(0x73)] = 1;
                }
            }
            c += 2;
            break;

        default:
            if (CommandParser(lvns, &lvns->scn_cur, False))
				return;
        }
    } /* while(1) */
}
#undef c

/**
 * ����ľ���ν����
 */
void
KizuatoStart(Lvns *lvns)
{
    lvns->setPaletteIndex(lvns, KIZUATO_COL_WHITE, 255, 255, 255);
    lvns->setPaletteIndex(lvns, KIZUATO_COL_GRAY,  127, 127, 127);
    lvns->vram->black       = KIZUATO_COL_BLACK;
    lvns->background->black = KIZUATO_COL_BLACK;
    LvnsInitAnimation(lvns, kizuato01);
    LvnsInitAnimation(lvns, kizuato02);
    LvnsInitAnimation(lvns, kizuato03);
}

void
KizuatoDispHistory(Lvns *lvns, int pos)
{
	LvnsClearText(lvns);         /* �õ� */
	LvnsDispWindow(lvns);
	if (pos >= 0 && pos < lvns->history_pos) {
		LvnsLoadScenario(lvns, lvns->history[pos].scn, lvns->history[pos].blk);
		KizuatoDispText(lvns, LvnsGetScenarioText(lvns, lvns->history[pos].no), True);
	}

	// ��������ɽ��
#ifndef USE_MGL
#define CUR_X 25
#else
#define CUR_X 24
#endif
	LvnsLocate(lvns, CUR_X, 0);
	LvnsPuts(lvns, "��", 1);
	LvnsLocate(lvns, CUR_X, 11);
	LvnsPuts(lvns, "��", 2);
}

