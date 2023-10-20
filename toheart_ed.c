/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: toheart_ed.c,v 1.13 2001/08/18 02:48:49 tf Exp $
 *
 */

/*
 * ToHeart エンディング
 */

#include <stdio.h>
#include "toheart.h"

#define EDCOL 250
#define EDSHA 251

#ifdef USE_MGL
static struct {
  int x;
  int y;
  char str[128];
  int type;
} telop[10];

static int telop_num;
#endif

static int
start(Lvns *lvns, LvnsScript *scr)
{
    lvnsimage_copy(lvns->vram, lvns->background);
#ifdef USE_MGL
    telop_num = 0;
    lvns->latitude = 16;
#endif
    return True;
}

#ifndef USE_MGL
#define XF 192
#define YF 144
#else
#define XF 96
#define YF 72
#endif
static int
ToSmall(Lvns *lvns, LvnsScript *scr, void *param1, void *param2)
{
    ToHeartState *state = (ToHeartState*)lvns->system_state;
#ifndef USE_MGL
    memset(lvns->vram->data, TOHEART_COL_WHITE, 
           lvns->vram->rwidth * lvns->vram->rheight);
#else
    mglimage_clear_color(lvns->vram, COLOR_WHITE);
#endif
    lvnsimage_copy_scale(lvns->background, 0, 0, WIDTH, HEIGHT, 
                         lvns->vram, 0, 0, 
                         WIDTH  - XF * scr->state/24,
                         HEIGHT - YF * scr->state/24);
    lvns->drawWindow(lvns);
    lvns->flushWindow(lvns);
	
    if (++scr->state > 24) {
        return True;
    } else {
        return False;
    }
}
#undef XF
#undef YF

#ifdef USE_MGL
static void
SetTelop(Lvns *lvns, int x, int y, const char *str, int type)
{
  telop[telop_num].x = x;
  telop[telop_num].y = y;
  strcpy(telop[telop_num].str, str);
  telop[telop_num].type = type;
  telop_num++;
}
#endif

#define XY(x,y) (void*)(x<<8|y)
#ifndef USE_MGL
#define XF 8
#define YB 344
#define YF 30
#else
#define XF 4
#define YB 172
#define YF 15
#define ToHeartPutsVRAM SetTelop
#endif

static int
tputs(Lvns *lvns, LvnsScript *scr, void *param1, void *param2)
{
    int x = ((int)param2 >> 8) * XF;
    int y = YB + ((int)param2 & 0xff) * YF;
    ToHeartPutsVRAM(lvns, x+2, y+2, (u_char*)param1, EDSHA);
    ToHeartPutsVRAM(lvns, x  , y  , (u_char*)param1, EDCOL);
    return True;
}
#undef ToHeartPutsVRAM

static int
white(Lvns *lvns, LvnsScript *scr, 
      void *param1, void *param2)
{
    /* 文字を真っ白に… */
    lvns->vram->palette[EDCOL][0] = 255;
    lvns->vram->palette[EDCOL][1] = 255;
    lvns->vram->palette[EDCOL][2] = 255;
    lvns->vram->palette[EDSHA][0] = 255;
    lvns->vram->palette[EDSHA][1] = 255;
    lvns->vram->palette[EDSHA][2] = 255;
    lvns->vram->palette_num = EDSHA+1;
}

static int
DispSmall(Lvns *lvns, LvnsScript *scr, 
          void *param1, void *param2)
{
    ToHeartLoadBG(lvns, (int)param1);
#ifndef USE_MGL
    memset(lvns->vram->data, TOHEART_COL_WHITE, 
           lvns->vram->rwidth * lvns->vram->rheight);
    lvnsimage_copy_palette(lvns->background, lvns->vram);
    lvnsimage_copy_scale(lvns->background, 0, 0, WIDTH, HEIGHT, 
                         lvns->vram, 0, 0, 448, 336);
#else
    mglimage_clear_color(lvns->vram, COLOR_WHITE);
    lvnsimage_copy_scale(lvns->background, 0, 0, WIDTH, HEIGHT, 
                         lvns->vram, 0, 0, 224, 168);
    telop_num = 0;
#endif
    white(lvns, scr, NULL, NULL);
    return True;
}

static int
SetLogo(Lvns *lvns, LvnsScript *scr, 
        void * param1, void * param2) 
{
    ToHeartState *state = (ToHeartState*)lvns->system_state;
#ifndef USE_MGL
    lvnsimage_clear_index(lvns->background, TOHEART_COL_WHITE);
    lvnsimage_clear_index(lvns->vram,       TOHEART_COL_WHITE);
#else
    mglimage_clear_color(lvns->background, COLOR_WHITE);
    mglimage_clear_color(lvns->vram, COLOR_WHITE);
#endif
    lvnsimage_delete(state->title);
    state->title = LvnsLoadImage(lvns, "LEAF.LF2", NULL);
    ToHeartSakuraSetNumber(lvns, 40);
    LvnsSetNextBackEffect(lvns, &toHeartSakuraEffect);
    return True;
}

