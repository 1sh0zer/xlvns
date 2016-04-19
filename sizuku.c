/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: sizuku.c,v 1.57 2001/08/05 11:46:02 go Exp $
 *
 */

/* 
 * �� ���ʥꥪ�������󥸥�
 */
#include <stdio.h>
#include <ctype.h>
#include "sizuku.h"

/* ���ͤ��� sin_effect.h */
extern LvnsBackEffectInfo lvnsSinEffect;

/* �ü쥨�ե����ȡ��ޤμ����ѥǡ��� ����1 */
static LvnsAnimationData sizuku01[] = {
    { LVNS_ANIM_IMAGE, "OP_S00.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S01.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S02.LFG", 10, 0, 160 },
    { LVNS_ANIM_SOUND },
    { LVNS_ANIM_IMAGE, "OP_S03.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S04.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S05.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S06.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S07.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S08.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S09.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S10.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S11.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S12.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S13.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S14.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S15.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S16.LFG", 10, 0, 160 },
    { LVNS_ANIM_WAIT, NULL, 200 },
    { LVNS_ANIM_IMAGE, "OP_S00.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S01.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S02.LFG", 10, 0, 160 },
    { LVNS_ANIM_SOUND },
    { LVNS_ANIM_IMAGE, "OP_S03.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S04.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S05.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S06.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S07.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S08.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S09.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S10.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S11.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S12.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S13.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S14.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S15.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S16.LFG", 10, 0, 160 },
    { LVNS_ANIM_WAIT, NULL, 200 },
    { LVNS_ANIM_NONE, NULL, 0 }
};

