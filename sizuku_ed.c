/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: sizuku_ed.c,v 1.27 2001/08/02 20:05:30 go Exp $
 *
 */

/* 
 * 雫 エンディング
 */

#include <stdio.h>
#include "string.h"
#include "sizuku.h"

extern u_char sizuku_haikei_palette[][4][3];

#define EDCOL 4
#define EDSHA 0

static int
tputs(Lvns *lvns, LvnsScript *scr, void *param1, void *param2)
{
    char *str = param1;
    int x = (WIDTH2 - strlen(str) * CHARSIZE/2)/2;
    int y = (int)param2 * EDYOFF;
    SizukuPutsVRAM(lvns, x+1, y+1, (u_char*)param1, EDSHA);
    SizukuPutsVRAM(lvns, x+2, y+2, (u_char*)param1, EDSHA);
    SizukuPutsVRAM(lvns, x  , y  , (u_char*)param1, EDCOL);
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

/* 雫 エンディングスクリプト */
static LvnsScriptData eddata[]= {
    { LVNS_SCRIPT_CLEAR, NULL, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_BG, (void*)2 },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "プログラム", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "ＨＡＪＩＭＥ　ＮＩＮＯＭＡＥ", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)44 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "キャラクター原案", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "高彦　龍哉", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)11 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "原画", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "水無月　徹", (void*) 7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)12 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "脚本", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "高橋　龍也", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_BG, (void*)13 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "ビジュアルグラフィックス", (void*)4 },
    { LVNS_SCRIPT_FUNC, tputs, "鳥野　正信", (void*)6 },
    { LVNS_SCRIPT_FUNC, tputs, "ＨＡＭＭＥＲ", (void*)7 },
    { LVNS_SCRIPT_FUNC, tputs, "親父油", (void*)8 },
    { LVNS_SCRIPT_FUNC, tputs, "生波夢", (void*)9 },
    { LVNS_SCRIPT_FUNC, tputs, "ＤＯＺＡ", (void*)10 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)15 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "キャラクターグラフィックス", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "ＨＡＭＭＥＲ", (void*)7 },
    { LVNS_SCRIPT_FUNC, tputs, "鳥野　正信", (void*)8 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)20 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "背景グラフィックス", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "鳥野　正信", (void*)7 },
    { LVNS_SCRIPT_FUNC, tputs, "親父油", (void*)8 },
    { LVNS_SCRIPT_FUNC, tputs, "生波夢", (void*)9 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)22 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "オリジナルフォント", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "ＨＡＭＭＥＲ", (void*)7 },
    { LVNS_SCRIPT_FUNC, tputs, "爆裂煙々黒頭巾", (void*)8 },
    { LVNS_SCRIPT_FUNC, tputs, "水無月　徹", (void*)9 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)24 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "シーン構成", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "ＯＲＩＢＡＳＳ", (void*)7 },
    { LVNS_SCRIPT_FUNC, tputs, "葉月　優一", (void*)8 },
    { LVNS_SCRIPT_FUNC, tputs, "水無月　徹", (void*)9 },
    { LVNS_SCRIPT_FUNC, tputs, "初号機", (void*)10 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)26 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "音楽", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "折戸　伸治", (void*)7 },
    { LVNS_SCRIPT_FUNC, tputs, "下川　直哉", (void*)8 },
    { LVNS_SCRIPT_FUNC, tputs, "石川　真也", (void*)9 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)27 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "ＴＥＳＴ　ＰＬＡＹ", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "ＡＬＬ　ＬＥＡＦ　ＳＴＡＦＦ", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)31 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "ＳＰＥＣＩＡＬ　ＴＨＡＮＸ", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "ＫＥＮ　ＫＥＮ", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_BG, (void*)35 },
    { LVNS_SCRIPT_WAIT, (void*)6000 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "ＡＮＤ　ＹＯＵ", (void*)6 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_WAIT, (void*)6000 },

    { LVNS_SCRIPT_BG, (void*)5 },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_DISP, (void*)LVNS_EFFECT_FADE_PALETTE },

    { LVNS_SCRIPT_WAIT, (void*)1000 },
    { LVNS_SCRIPT_FUNC, tputs, "企画・開発", (void*)5 },
    { LVNS_SCRIPT_FUNC, tputs, "１９９６　ＬＥＡＦ", (void*)7 },
    { LVNS_SCRIPT_FUNC, DispMoji },

    { LVNS_SCRIPT_WAIT, (void*)0 },

    { LVNS_SCRIPT_WAIT_CLICK },
    { LVNS_SCRIPT_CLICK_JUMP },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_END }
};

void
SizukuEnding(Lvns *lvns)
{
	LvnsScriptRun(lvns, eddata);
}