static int
sakura_start(Lvns *lvns, LvnsScript *scr)
{
}

#define SCR(c,a) (c + (255-c)*a/16)

static int
DispMoji(Lvns *lvns, LvnsScript *scr, 
         void *param1, void *param2)
{
    int al = 16 - scr->state;

    /* 文字に色を設定… */
    lvns->vram->palette[EDCOL][0] = SCR(255,al);
    lvns->vram->palette[EDCOL][1] = SCR(  8,al);
    lvns->vram->palette[EDCOL][2] = SCR(127,al);
    lvns->vram->palette[EDSHA][0] = SCR(255,al);
    lvns->vram->palette[EDSHA][1] = SCR(192,al);
    lvns->vram->palette[EDSHA][2] = SCR(224,al);
    
#ifdef USE_MGL
    {
      int i, color;
      u_char r, g, b;
      for (i = 0; i < telop_num; i++) {
	switch (telop[i].type) {
	case EDCOL:
	  r = (SCR(255,al) >> 4) & 0xf;
	  g = (SCR(  8,al) >> 4) & 0xf;
	  b = (SCR(127,al) >> 4) & 0xf;
	  color = mc_from_rgb(packRGB(r, g, b));
	  ToHeartPutsVRAM(lvns, telop[i].x, telop[i].y, telop[i].str, color);
	  break;
	case EDSHA:
	  r = (SCR(255,al) >> 4) & 0xf;
	  g = (SCR(192,al) >> 4) & 0xf;
	  b = (SCR(224,al) >> 4) & 0xf;
	  color = mc_from_rgb(packRGB(r, g, b));
	  ToHeartPutsVRAM(lvns, telop[i].x-1, telop[i].y, telop[i].str, color);
	  ToHeartPutsVRAM(lvns, telop[i].x, telop[i].y, telop[i].str, color);
	  break;
	}
      }
    }
#endif
    /* パレット反映 */
    lvns->setPalette(lvns);
    lvns->drawWindow(lvns);
    lvns->flushWindow(lvns);

    if (++scr->state > 16) {
        return True;
    } else {
        return False;
    }
}

#define BASETIME 19000
#define CYCLE    11000 