/* �ü쥨�ե����ȡ��ޤμ����ѥǡ��� ����2 */
static LvnsAnimationData sizuku02[] = {
    { LVNS_ANIM_IMAGE, "OP_S00.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S01.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S02.LFG", 10, 0, 160 },
    { LVNS_ANIM_SOUND },
    { LVNS_ANIM_IMAGE, "OP_S03.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S04.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S05.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S06.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S07.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S08.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S09.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S10.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S11.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S12.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S13.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S14.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S15.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "OP_S16.LFG", 10, 0, 160 },
    { LVNS_ANIM_IMAGE, "MAX_S37.LFG", 10, 0, 0 },
    { LVNS_ANIM_NONE, NULL, 0 }
};

/* �� ���ե������б�ɽ */
static LvnsEffectType sizuku_effect[] = {
    LVNS_EFFECT_FADE_PALETTE,
    LVNS_EFFECT_GURUGURU,
    LVNS_EFFECT_SLANTTILE,
    LVNS_EFFECT_FADE_SQUARE,
    LVNS_EFFECT_WIPE_SQUARE_LTOR,
    LVNS_EFFECT_FADE_MASK,
    LVNS_EFFECT_WIPE_TTOB,
    LVNS_EFFECT_WIPE_LTOR, 
    LVNS_EFFECT_WIPE_MASK_LTOR,
    LVNS_EFFECT_VERTCOMPOSITION,
    
    LVNS_EFFECT_SLIDE_LTOR,
    LVNS_EFFECT_NORMAL,
    LVNS_EFFECT_FADE_MASK
};

/*
 * �ƥ������ѥ��ե����ȥѥ�᡼���Ѵ�
 */
static LvnsEffectType
text_effect(int c1, int c2)
{
    int no = ((c1 - '0') * 10) + (c2 - '0');
    LvnsEffectType ret;

    if (no == 99) {
        ret =LVNS_EFFECT_NORMAL;
    } else  {
        ret = sizuku_effect[no];
    }

    // dprintf((stderr,  "(%02d)%s\n", no, LvnsEffectName(ret)));
    return ret;
}

/*
 * bgm �ѥ�᡼���Ѵ�
 */
static int
bgmmap(int no)
{
    if (no == 14) 
        return 2;
    else if (no < 16)
        return no+2;
    else 
        return no+1;
}

/* 
 * �ե饰�ѥ�᡼���Ѵ�
 */
static int
flagmap(int no)
{
    switch (no) {
    case 0:
        return 0; /* ����ǥ��󥰾��ֵ�Ͽ(�������о�) */
    case 1:
        return 1; /* ���ե饰 */
    case 0x40: /* �����Ҥ���« */
    case 0x41: /* �������ͱ� */
    case 0x42: /* ����ư���Ѥ�ä����� */
    case 0x43: /* �ΰ�ۤǡ� */
    case 0x44: /* �����Ҳ�����Ϥ��ʤ��ä��� */
    case 0x45: /* ������ TRUE  (�������о�) */
    case 0x46: /* ������ HAPPY (�������о�) */
    case 0x47: /* �����롼��ON */
    case 0x48: /* ���� H������ */  
    case 0x49: /* �����餯̤���� */
    case 0x4a: /* �����ư��������� */
    case 0x4b: /* ���르���뤼��ޤ� */
        return 2 + no - 0x40;
    default:
        dprintf((stderr, "bad flag no:%d\n", no));
        exit(1);
    }
}

/**
 * ���ʥꥪ����� (���ꥢ���֥ե饰��̤������ʳ��ν����)
 */
void
SizukuScenarioInit(Lvns *lvns)
{
    SizukuState *state = (SizukuState*)lvns->system_state;

    /* �����־��־��ֽ���� */
    LvnsInitSavePoint(lvns, &lvns->savepoint);

    /* ���ʥꥪ�ե饰����� */
    state->flag_save[2] = 0;
    state->flag_save[3] = 0;
    state->flag_save[4] = 0;
    state->flag_save[5] = 0;
    state->flag_save[6] = 0;
	
    state->flag_save[9]  = 0;
    state->flag_save[10] = 0;
    state->flag_save[11] = 0;
    state->flag_save[12] = 0;
    state->flag_save[13] = 0;
}

/**
 * ���������� (���ե饰�õ�)
 */
void
SizukuSioriInit(Lvns *lvns)
{
    SizukuState *state = (SizukuState *)lvns->system_state;
    int i;

    dprintf((stderr, "Siori Init\n"));

    /* ���ʥꥪ�ǡ�������� */
    SizukuScenarioInit(lvns);

    /* ̤���ե饰���õ� */
    for (i=0; i<SIZUKU_SCN_NO;i++) {
		state->seen_flag[i] = 0;
    }

    /* �Ĥ������ե饰�ξõ� */
    state->flag_save[0] = 0;
    state->flag_save[7] = 0;
    state->flag_save[8] = 0;
}

#define HexToDig(c) ((tolower(c) >= 'a')? (tolower(c) -'a' + 10) : (c - '0'))
#define HexToDig2(c,c2) (HexToDig(c)*16 + HexToDig(c2))
#define Dig(c1,c2) (c1 - '0') * 10 + (c2 - '0')

static void
SizukuDispText(Lvns *lvns, const u_char *c, Bool history_mode)
{
    while(1) {
		if (c[0] & 0x80) {
			int FontCode = ((c[0] & 0x7f) << 8) + c[1];
			LvnsDispText(lvns);
			if (lvns->tvram[lvns->current_tvram].cur_x > 24) {
				LvnsNewLineText(lvns);
			}
			LvnsPutChar(lvns, FontCode, lvns->text_attr);
			c += 2;
		} else switch (c[0]) {

		case '$': /* '$' ��å�������λ */
			c++;
			return;
			
		case 'r': /* ���� */
			LvnsNewLineText(lvns);
			c++;
			break;
			
		case 'p': /* �ڡ��������Ԥ� */
			if (!history_mode) {
				LvnsWaitPage(lvns);
				lvns->savepoint_flag = True;
			}
			c++;
			break;
			
		case 'k':
		case 'K': /* ���������Ԥ� */
			if (!history_mode) {
				if (!lvns->fast_text)
					LvnsWaitKey(lvns);
			}
			c++;
			break;
			
		case '0': /* ������ץȥХ��� '0' ��;ʬ�ˤĤ��Ƥ���Ȥ������� */
			dprintf((stderr, "[��0��̵��!]\n"));
			c++;
			break;
			
		case 'C': /* ����饯���� */
			dprintf((stderr, "[����饯����(%c, MAX_C%c%c)]", c[1], c[2], c[3]));
			if (!history_mode) {
				LvnsUndispText(lvns);
				SizukuClearCharacter(lvns, c[1]);
				SizukuLoadCharacter(lvns, HexToDig2(c[2], c[3]), c[1]);
				LvnsDisp(lvns, LVNS_EFFECT_FADE_MASK);
			}
			c += 4;
			break;
			
		case 'B': /* �ط�ɽ�� */
			dprintf((stderr, "[�طʥ���(%c%c, %d, %d)]\n", c[1], c[2],
					 Dig(c[3],c[4]), Dig(c[5],c[6])));
			if (!history_mode) {
				LvnsUndispText(lvns);
				LvnsClear(lvns, text_effect(c[3],c[4]));
				SizukuLoadBG(lvns, Dig(c[1],c[2]));
				LvnsDisp(lvns, text_effect(c[5],c[6]));
			}
			c += 7;
			break;
			
		case 'S': /* �طʤĤ�����饯��ɽ�� */
			dprintf((stderr, "[�ط��դ������ɽ��(%c, MAX_C%c%c, MAX_S%c%c, %d, %d)]",
					 c[1], c[2],c[3],c[4],c[5], Dig(c[6],c[7]), Dig(c[8], c[9])));
			if (!history_mode) {
				LvnsUndispText(lvns);
				LvnsClear(lvns, text_effect(c[6],c[7]));
				SizukuLoadBG(lvns, Dig(c[4], c[5]));
				SizukuLoadCharacter(lvns, HexToDig2(c[2], c[3]), c[1]);
				LvnsDisp(lvns,text_effect(c[8],c[9]));
			}
			c += 10;
			break;
			
		case 'D': /* ����饯�����õ��ɽ�� */
			dprintf((stderr, "[��������õ��ɽ��(%c, MAX_C%c%c)]", c[1], c[2], c[3]));
			if (!history_mode) {
				LvnsUndispText(lvns);
				SizukuClearCharacter(lvns, 'a');
				SizukuLoadCharacter(lvns, HexToDig2(c[2], c[3]), c[1]);
				LvnsDisp(lvns, LVNS_EFFECT_FADE_MASK);
			}
			c += 4;
			break;
			
		case 'A':
		case 'a':
			dprintf((stderr, "[�����3��ɽ��(%c, %c%c, %c, %c%c, %c, %c%c)]",
					 c[1],c[2],c[3],  c[4],c[5],c[6], c[7],c[8],c[9]));
			if (!history_mode) {
				LvnsUndispText(lvns);
 				SizukuClearCharacter(lvns, 'a');
				SizukuLoadCharacter(lvns, HexToDig2(c[2], c[3]), c[1]);
				SizukuLoadCharacter(lvns, HexToDig2(c[5], c[6]), c[4]);
				SizukuLoadCharacter(lvns, HexToDig2(c[8], c[9]), c[7]);
				LvnsDisp(lvns, LVNS_EFFECT_FADE_MASK);
			}
			c += 10;
			break;
			
		case 'Q': /* ���̤��ɤ餹 */
			dprintf((stderr, "[���̤��ɤ餹(%02x)]", c[0]));
			if (!history_mode) {
				LvnsClearCursor(lvns);
				LvnsVibrato(lvns);
			}
			c++;
			break;
			
		case 'E': /* �ط�ɽ��2 ? */
			dprintf((stderr, "[�طʥ���(2)?(MAX_S%c%c.LFG, %d, %d)]",
					 c[1], c[2], Dig(c[3], c[4]), Dig(c[5], c[6])));
			if (!history_mode) {
				LvnsUndispText(lvns);
				LvnsClear(lvns, text_effect(c[3],c[4]));
				SizukuLoadBG(lvns, Dig(c[1],c[2]));
				LvnsDisp(lvns, text_effect(c[5],c[6]));
			}
			c += 7;
			break;
			
		case 'F': /* �ե�å��� */
			dprintf((stderr, "[�ե�å���(%c)]", c[0]));
			if (!history_mode) {
				LvnsWhiteOut(lvns);
				LvnsWhiteIn(lvns);
			}
			c++;
			break;
			
		case 'V': /* �ӥ��奢�륷����ɽ�� */
			dprintf((stderr, "[�ӥ��奢��(VIS%c%c, %d, %d)]",
					 c[1], c[2], Dig(c[3], c[4]), Dig(c[5], c[6])));
			if (!history_mode) {
				LvnsUndispText(lvns);
				LvnsClear(lvns, text_effect(c[3],c[4]));
				SizukuLoadVisual(lvns, Dig(c[1],c[2]));
				LvnsDisp(lvns, text_effect(c[5],c[6]));
			}
			c += 7;
			break;
			
		case 'H': /* H������ɽ�� */
			dprintf((stderr, "[H������(HVS%c%c, %d, %d)]",
					 c[1], c[2], Dig(c[3], c[4]), Dig(c[5], c[6])));
			if (!history_mode) {
				LvnsUndispText(lvns);
				LvnsClear(lvns,text_effect(c[3],c[4]));
				SizukuLoadHVisual(lvns, Dig(c[1],c[2]));
				LvnsDisp(lvns, text_effect(c[5],c[6]));
			}
			c += 7;
			break;
			
		case 'M': /* BGM ��Ϣ */
			{
				int c1 = c[1];
				c += 2;
				if (c1 == 'f') {
					dprintf((stderr, "[BGM �ե����ɥ�����]\n"));
					if (!history_mode) {
						LvnsFadeMusic(lvns);
					}
				} else if (c1 == 'n') {
					int c2 = *c++;
					int c3 = *c++;
					dprintf((stderr, "[BGM ����(next)(M_%c%c)\n", c2, c3));
					if (!history_mode) {
						LvnsSetNextMusicLoop(lvns, bgmmap(Dig(c2, c3)));
					}
				} else if (c1 == 'w') {
					dprintf((stderr, "[BGM FADE WAIT]\n"));
					if (!history_mode) {
						LvnsWaitMusicFade(lvns);
					}
				} else if (c1 >= '0' && c1 <= '2') {
					int c2 = *c++;
					dprintf((stderr, "[BGM ����(M_%c%c)]\n", c1, c2));
					if (!history_mode) {
						LvnsStartMusicLoop(lvns, bgmmap(Dig(c1, c2)));
					}
				} else if (c1 == 's') {
					dprintf((stderr, "[BGM ���]\n"));
					if (!history_mode) {
						LvnsPauseMusic(lvns);
					}
				} else {
					dprintf((stderr, "[cmd:4d][cmd:%x]\n", c1));
				}
			}
			break;
			
		case 'P': /* PCM ��Ϣ */
			{
				int c1 = c[1];
				c += 2;
				if ( c1 == 'l' ) {
					int c2 = *c++;
					int c3 = *c++;
					dprintf((stderr, "[PCM����(%c%c)]\n", c2, c3));
					if (!history_mode) {
						LvnsLoadSound(lvns, "SZ_VD%02d.P16", Dig(c2,c3));
					}
				} else if (c1 >= '0' && c1 <= '9') {
					int c2 = *c++;
					int c3 = *c++;
					int c4 = *c++;
					dprintf((stderr, "[PCM��������(%c%c, %c%c)\n]", 
							 c1, c2, c3, c4));
					if (!history_mode) {
						LvnsStartSound(lvns, 1);
					}
				} else if (c1 == 'f') {
					dprintf((stderr, "[PCM�ե����ɥ�����?]\n"));
				} else if (c1 == 'w') {
					dprintf((stderr, "[PCM wait?]\n"));
					if (!history_mode) {
						LvnsWaitSound(lvns);
					}
				} else if (c1 == 's') {
					dprintf((stderr, "[PCM���]\n"));	
					if (!history_mode) {
						LvnsStopSound(lvns);
					}
				} else {
					dprintf((stderr, "[cmd:50][cmd:%x]\n", c1));
				}
				break;
			}
			
		case 'X': /* 'X' ɽ�����ե��åȻ��� */
			dprintf((stderr, "[ɽ�����ե��åȻ���(%x)]\n", c[1]));
			if (!history_mode) {
				LvnsSetTextOffset(lvns, c[1]);
			}
			c += 2;
			break;
			
		case 's': /* 's' ɽ��®�ٻ��� */
			dprintf((stderr, "[ɽ��®�ٻ���?(%x)]\n", c[1]));
			if (!history_mode) {
				lvns->char_wait_time = c[1];
			}
			c += 2;
			break;
			
		default:
			dprintf((stderr, "unknown!!"));
			dprintf((stderr, "[cmd:%x]", c[0]));
			return ;
		}
    } /* while(1) */
}

/*
 * �ƥ����ȥ��ʥꥪ����
 */
static void
TextParser(Lvns *lvns, int no, Bool add_history)
{
	SizukuSetTextScenarioState(lvns, no);
	SizukuDispText(lvns, LvnsGetScenarioText(lvns, no),  False);
	if (add_history)
		LvnsAddHistory(lvns, no);
}

/* ------------------------------------------------------------ */
/** ���ʥꥪ���󥸥󳫻��� */
static void
SizukuStartScenario(Lvns *lvns)
{
    SizukuState *state = (SizukuState*)lvns->system_state;

    /* ���ʥꥪ�������� */
    LvnsLoadScenario(lvns, lvns->savepoint.scn, lvns->savepoint.blk);
    lvns->scn_cur = lvns->scn_cur_head + lvns->savepoint.scn_offset;

	LvnsClearText(lvns);

    /* ���̾��֤����� */
    if (lvns->savepoint.bg_no) {
        switch (lvns->savepoint.bg_type) {
        case LVNS_VISUAL:
			SizukuLoadVisual(lvns, lvns->savepoint.bg_no);
            break;
        case LVNS_HCG:
            SizukuLoadHVisual(lvns, lvns->savepoint.bg_no);
            break;
        default:
            SizukuLoadBG(lvns, lvns->savepoint.bg_no);
            break;
        }
        SizukuClearCharacter(lvns, 'a');
        if (lvns->savepoint.character_no[0] != NOCHARACTER)
            SizukuLoadCharacter(lvns, lvns->savepoint.character_no[0], 'l');
        if (lvns->savepoint.character_no[1] != NOCHARACTER)
            SizukuLoadCharacter(lvns, lvns->savepoint.character_no[1], 'r');
        if (lvns->savepoint.character_no[2] != NOCHARACTER)
            SizukuLoadCharacter(lvns, lvns->savepoint.character_no[2], 'c');
		LvnsDisp(lvns, LVNS_EFFECT_WIPE_TTOB);
    } else {
        SizukuLoadBG(lvns, 0);       
        SizukuClearCharacter(lvns, 'a');
    }


    /* BGM���� */
    if (lvns->savepoint.current_music)
        LvnsSetNextMusicLoop(lvns, lvns->savepoint.current_music);

    /* �ե饰���� */
    memcpy(state->flag, state->flag_save, sizeof state->flag);

    /* �����������׽���� */
    lvns->selectpoint = lvns->savepoint;
    memcpy(state->flag_select, state->flag_save, sizeof state->flag);
}


/*
 * �����ʥꥪ�ѡ�������
 */
#define c lvns->scn_cur
void
SizukuMain(Lvns *lvns)
{
    SizukuState *state = (SizukuState *)lvns->system_state;
	SizukuStartScenario(lvns);

	/* �ᥤ��롼�� */
    while(1) {

		lvns->inside_state = LVNS_MAIN;

		/* �����֥ݥ�������� */
		if (lvns->savepoint_flag) {
			LvnsSetSavePoint(lvns, &lvns->savepoint);
			memcpy(state->flag_save, state->flag, sizeof state->flag);
			lvns->savepoint_flag = False;
		}

        switch (c[0]) {
        case 0x00:		/* �֥�å���λ */
            dprintf((stderr, "[END]\n"));
            c++;
			return;

        case 0x01: /* �ü���� */
            switch (c[1]) {
            case 0x01:
                dprintf((stderr, "[���ˤ�ꢪ��]-[��å�����:%d]\n", c[2]));
				LvnsSetBackEffect(lvns, &lvnsSinEffect);
				TextParser(lvns, c[2], True);
				LvnsSetBackEffect(lvns, NULL);   
				LvnsClear(lvns, LVNS_EFFECT_FADE_PALETTE);
				c += 3;
                break;
            case 0x02:
                dprintf((stderr, "[�Ţ����ˤ��]-[��å�����:%d]\n", c[2]));
				LvnsDisp(lvns, LVNS_EFFECT_FADE_PALETTE);
				LvnsSetBackEffect(lvns, &lvnsSinEffect);
				TextParser(lvns, c[2], True);
				LvnsSetBackEffect(lvns, NULL);   
				c += 3;
                break;
            case 0x03:
                dprintf((stderr, "[�ޤμ�:%02x]\n", c[2]));
                LvnsAnimation(lvns, (c[2] == 0)?sizuku01:sizuku02);
                c += 3;
                break;
            case 0x04:
                dprintf((stderr, "[���ˤ��2(�ۼ���)]-[��å�����:%d]\n", c[2]));
				TextParser(lvns, c[2], True);
                c += 3;
                break;
            default:
                dprintf((stderr, "�۾��0x01���ޥ�ɤǤ�(%02x,%02x)\n", c[1], c[2]));
				return;
                break;
            }
            break;

        case 0x03: /* �� */
            dprintf((stderr, "[��:%02x(%02x)]\n", c[0], c[1]));
            c += 2;
            break;

        case 0x04: /* ������̿�� */
            dprintf((stderr, "[������ SCN%03d.DAT - Block %d]\n", c[1], c[2]));
            LvnsLoadScenario(lvns, c[1], c[2]);
			break;
			
        case 0x05: /* ����� */
			dprintf((stderr, "[�����(%d)]-[��å�����:%d]\n", c[2], c[1]));
            {
                int i;
				TextParser(lvns, c[1], True);
                for (i = 0; i < c[2]; i++) {
                    dprintf((stderr,
							 "\t[����� %d]-[��å�����:%d]-[���ե��å�:%02x]\n"
                             , i, c[3+i*2], c[4+i*2]));
					lvns->text_attr = i+1;
					TextParser(lvns, c[3+i*2], False);
                }
				lvns->text_attr = 0;
				i = LvnsWaitSelect(lvns, c[2]);
				LvnsAddHistory(lvns, c[3+i*2]);

				dprintf((stderr, "����ʬ��: %d (+%02x)\n", i, c[4 + i*2]));
				c = c + 3 + c[2]*2 + c[4 + i*2];
            }
            break;

        case 0x06:
            dprintf((stderr, "[��:%02x()]\n", c[0]));
            c++;
            break;
			
        case 0x07:
            dprintf((stderr, "[�������������ޡ�������]\n"));
            LvnsSetSavePoint(lvns, &lvns->selectpoint);
            memcpy(state->flag_select, state->flag, sizeof state->flag);
            c++;
            break;		      
			
        case 0x0a: /* �طʥ��� */
            dprintf((stderr, "[�طʥ��ɤΤ�: MAX_S%02d.LFG]\n", c[1]));
			SizukuLoadBG(lvns, c[1]);
            c += 2;
			break;
			
        case 0x14: /* ���̾õ� */
            dprintf((stderr, "[���̥��ꥢ? (%02d)]\n", c[1]));
			LvnsUndispText(lvns);
            LvnsClear(lvns, sizuku_effect[c[1]]);
            c += 2;
			break;

        case 0x16: /* H���� */
            dprintf((stderr, "[H��������� : MAX_H%02d.LFG]\n", c[1]));
			LvnsUndispText(lvns);
			SizukuLoadHVisual(lvns, c[1]);
			LvnsDisp(lvns, LVNS_EFFECT_NORMAL);
            c += 2;
			break;

        case 0x22: /* ����饯������ */
			dprintf((stderr, "[����饯������ : MAX_C%02x.LFG : %02x]\n", c[1], c[2]));
			LvnsUndispText(lvns);
			SizukuLoadCharacter(lvns, c[1], c[2]);
			LvnsDisp(lvns, LVNS_EFFECT_FADE_MASK);
			c += 3;
			break;
			
        case 0x24: /* ����饯�����ɤ���2? */
			dprintf((stderr, "[����饯������(2? center?) : MAX_C%02x.LFG]\n", c[1]));
			LvnsUndispText(lvns);
			SizukuLoadCharacter(lvns, c[1], 'c');
			LvnsDisp(lvns, LVNS_EFFECT_FADE_MASK);
			c += 3;
			break;
			
        case 0x28: /* ����������¸�ߤ���ǡ��� */
            dprintf((stderr,  "[��������]\n"));
            c++;
            break;

        case 0x38:      
            dprintf((stderr, "[ɽ������:%02x(%02x)]\n", c[0], c[1]));
            LvnsDisp(lvns, sizuku_effect[c[1]]);
            c += 2;
            break;
			
        case 0x3d:	/* ifʸ */
            dprintf((stderr, "[if flg:%02x == 0x%02x pc += %02x]\n", c[1], c[2], c[3]));
            if (state->flag[flagmap(c[1])] == c[2])
                c += c[3];
            c += 4;
            break;

        case 0x3e:	/* ifʸ(����) */
            dprintf((stderr, "[if flg:%02x != 0x%02x pc += %02x]\n", c[1], c[2], c[3]));
            if (state->flag[flagmap(c[1])] != c[2])
                c += c[3];
            c += 4;
            break;

        case 0x47:     /* �ե饰�������� */
            dprintf((stderr, "[flg:%02x = 0x%02x]\n", c[1], c[2]));
            state->flag[flagmap(c[1])] = c[2];
            c += 3;
            break;
			
        case 0x48:	/* �ե饰�û� */
            dprintf((stderr, "[flg:%02x += 0x%02x\n", c[1], c[2]));
            state->flag[flagmap(c[1])] += c[2];
            c += 3;
            break;
			
        case 0x54:      /* �ƥ����ȥ�å����� */
            dprintf((stderr, "[��å�����:%d]\n", c[1]));
			TextParser(lvns, c[1], True);
            c += 2;
            break;

        case 0x5a:
            dprintf((stderr, "[��:%02x()]\n", c[0]));
            c ++;
            break;

        case 0x5c:
            dprintf((stderr, "[��:%02x(%02x)]\n", c[0], c[1]));
            c += 2;
            break;

        case 0x61:
            dprintf((stderr, "[��:%02x(%02x %02x)]\n", c[0], c[1], c[2]));
            c += 3;
            break;

        case 0x62:
            dprintf((stderr, "[��:%02x(%02x)]\n", c[0], c[1]));
            c += 2;
            break;

        case 0x60:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x66:
            dprintf((stderr, "[��:%02x()]\n", c[0]));
            c++;
            break;

        case 0x6e: /* BGM ���� */
            dprintf((stderr, "[BGM����(CMD):(%02x)]\n", c[1]));
            LvnsStartMusicLoop(lvns, bgmmap(c[1]));
            c += 2;
            break;

        case 0x6f:
        case 0x73:
            dprintf((stderr, "[��:%02x()]\n", c[0]));
            c++;
            break;

        case 0x7e:
            dprintf((stderr, "[����ǥ����ֹ�����: %02x]\n", c[1]));
/*
 0 12 ´�ȼ�
 1 12 ���� BAD
 2 12 �˲�
 3 12 �ȡ�������
 4 11 ���� HAPPY
 5 12 ���� BAD
 6 11 ���� HAPPY
 7 12 ���� BAD
 8 10 True
 9 11 ������ Happy
 a 01 ���Ĥ���
 b 14 �ۼ���
 c 12 �ۼ��� BAD
*/
            if (state->flag[flagmap(0x46)] == 1) { 
                /* ������ True END �򸫤Ƥ��� */
                state->flag[0] = 3;
            } else {
                if (state->flag[0] == 0)
                    state->flag[0] = 2;
                else
                    state->flag[0] = 1;
            }       
            c += 2;
            break;

        case 0x7c:
            dprintf((stderr, "[����ǥ��󥰴ط� ��:%02x()]\n", c[0]));
            c++;
            break;

        case 0x7d:
            dprintf((stderr, "[����ǥ���BGM���� & ��ư:%02x(%d)]\n", c[0], c[1]));
            /* ����ǥ��󥰱��ճ��� */
            LvnsStartMusic(lvns, bgmmap(c[1]));
            
			/* ����ǥ��󥰼¹� */
			SizukuEnding(lvns);

            /* �ǽ�Ū�ʥե饰��ȿ�� */
            memcpy(state->flag_save, state->flag, sizeof state->flag);

            /* ����ǥ�����ã�ˤĤ��������ʥꥪ�ǡ�������� */
            SizukuScenarioInit(lvns);
            SizukuSave(lvns);

            c += 2;
			break;

        case 0xff:
			dprintf((stderr, "���襢�����������ʤ����˥�����������??\n"));
			return;
			
        default:
            dprintf((stderr, "����å�����ʤ��ä����ޥ�ɤǤ�(%02x)\n", c[0]));
			return;
        }
	
    } /* while(1) */
}
#undef c

/**
 * ����ľ���ν����
 */
void
SizukuStart(Lvns *lvns)
{
    lvns->setPaletteIndex(lvns, SIZUKU_COL_WHITE, 255, 255, 255);
    lvns->setPaletteIndex(lvns, SIZUKU_COL_GRAY,  127, 127, 127);
    lvns->vram->black       = SIZUKU_COL_BLACK;
    lvns->background->black = SIZUKU_COL_BLACK;
    LvnsInitAnimation(lvns, sizuku01);
    LvnsInitAnimation(lvns, sizuku02);
}

/**
 * �����ɽ��
 */
void
SizukuDispHistory(Lvns *lvns, int pos)
{
	LvnsClearText(lvns);         /* �õ� */
	LvnsDispWindow(lvns);
	if (pos >= 0 && pos < lvns->history_pos) {
		LvnsLoadScenario(lvns, lvns->history[pos].scn, lvns->history[pos].blk);
		SizukuDispText(lvns, LvnsGetScenarioText(lvns, lvns->history[pos].no), True);
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

