#ifndef __LvnsScript_h_
#define __LvnsScript_h_

#include "Lvns.h"

typedef struct {
    enum {
        LVNS_SCRIPT_BG,                 /* $BGX7J%m!<%I(B       */
        LVNS_SCRIPT_DISP,               /* $B2hA|I=<((B(BG$B$+$i(B) */
        LVNS_SCRIPT_DISP_VRAM,          /* $B2hA|I=<((B         */
        LVNS_SCRIPT_CLEAR,              /* $B2hA|>C5n(B         */               
        LVNS_SCRIPT_MUSIC,              /* $B2;3Z:F@8(B         */
        LVNS_SCRIPT_MUSIC_FADE,
        LVNS_SCRIPT_WAIT_MUSIC,         /* $B2;3Z=*N;BT$A(B     */
        LVNS_SCRIPT_ANIM,
        LVNS_SCRIPT_WAIT_CLICK,
        LVNS_SCRIPT_WAIT,               /* $B;XDj;~4VBT$A(B         */
        LVNS_SCRIPT_TIMER_INIT,         /* $BBT$A;~4V%?%$%^=i4|2=(B */
        LVNS_SCRIPT_TIMER_WAIT,         /* $B;XDj;~4V7P2a$^$GBT$D(B */
        LVNS_SCRIPT_FUNC,               /* $B4X?t8F$S=P$7(B         */
        LVNS_SCRIPT_LOOP_FUNC,        /* $B4X?t8F$S=P$7(B         */
        LVNS_SCRIPT_CLICK_JUMP,
		LVNS_SCRIPT_WHITEOUT,
		LVNS_SCRIPT_WHITEIN,
        LVNS_SCRIPT_EXIT,
        LVNS_SCRIPT_END
    } type;
    void *data0;
    void *data1;
    void *data2;
} LvnsScriptData;       /* OP/ED $B%9%/%j%W%H=hM}MQ(B */

typedef struct LvnsScript {
    LvnsScriptData *data;       /* $B%9%/%j%W%H%G!<%?(B       */
    int cur;                    /* $B<B9T%+!<%=%k(B           */
    int state;                  /* $B%9%F!<%H(B               */
} LvnsScript;

typedef int LvnsScriptFunc(struct Lvns *lvns, LvnsScript *scr, void *param1, void *param2);

/* LvnsScript.c OP/ED $BMQ(B $B%9%/%j%W%H=hM}(B */
void LvnsScriptRun(Lvns *lvns, LvnsScriptData *data);

#endif