/* ToHeart エンディングスクリプト */
static LvnsScriptData eddata[] = {

    { LVNS_SCRIPT_MUSIC, (void*)35 },
    { LVNS_SCRIPT_TIMER_INIT },

    { LVNS_SCRIPT_TIMER_WAIT, (void*)16000 },
    { LVNS_SCRIPT_FUNC, start },
    { LVNS_SCRIPT_LOOP_FUNC, ToSmall },
    { LVNS_SCRIPT_FUNC, white },


    /* 企画 */
    { LVNS_SCRIPT_FUNC, tputs, "企画", XY(50, 1) }, 
    { LVNS_SCRIPT_FUNC, tputs, "高橋　龍也", XY(54, 2) }, 
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*0 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*0 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* 脚本 */
    { LVNS_SCRIPT_FUNC, DispSmall, (void*)31 },
    { LVNS_SCRIPT_FUNC, tputs, "脚本", XY(50, 0) }, 
    { LVNS_SCRIPT_FUNC, tputs, "高橋　龍也", XY(54, 1) }, 
    { LVNS_SCRIPT_FUNC, tputs, "青　紫", XY(54, 2) }, 
    { LVNS_SCRIPT_FUNC, tputs, "葵　もりふみ", XY(54, 3) }, 
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*1) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*1 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*1 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* 原画 */
    { LVNS_SCRIPT_FUNC, DispSmall, (void*)8 },
    { LVNS_SCRIPT_FUNC, tputs, "原画", XY(50, 0) }, 
    { LVNS_SCRIPT_FUNC, tputs, "水無月　徹", XY(54, 1) }, 
    { LVNS_SCRIPT_FUNC, tputs, "ラー・ＹＯＵ", XY(54, 2) }, 
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*2) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*2 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*2 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* シーン構成 */
    { LVNS_SCRIPT_FUNC, DispSmall, (void*)9 },
    { LVNS_SCRIPT_FUNC, tputs, "シーン構成", XY(32, 0) }, 
    { LVNS_SCRIPT_FUNC, tputs, "高橋　龍也　　水無月　徹", XY(36, 1) }, 
    { LVNS_SCRIPT_FUNC, tputs, "亜絵理　　　　ラー・ＹＯＵ", XY(36, 2) }, 
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*3) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*3 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*3 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* プログラム */
    { LVNS_SCRIPT_FUNC, DispSmall, (void*)10 },
    { LVNS_SCRIPT_FUNC, tputs, "プログラム", XY(30, 1) }, 
    { LVNS_SCRIPT_FUNC, tputs, "Ｈａｊｉｍｅ　Ｎｉｎｏｍａｅ", XY(34, 2) }, 
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*4) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*4 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*4 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* グラフィクス */
    { LVNS_SCRIPT_FUNC, DispSmall, (void*)11 },
    { LVNS_SCRIPT_FUNC, tputs, "グラフィクス", XY(50, 0) }, 
    { LVNS_SCRIPT_FUNC, tputs, "ＫＥＮＫＥＮ", XY(54, 1) }, 
    { LVNS_SCRIPT_FUNC, tputs, "生波夢", XY(54, 2) }, 
    { LVNS_SCRIPT_FUNC, tputs, "鳥野　正信", XY(54, 3) }, 
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*5) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*5 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*5 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* グラフィクス */
    { LVNS_SCRIPT_FUNC, DispSmall, (void*)12 },
    { LVNS_SCRIPT_FUNC, tputs, "グラフィクス", XY(47, 0) }, 
    { LVNS_SCRIPT_FUNC, tputs, "ねのつきゆきしろ", XY(51, 1) }, 
    { LVNS_SCRIPT_FUNC, tputs, "ろみゅ", XY(51, 2) }, 
    { LVNS_SCRIPT_FUNC, tputs, "ヴィーナス", XY(51, 3) }, 
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*6) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*6 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*6 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* グラフィクス */
    { LVNS_SCRIPT_FUNC, DispSmall, (void*)13 },
    { LVNS_SCRIPT_FUNC, tputs, "グラフィクス", XY(50, 0) }, 
    { LVNS_SCRIPT_FUNC, tputs, "ＤＯＺＡ", XY(54, 1) }, 
    { LVNS_SCRIPT_FUNC, tputs, "親父", XY(54, 2) }, 
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*7) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*7 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*7 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* 背景グラフィクス */
    { LVNS_SCRIPT_FUNC, DispSmall, (void*)14 },
    { LVNS_SCRIPT_FUNC, tputs, "背景グラフィクス", XY(50, 1) }, 
    { LVNS_SCRIPT_FUNC, tputs, "鳥野　正信", XY(54, 2) }, 
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*8) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*8 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*8 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* 音楽 */
    { LVNS_SCRIPT_FUNC, DispSmall, (void*)15 },
    { LVNS_SCRIPT_FUNC, tputs, "音　楽", XY(50, 0) }, 
    { LVNS_SCRIPT_FUNC, tputs, "下川　直哉", XY(54, 1) }, 
    { LVNS_SCRIPT_FUNC, tputs, "中上　和英", XY(54, 2) }, 
    { LVNS_SCRIPT_FUNC, tputs, "石川　真也", XY(54, 3) }, 
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*9) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*9 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*9 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* EDソング */
    { LVNS_SCRIPT_FUNC, DispSmall, (void*)7 },
    { LVNS_SCRIPT_FUNC, tputs, "ＥＤソング", XY(21, 0) }, 
    { LVNS_SCRIPT_FUNC, tputs, "作　詞：ＮＥＫＯ　作編曲：下川直哉", XY(25, 1) }, 
    { LVNS_SCRIPT_FUNC, tputs, "ギター：福原紳吾　録　音：有村健一", XY(25, 2) }, 
    { LVNS_SCRIPT_FUNC, tputs, "　歌　：あっこ", XY(25, 3) }, 
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*10) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*10 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*10 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* タイトルロゴ */
    { LVNS_SCRIPT_FUNC, DispSmall, (void*)21 },
    { LVNS_SCRIPT_FUNC, tputs, "タイトルロゴ", XY(47, 1) }, 
    { LVNS_SCRIPT_FUNC, tputs, "クラフトマン", XY(51, 2) }, 
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*11) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*11 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*11 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* TEST PLAY */
    { LVNS_SCRIPT_FUNC, DispSmall, (void*)32 },
    { LVNS_SCRIPT_FUNC, tputs, "ＴＥＳＴ　ＰＬＡＹ", XY(30, 1) },  
    { LVNS_SCRIPT_FUNC, tputs, "ＡＬＬ　ＬＥＡＦ　ＳＴＡＦＦ", XY(34, 2) },  
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*12) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*12 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*12 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* 開発 */
    { LVNS_SCRIPT_FUNC, DispSmall, (void*)17 },
    { LVNS_SCRIPT_FUNC, tputs, "開　発", XY(44, 1) }, 
    { LVNS_SCRIPT_FUNC, tputs, "１９９７　ＬＥＡＦ", XY(48, 2) }, 
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*13) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*13 + 3000) },
    { LVNS_SCRIPT_LOOP_FUNC, DispMoji },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*13 + 9500) },
    { LVNS_SCRIPT_WHITEOUT },

    /* Leaf ロゴ */
    { LVNS_SCRIPT_FUNC, SetLogo },
    { LVNS_SCRIPT_TIMER_WAIT, (void*)(BASETIME + CYCLE*14) },
    { LVNS_SCRIPT_WHITEIN },
    { LVNS_SCRIPT_FUNC, sakura_start },

    { LVNS_SCRIPT_WAIT_CLICK },
    { LVNS_SCRIPT_CLICK_JUMP },
    { LVNS_SCRIPT_CLEAR, (void*)LVNS_EFFECT_FADE_PALETTE },
    { LVNS_SCRIPT_END },
};

void
ToHeartEnding(Lvns *lvns)
{
	LvnsScriptRun(lvns, eddata);
}
