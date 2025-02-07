/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: Lvns.h,v 1.25 2001/08/17 16:55:51 tf Exp $
 *
 */

#ifndef __Lvns_h
#define __Lvns_h

#include <sys/types.h>
#include <setjmp.h>

#ifdef DEBUG
#define dprintf(a) fprintf a
#else
#define dprintf(a)
#endif

#ifndef Bool
#define Bool int
#endif

#ifndef True
#define True 1
#endif

#ifndef False
#define False 0
#endif

#include "leafpack.h"   /* Leaf Pack File   */
#include "Sound.h"
#include "Music.h"
#ifndef USE_MGL
#include "lvnsimage.h"  /* Leaf Image Data  */
#else
#include "mgl2.h"
#include "mglcol.h"
#include "mgImage.h"
#ifdef FONT_PLUS
#include "mgFont.h"
#endif /* FONT_PLUS */
#endif

#ifndef USE_MGL
#define VERSION "LEAF Visual Novel System for X\n"\
                " ==== XLVNS 1-2-3 Ver 3.0beta ====\n"\
                "(c) Copyright 1999,2000 Go Watanabe\n"\
                "Original LVNS (c)LEAF/AQUAPLUS\n"
#else
#define VERSION "LEAF Visual Novel System for MGL2\n"\
                " ==== mglvns 1-2-3 Ver 1.0beta ====\n"\
                "(c) Copyright 1999-2001 Go Watanabe\n"\
                "(c) Copyright 2001 TF\n"\
                "Original LVNS (c)LEAF/AQUAPLUS\n"
#endif

#define INTERVAL 60
#define NOCHARACTER 255

/* EUC «Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆμ΅ή«Τ©Π«Τ©Π«Τ©Πτώτώ΅ή«Τ©ΠτώΆμτώ«Τ©ΠτώΆμΆμ«Τ©Πτώτώªφ«Τ©Π«Τ©Π«Τ©Πτώτώª£ JIS «Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆμ΅ή«Τ©ΠτώΆντώτώτώª¤τώΆμτώ */
#define EucToJisPack(code)  ((((code>>8)&0x7f)-33)*94 + ((code&0x7f)-33))

#define LVNS     lvns->system_state
#define COLORS  (lvns->system_state->colors)
#define WIDTH	(lvns->system_state->width)
#define HEIGHT	(lvns->system_state->height)
#ifndef USE_MGL
#define WIDTH2       640
#define CHARSIZE     18
#define CHARDRAWSIZE 26
#define EDYOFF       30
#define XPOS(x,y)    ((x) * 24 + lvns->tvram[lvns->current_tvram].row[y].offset)
#define YPOS(y)      ((y) * 32 + 8)
#define R_XPOS(x, y) (((x) - lvns->tvram[lvns->current_tvram].row[y].offset)/24)
#define R_YPOS(y)    (((y) - 8) / 32)
#else
#define WIDTH2       320
#define CHARSIZE     12
#define CHARDRAWSIZE 14
#define EDYOFF       15
#define XPOS(x,y)    ((x) * 12 + lvns->tvram[lvns->current_tvram].row[y].offset)
#define YPOS(y)      ((y) * 14 + 4)
#define R_XPOS(x, y) (((x) - lvns->tvram[lvns->current_tvram].row[y].offset)/12)
#define R_YPOS(y)    (((y) - 4) / 14)
#endif

#ifdef USE_MGL
#define PATH_LEN 200

#ifndef MGLVNS_APPDIR
#define MGLVNS_APPDIR "/usr/local/lib/mglvns";
#endif

#define MGL_XOFFSET 80
#define MGL_YOFFSET 20

/* ΆΒ«³τώτώªΆτώΆμ΅ήτώτώªΤ«Τ©ΠτώΆμª¤τώΆμ«ΣτώΆντώτώτώª£τώτώªΤτώτώª¤τώΆμ©°«Τ©ΠτώΆνªΏτώΆμτώ«Τ©ΠτώΆμτώτώΆμª¤«Τ©ΠτώΆμª΅ MGL2 τώΆμτώτώΆμΆξ«Τ©ΠτώΆμΆξτώτώ«³«Τ©Π«Τ©Π«Τ©ΠτώΆμτώτώΆντώ«Τ©Πτώτώ©° */
#define LvnsImage mgImage
#define lvnsimage_new mglimage_new
#define lvnsimage_delete mglimage_delete
#define lvnsimage_load_lfg mglimage_load_lfg
#define lvnsimage_load_lf2 mglimage_load_lfg
#define lvnsSinEffect2 lvnsSinEffect
#define lvnsimage_add(s,d,x,y) mglimage_add(s,d,(x)/2,(y)/2)
#define lvnsimage_add2(s,d,x,y) mglimage_add(s,d,(x)/2,(y)/2)
#define lvnsimage_add3(s,d,x,y,p,t,i) mglimage_add3(s,d,(x)/2,(y)/2,p,t,i)
#define lvnsimage_add4(s,d,x,y,p) mglimage_add(s,d,(x)/2,(y)/2)
#define lvnsimage_circle(i,x,y,r,s,p) mglimage_circle(i,x,y,(r)/2,s,p)
/*
 * MGL2 τώΆμτώτώΆμΆξ«Τ©ΠτώΆμª¤τώΆμ«ΡτώτώªΆ«Τ©ΠτώτώΆξ«Τ©ΠτώΆμª΅ X τώΆμτώτώΆμΆξ«Τ©ΠτώΆμτώΆΒ«³τώτώΆμ«Τ©Π«Τ©Πτώτώª¤«Τ©Πτώτώª£ΆΒ©Μ«Τ©ΠτώτώªÒΆΒ«³τώτώ«¤τώτώª£τώτώªΤ«Τ©ΠτώΆμτώ«Τ©Π«Τ©Π«Τ©ΠτώΆμª¤τώΆμ«ΣτώΆντώτώτώª£τώτώªΤ«Τ©ΠτώτώªÒτώτώ«ΣτώΆνΆΒ«Τ©ΠτώΆμªΠτώΆνªΏ«Τ©Π«Τ©Π«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆμτώτώτώª¤«Τ©Π
 */
#define lvnsimage_clear_index mglimage_clear_color
#endif

#include <string.h>
#include <stdlib.h>

struct Lvns;

#include "LvnsMenu.h"

typedef enum {
	LVNS_JUMP_NONE,
	LVNS_JUMP_RESTART,
	LVNS_JUMP_TITLE,
	LVNS_JUMP_END
} LvnsJumpParameter;

typedef enum {
	LVNSTIMER_MAIN,
	LVNSTIMER_EFFECT,
	LVNSTIMER_SCRIPT,
} LvnsTimerType;

typedef enum {
    LVNS_CURSOR_PAGE,
    LVNS_CURSOR_KEY,
}LvnsCursorType;

typedef void (*LvnsBackEffectSetFunc)(struct Lvns *lvns);
typedef void (*LvnsBackEffectFunc)(struct Lvns *lvns);
typedef struct {
    LvnsBackEffectSetFunc set;
    LvnsBackEffectFunc func;
} LvnsBackEffectInfo;

typedef struct {
	int scn;  /* «Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐτώΆμτώτώΆμªΡ«Τ©ΠτώΆνª΅ */
	int blk;  /* «Τ©ΠτώΆμªΡ«Τ©ΠτώτώΆΔ«Τ©ΠτώΆμΆμ«Τ©ΠτώτώªφτώΆμτώτώΆμªΡ«Τ©ΠτώΆνª΅ */
	int no;   /* «Τ©ΠτώΆνΆΒ«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆμτώτώΆμτώτώΆμªΡ«Τ©ΠτώΆνª΅ */
} LvnsHistoryData;        /* «Τ©ΠτώτώªΠτώΆμΆΒτώΆμ«©«Τ©Πτώτώτώτώτώª¤τώΆν«Τ©ΠτώΆμ΅ήτώΆμΆΔτώΆμτώ */

typedef struct {
    char *command;
    int enable;
} LvnsCommandInfo;   /* «Τ©ΠΆΒ«³τώΆμ΅ήτώτώªΤτώΆμτώτώτώ«Ώτώτώ«ΣτώΆνª΅τώΆμΆΔτώΆμτώ */

typedef struct {

    int width;                   /* τώΆνΆΒτώτώτώτώΆμ΅ήτώτώªΤτώΆμ΅ήτώΆμΆΒ«Τ©ΠτώτώτώτώΆμª¤τώΆμª¤ΆΒ«³τώτώτώ«Τ©ΠτώΆμª¤«Τ©Π«Τ©Π«Τ©Π«Τ©Π«Τ©Πτώτώ«³ */
    int height;             
    int colors;                  /* τώΆμ΅ή«Τ©ΠτώΆμΆΔΆΒ©Ξ«Τ©ΠτώΆμªφτώτώ«³τώΆμΆξτώΆμτώτώτώΆξτώτώª£ΆΒ©Μτώτώª£τώτώªΤ       */ 

    const char *leafpack_name;   /* «Τ©ΠτώΆμΆξτώτώª¤τώΆν«Τ©Πτώτώª£τώΆμΆΔτώΆμτώ«Τ©ΠτώΆμτώ«Τ©ΠτώΆμΆμ«Τ©Πτώτώªφ«Τ©ΠτώΆμ©¬«Τ©Πτώτώª¤«Τ©Π«Τ©Π«Τ©ΠτώτώτώτώΆμªΆτώΆμ«ΡτώΆμ«ΡτώΆμª¤   */
    const char *scnpack_name;    /* «Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐτώΆμΆΔτώΆμτώ«Τ©ΠτώΆμτώ«Τ©ΠτώΆμΆμ«Τ©Πτώτώªφ«Τ©ΠτώΆμ©¬«Τ©Πτώτώª¤«Τ©Π«Τ©Π«Τ©ΠτώτώτώτώΆμªΆτώΆμ«ΡτώΆμ«ΡτώΆμª¤ */
    const char *scn_name;        /* «Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐ«Τ©ΠτώΆμ©¬«Τ©Πτώτώª¤«Τ©Π«Τ©Π«Τ©Πτώτώτώ«Τ©ΠτώΆμ«ªτώτώª¤τώΆν«Τ©Π«Τ©ΠτώΆμªΆτώΆμ«ΡτώΆμ«ΡτώΆμª¤ */
    const char *fonttable_name;  /* «Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆμ΅ή«Τ©Π«Τ©Π«Τ©ΠτώτώªΠ«Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆμτώ«Τ©ΠτώΆνΆΒτώτώª¤τώΆν«Τ©ΠτώΆμªΡ«Τ©ΠτώτώτώτώΆμªΆτώΆμ«ΡτώΆμ«ΡτώΆμª¤ */

	MenuData *main_menu;

	void (*start)(struct Lvns *lvns);    /* τώΆνªΏτώτώ΅ήτώΆμ«Στώτώ«£«Τ©ΠτώΆνªΏ       */
	void (*jingle)(struct Lvns *lvns);   /* «Τ©Πτώτώ«Σ«Τ©ΠτώτώªΠ«Τ©ΠΆΒ«³«Τ©Πτώτώτώ     */
	void (*opening)(struct Lvns *lvns);  /* «Τ©Πτώτώ«ΐτώτώª¤τώΆν«Τ©ΠΆΒ©Ξ«Τ©ΠτώΆμτώ«Τ©ΠτώτώªΠ«Τ©ΠΆΒ«³ */
	void (*title)(struct Lvns *lvns);    /* «Τ©Πτώτώª£«Τ©Π«Τ©Π«Τ©ΠτώΆμτώ«Τ©Πτώτώτώ     */
	void (*main)(struct Lvns *lvns);     /* «Τ©ΠτώτώΆΒ«Τ©Π«Τ©Π«Τ©ΠτώτώªΠ       */

    void (*mergeCharacter)(struct Lvns *lvns);   /* «Τ©Π«Τ©ΠτώτώΆμ«Τ©Πτώτώ΅ή«Τ©Πτώτώªφ«Τ©Πτώτώª£«Τ©ΠτώτώΆξτώΆμτώτώτώ«Β */
    void (*clearScreen)(struct Lvns *lvns);      /* «Τ©ΠτώΆμ«ΡτώΆμΆΒτώΆμ«© VRAM «Τ©ΠτώΆμª¤τώΆμ«ΡτώΆμΆμ«Τ©Πτώτώª¤ */
    void (*loadBG)(struct Lvns *lvns, int no);   /* τώΆμΆξτώΆντώ«Τ©ΠτώΆμªφτώΆνΆΒτώΆμ΅ή«Τ©ΠτώΆνªΒ«Τ©ΠτώΆντώ«Τ©ΠτώΆνªΒ */

    LvnsCommandInfo* (*getCommandList)(struct Lvns *lvns);           /* «Τ©ΠΆΒ«³τώΆμ΅ήτώτώªΤτώΆμΆΒτώτώτώτώτώ«³τώτώª¤ΆΒ«³τώτώªΆτώΆμΆΔΆΒ«Ά */
    void (*execCommand)(struct Lvns *lvns, const char *command);     /* «Τ©ΠΆΒ«³τώΆμ΅ήτώτώªΤτώΆμΆΒτώτώτώτώτώ«³τώτώª¤     */
    void (*drawChar)(struct Lvns *lvns, int x, int y, int, int);     /* τώΆμªφτώτώ«Σ«Τ©Πτώτώ«¤τώΆμ΅ήτώΆνªΏτώΆντώτώ©°     */

	void (*dispHistory)(struct Lvns *lvns, int pos);

    int cursor_key;   /* «Τ©Πτώτώª¤τώΆντώΆνΆΒτώΆντώτώΆμª¤τώΆμª΅τώΆμτώτώΆμªΤ«Τ©ΠτώΆμΆΒ«Τ©ΠτώΆμª¤«Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆνªΏ«Τ©Πτώτώτώ */
    int cursor_page;  /* «Τ©ΠτώΆντώ«Τ©ΠτώΆμ«¤τώτώª¤τώΆν«Τ©Πτώτώ«ΣτώΆμτώτώΆμªΤ«Τ©ΠτώΆμΆΒ«Τ©ΠτώΆμª¤«Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆνªΏ«Τ©Πτώτώτώ */

} LvnsSystemState;

typedef struct Lvns {

	enum {
		LVNS_JINGLE,
		LVNS_OP,
		LVNS_MAIN,
		LVNS_WAIT_MENU,
		LVNS_WAIT_KEY,
		LVNS_WAIT_PAGE,
		LVNS_WAIT_SELECT,
	} inside_state;

    /* ------------------------------------------------------------ */
	/* «Τ©ΠτώΆμΆξτώτώª¤τώΆν«Τ©Πτώτώª£«Τ©ΠτώΆμτώ«Τ©Πτώτώ΅ή«Τ©ΠτώτώΆΒτώτώª¤τώΆν«Τ©Πτώτώª£ */

    LvnsSystemState *system_state;     /* «Τ©Π«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆνΆΒ«Τ©Πτώτώτώ«Τ©Π«Τ©Π«Τ©ΠτώΆνΆΒτώτώª¤τώΆν«Τ©ΠτώΆμτώ */

    char *data_path;        /* «Τ©ΠτώΆμΆξτώτώª¤τώΆν«Τ©Πτώτώª£τώτώ«Στώτώª¤τώτώ«³ΆΒ«ΆτώΆμ«ΣτώΆνªªτώΆμτώτώΆμ«£«Τ©ΠτώΆμτώ«Τ©Π«Τ©Π */
    char *savedata_path;    /* «Τ©ΠτώΆμΆξτώτώª¤τώΆν«Τ©Πτώτώª£τώΆμªφτώΆμ«ÒτώΆμτώτώτώ«ΣΆΒ«³τώΆμªΆτώΆμΆμτώΆμªΡ */
 
    LeafPack *leafpack;     /* «Τ©ΠτώΆμΆξτώτώª¤τώΆν«Τ©Πτώτώª£τώΆμΆΔτώΆμτώ«Τ©ΠτώΆμτώ«Τ©ΠτώΆμΆμ«Τ©Πτώτώªφ«Τ©ΠτώΆμ©¬«Τ©Πτώτώª¤«Τ©Π«Τ©Π«Τ©Πτώτώτώ */
    LeafPack *scnpack;      /* «Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐτώΆμΆΔτώΆμτώ«Τ©ΠτώΆμτώ«Τ©ΠτώΆμΆμ«Τ©Πτώτώªφ«Τ©ΠτώΆμ©¬«Τ©Πτώτώª¤«Τ©Π«Τ©Π«Τ©Πτώτώτώ */
    u_char *leaf_font_data; /* Leaf FONT «Τ©ΠτώΆμª¤«Τ©ΠτώΆμΆξτώτώª¤τώΆν«Τ©Πτώτώª£   */

    u_char *leaf_to_euc;    /* «Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆμ΅ή«Τ©Π«Τ©Π«Τ©ΠτώτώªΠ«Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆμτώ«Τ©ΠτώΆνΆΒτώτώª¤τώΆν«Τ©ΠτώΆμªΡ«Τ©Πτώτώτώ Leaf -> EUC  */
    u_short *jis_to_leaf;   /* «Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆμ΅ή«Τ©Π«Τ©Π«Τ©ΠτώτώªΠ«Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆμτώ«Τ©ΠτώΆνΆΒτώτώª¤τώΆν«Τ©ΠτώΆμªΡ«Τ©Πτώτώτώ JIS Pack -> Leaf */

    /* ------------------------------------------------------ */
    /* «Τ©Πτώτώªφ«Τ©ΠτώΆνªΏτώτώª¤τώΆν«Τ©Π«Τ©Π */

    Bool force_skip;      /* τώτώ«Σ«Τ©Π«Τ©Πτώτώª£τώΆμªφτώτώ«ΣτώΆνªΏτώτώτώ«Τ©ΠτώΆμΆξ«Τ©Πτώτώτώ«Τ©Π«Τ©Π«Τ©ΠτώΆμΆμ«Τ©ΠτώΆμτώ«Τ©Π«Τ©Π«Τ©Π«Τ©Π           */
    Bool key_click_fast;  /* «Τ©Πτώτώª¤τώΆν«Τ©Πτώτώªφ«Τ©Πτώτώªφ«Τ©ΠτώΆμΆμ«Τ©Πτώτώªφ«Τ©ΠτώΆμΆξτώΆμªφτώτώ«ΣτώΆνªΏτώτώτώ«Τ©ΠτώτώªÒ«Τ©ΠτώτώτώτώΆμτώτώτώ«ΒτώΆμ΅ήτώΆνªΏτώΆντώτώ©°«Τ©ΠτώτώªΡ«Τ©ΠτώΆμτώ */
	Bool fast_when_seen;  /* τώΆμ«Στώτώ«©τώΆνΆΒτώΆμ΅ήτώΆμªφτώτώ«ΣτώΆνªΏτώτώτώ«Τ©ΠτώΆμª΅τώΆμΆΒτώτώΆΒ«Τ©ΠτώτώªφτώΆμ΅ήτώΆνªΏτώΆντώτώ©°«Τ©Π«Τ©Π«Τ©Πτώτώτώ           */
    Bool enable_effect;       /* τώτώ«Στώτώ«¤«Τ©ΠτώΆμªΆ«Τ©ΠτώτώªÒ«Τ©ΠτώΆμτώ«Τ©ΠτώΆμ©° */
    Bool enable_effect_back;  /* τώΆμΆξτώΆντώ«Τ©ΠτώΆμªφτώτώ«Στώτώ«¤«Τ©ΠτώΆμªΆ«Τ©ΠτώτώªÒ«Τ©ΠτώΆμτώ«Τ©ΠτώΆμ©° */

    Bool seen_mode;       /* τώΆμΆΒτώΆμ«ΣτώΆμªφτώτώ«ΣτώΆμ«Στώτώ«©τώΆνΆΒτώΆμ΅ή«Τ©ΠτώΆμτώ«Τ©ΠτώΆνªΒ«Τ©ΠτώΆμªφ«Τ©Π«Τ©Π«Τ©Πτώτώτώτώτώª¤τώΆν«Τ©ΠτώΆμ΅ή(for DEBUG) */
    Bool demo_mode;       /* «Τ©Πτώτώ«ΐτώτώª¤τώΆν«Τ©ΠτώΆμτώ«Τ©ΠτώΆμΆξ«Τ©Πτώτώτώ«Τ©Πτώτώτώτώτώª¤τώΆν«Τ©ΠτώΆμ΅ή */

    /* ------------------------------------------------------ */
    /* «Τ©Π«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆνΆΒ«Τ©Πτώτώτώ«Τ©ΠτώΆμτώ«Τ©Πτώτώ΅ή«Τ©ΠτώτώΆΒτώτώª¤τώΆν«Τ©Πτώτώª£ */

    Bool image_dark;    /* τώτώ«ΣτώΆνªΏτώτώ«³τώΆνªΒ«Τ©ΠτώΆμª¤ΆΒ«³τώΆμΆν«Τ©Π«Τ©Π                      */

    int  char_wait_time;         /* τώΆμªφτώτώ«Σ«Τ©Πτώτώ«¤τώΆμ΅ήτώΆνªΏτώΆντώτώ©°τώΆμτώτώΆμªΤ«Τ©ΠτώΆμΆΒ«Τ©ΠτώΆντώτώΆμ«ΣτώΆμªΡ     */
    
    Bool fast_text;    /* «Τ©ΠτώΆνΆΒ«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆμτώ«Τ©Π«Τ©Π«Τ©ΠτώΆμΆμτώΆμτώτώτώªΤ«Τ©Π«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆμ©¬«Τ©Πτώτώ΅ή«Τ©ΠΆΒ«³       */
    Bool fast_disp;    /* τώΆμ΅ήτώΆνªΏτώΆντώτώ©°«Τ©Πτώτώ©°«Τ©ΠτώΆμ©¬«Τ©ΠΆΒ©Μ«Τ©Πτώτώªφ«Τ©ΠτώΆμτώ«Τ©Π«Τ©Π«Τ©ΠτώΆμΆμ«Τ©ΠτώΆμτώ«Τ©Π«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆμ©¬«Τ©Πτώτώ΅ή«Τ©ΠΆΒ«³ */

	jmp_buf jump;

    /* ------------------------------------------------------ */
    /* τώτώ«ΣτώΆνªΏτώτώ«³τώΆνªΒ«Τ©ΠτώΆμª¤τώΆμ«Ρτώτώ©¬τώΆμτώτώΆμªΡ «Τ©ΠτώΆμΆξτώτώª¤τώΆν«Τ©Πτώτώª£«Τ©ΠτώΆμª¤τώΆμªφτώΆμ«ÒτώΆμτώτώτώ«ΣτώΆμΆΔτώΆμτώ */
    enum bgtype {
        LVNS_VISUAL,
        LVNS_HCG,
        LVNS_BACKGROUND
    } bg_type;
    int bg_no;

	int character_no[3];

    Bool savepoint_flag;
    struct SavePoint{
        enum bgtype bg_type;
        int bg_no;
		int character_no[3];
        int scn;         /* «Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐτώΆμτώτώΆμªΡ«Τ©ΠτώΆνª΅τώΆμªφτώΆμ«ÒτώΆμτώτώτώ«ΣτώΆμΆΔτώΆμτώ */
        int blk;         /* «Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐ«Τ©ΠτώΆμªΡ«Τ©ΠτώτώΆΔ«Τ©ΠτώΆμΆμ«Τ©ΠτώτώªφτώΆμªφτώΆμ«ÒτώΆμτώτώτώ«ΣτώΆμΆΔτώΆμτώ       */
        long scn_offset; /* «Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐ«Τ©ΠτώΆμΆξτώτώª¤τώΆν«Τ©Πτώτώª£τώΆμªφτώΆμ«ÒτώΆμτώτώτώ«ΣτώΆμΆΔτώΆμτώ«Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆνªΏ«Τ©Πτώτώτώ */
        int current_music;
    } savepoint;
    struct SavePoint selectpoint;

    /* ---------------------------------------------------------- */
    /* τώΆμΆΒτώτώτώτώτώ«³τώτώª¤τώΆμ«Ρτώτώ©¬τώΆμτώτώΆμªΡ */

	/* «Τ©ΠΆΒ«³τώΆμ΅ήτώτώªΤτώΆμτώτώτώ«Ώτώτώ«ΣτώΆνª΅τώΆμ«Ρτώτώ©¬τώΆμτώτώΆμªΡ */
    Bool select;
	Bool cancel;
	Bool skip;
	Bool cursor_up;
	Bool cursor_down;
	Bool motion;
	int  motion_x;
	int  motion_y;
	int  number;

	Bool image_mode;
	
    /* ---------------------------------------------------------- */
    /* «Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐ«Τ©ΠτώΆμτώτώτώª¤τώΆν«Τ©Π«Τ©Π */

    int start_scn_num;    /* «Τ©ΠτώτώªφτώΆνΆΒΆΒ«³«Τ©ΠτώΆντώ«Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐτώΆμτώτώΆμªΡ«Τ©ΠτώΆνª΅(for DEBUG) */

    u_char *scn_data;     /* «Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐ«Τ©ΠτώΆμΆξτώτώª¤τώΆν«Τ©Πτώτώª£             */
    u_char *scn_cur_head; /* «Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐ«Τ©ΠτώΆμΆξτώτώª¤τώΆν«Τ©Πτώτώª£τώΆμτώτώτώτώτώΆνΆΒ«Τ©Π         */
    const  u_char *scn_cur;      /* «Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐ«Τ©ΠτώΆμΆξτώτώª¤τώΆν«Τ©Πτώτώª£«Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆνªΏ«Τ©Πτώτώτώ     */
    u_char *scn_text;     /* «Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐ«Τ©ΠτώΆνΆΒ«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆμτώ           */

    size_t  scn_length;
	Bool    seen;         /* τώΆμ«Στώτώ«©τώΆνΆΒτώΆμ΅ή«Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐ«Τ©ΠτώΆμ©¬«Τ©Πτώτώ΅ή«Τ©ΠΆΒ«³         */

    int scn_current;                       /* τώτώ«ΣτώΆνªΏτώτώ«³τώΆνªΒ«Τ©ΠτώΆμª¤«Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐτώΆμτώτώΆμªΡ«Τ©ΠτώΆνª΅     */
    int blk_current;                       /* τώτώ«ΣτώΆνªΏτώτώ«³τώΆνªΒ«Τ©ΠτώΆμª¤«Τ©Π«Τ©Π«Τ©ΠτώΆμªφ«Τ©Πτώτώªφ«Τ©Πτώτώ«ΐ«Τ©ΠτώΆμªΡ«Τ©ΠτώτώΆΔ«Τ©ΠτώΆμΆμ«Τ©Πτώτώªφ */


    /* ------------------------------------------------------ */
    /* «Τ©ΠτώΆνΆΒ«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆμτώ«Τ©ΠτώτώªΆ«Τ©Π«Τ©Π«Τ©ΠτώτώτώτώΆνªΏτώτώτώτώΆμΆΔτώτώ«Ò«Τ©ΠτώΆμª΅ */

#define TEXT_WIDTH  26
#define TEXT_HEIGHT 16

    struct {
        /* «Τ©ΠτώΆνΆΒ«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆμτώ«Τ©ΠτώτώªΆ«Τ©Π«Τ©Π«Τ©ΠτώτώτώτώΆμΆΔτώΆμτώ«Τ©ΠτώΆμ«ΡτώΆμΆΒτώΆμ«©VRAM */
		struct TextVramLine {
			int offset;
			struct TextVram {
				int code;       /* τώΆμªφτώτώ«Σ«Τ©Πτώτώ«¤«Τ©ΠτώΆμª¤«Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆμ΅ή       */
				int attribute;  /* τώΆμªφτώτώ«Σ«Τ©Πτώτώ«¤«Τ©ΠτώΆμª¤τώΆμτώΆΒ«³τώΆμτώ         */
			} column[TEXT_WIDTH];
		} row[TEXT_HEIGHT];
        int cur_x;
        int cur_y;
        int o_cur_x;
        int o_cur_y;
    } tvram[2];
    int current_tvram;

    /* «Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆνªΏ«Τ©Πτώτώτώ«Τ©ΠτώΆμª¤τώΆμ«Ρτώτώ©¬τώΆμτώτώΆμªΡ */
    int cursor_state;
    
    /* «Τ©ΠτώΆνΆΒ«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆμτώ«Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώΆνªΏ«Τ©Πτώτώτώ«Τ©ΠτώΆμª¤τώΆμ«Ρτώτώ©¬τώΆμτώτώΆμªΡ */
    int text_cursor_state;     

	/* τώΆμ΅ήτώΆνªΏτώΆντώτώ©°τώΆμτώΆΒ«³τώΆμτώ */
    int text_attr;

	/* τώΆμªφτώτώ«Σ«Τ©Πτώτώ«¤«Τ©Π«Τ©ΠτώΆνªΏτώτώτώ«Τ©Π«Τ©ΠτώΆντώ«Τ©ΠτώΆν΅λ«Τ©ΠτώτώªΆ«Τ©ΠτώΆνΆΒ«Τ©Π«Τ©Π«Τ©Πτώτώτώ«Τ©Π«Τ©Π? */
	Bool text_written;

	/* «Τ©ΠτώΆνΆΒ«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆμτώτώΆμ΅ήτώΆνªΏτώΆντώτώ©°«Τ©Πτώτώτώτώτώª¤τώΆν«Τ©ΠτώΆμ΅ή«Τ©Π«Τ©Π«Τ©ΠτώΆμ΅ή«Τ©ΠτώΆμ©°«Τ©Π«Τ©Π */
	Bool text_mode;

    /* ------------------------------------------------------- */
    /* «Τ©ΠτώτώτώτώΆμΆΒτώτώ«£τώΆμ΅ήτώΆμΆΒ«Τ©Πτώτώτώ«Τ©ΠτώΆμª΅ */

    LvnsImage *background; /* τώΆμΆξτώΆντώ«Τ©ΠτώΆμªφ«Τ©ΠτώΆμΆξτώτώª¤τώΆν«Τ©Πτώτώª£τώΆμªφτώΆμ«Ò«Τ©Πτώτώ«ÒτώΆμΆΔτώΆμτώ        */
    LvnsImage *vram;       /* τώΆμΆξτώΆντώ«Τ©ΠτώΆμªφ«Τ©ΠτώτώΆξτώΆμτώτώτώ«Βτώτώ«³τώτώª¤τώ«γτώΆμτώτώΆμΆΔτώΆμτώ«Τ©ΠτώΆμ«ΡτώΆμΆΒτώΆμ«© VRAM */

    int latitude;          /* τώτώ«ΣτώΆνªΏτώτώ«³τώΆνªΒ«Τ©ΠτώΆμª¤«Τ©ΠτώτώτώτώΆμΆΒτώτώ«£«Τ©ΠτώΆμª¤τώΆμªΆτώΆμτώ«Τ©Πτώτώτώ«Τ©Π«Τ©Π 0-255 */
    int latitude_dark;     /* «Τ©ΠτώτώτώτώΆμªΆτώΆμªΆ«Τ©Π«Τ©ΠΆΒ«³τώΆμΆν«Τ©Π«Τ©Π«Τ©ΠτώΆντώ«Τ©ΠτώΆμª¤«Τ©ΠΆΒ©°τώΆμΆντώΆμ«ª«Τ©ΠτώΆμª¤«Τ©ΠτώΆντώτώΆμτώτώτώªφ */

    int sepia_mode_next;   /* τώΆντώτώª¤«Τ©ΠτώΆμª¤«Τ©ΠτώτώτώτώΆμªΆτώΆμªΆτώΆμτώτώΆμ«¤«Τ©ΠτώτώªφτώΆμτώτώΆντώ«Τ©Πτώτώ©°«Τ©ΠτώΆμΆξ«Τ©Π«Τ©Π«Τ©ΠτώΆμªΤ«Τ©Π«Τ©ΠτώΆμτώτώΆμ«ΣτώΆμ΅ήτώΆνªΏτώΆντώτώ©° */
    int sepia_mode;        /* «Τ©Π«Τ©Π«Τ©ΠτώΆμªΤ«Τ©Π«Τ©ΠτώΆμτώτώΆμ«ΣτώΆμ΅ήτώΆνªΏτώΆντώτώ©° */

    LvnsBackEffectInfo *effect_back;
    LvnsBackEffectInfo *effect_back_next;
    int effect_back_state;                 /* τώΆμΆξτώΆντώ«Τ©ΠτώΆμªφ«Τ©Πτώτώ©°«Τ©ΠτώΆμ©¬«Τ©ΠΆΒ©Μ«Τ©Πτώτώªφ«Τ©ΠτώΆμτώτώΆμΆΔτώΆμτώ«Τ©Π«Τ©Π«Τ©ΠτώΆνΆΒτώτώª¤τώΆν«Τ©ΠτώΆμτώ */
#ifdef FONT_PLUS
    u_char*          font_file;
    LvnsFont	     *font;
#endif /* FONT_PLUS */

	void (*disp_func)(struct Lvns *lvns, MenuData *data); /* τώΆμτώτώΆμ΅ή«Τ©ΠτώΆμΆμτώΆμ΅ήτώΆνªΏτώΆντώτώ©°τώΆμ«ΣτώΆντώτώτώª£τώτώªΤ */
	void *disp_func_param;

    /* -------------------------------------------------------- */
    /* «Τ©Π«Τ©Π«Τ©ΠτώΆμ«¤τώτώ«³τώΆνΆΒτώΆμτώτώτώ«Σ«Τ©ΠτώΆμª΅ */
	Music *music;

    /* «Τ©Πτώτώτώτώτώª¤τώΆν«Τ©ΠΆΒ©ΞτώΆνªΏτώτώτώτώΆμΆΔτώτώ«ÒτώΆμΆΔτώΆμτώ */
    int current_music;                            /* τώτώ«ΣτώΆνªΏτώτώ«³τώΆνªΒΆΒ©°τώτώ΅ήτώΆμΆΒτώΆμ©¬τώΆμΆμτώΆνª΅«Τ©ΠτώΆμª¤«Τ©Π«Τ©Π«Τ©ΠτώΆμ«¤     */
    Bool loop_music;                              /* «Τ©Πτώτώτώτώτώª¤τώΆν«Τ©ΠΆΒ©Ξ«Τ©Π«Τ©Π«Τ©ΠτώΆμ΅ή«Τ©ΠτώΆμ©°«Τ©Π«Τ©Π       */
    int next_music;                               /* τώΆντώτώª¤«Τ©ΠτώΆμª¤«Τ©Π«Τ©Πτώτώª¤τώΆν«Τ©ΠτώτώªΠ«Τ©Π«Τ©Π«Τ©Πτώτώ΅ή«Τ©ΠτώΆμª¤«Τ©Π«Τ©Π«Τ©ΠτώΆμ«¤ */
    Bool loop_next_music;                         /* «Τ©Πτώτώτώτώτώª¤τώΆν«Τ©ΠΆΒ©Ξ«Τ©Π«Τ©Π«Τ©ΠτώΆμ΅ή«Τ©ΠτώΆμ©°«Τ©Π«Τ©Π       */

	/* «Τ©ΠτώΆμ«£«Τ©Πτώτώªφ«Τ©ΠτώτώΆντώτώª¤τώΆν«Τ©ΠτώτώτώτώΆνªΏτώτώτώτώΆμΆΔτώτώ«ÒτώΆμΆΔτώΆμτώ */
    int current_music_volume;                     /* τώτώ«ΣτώΆνªΏτώτώ«³τώΆνªΒ«Τ©ΠτώΆμª¤«Τ©ΠτώΆμ«£«Τ©Πτώτώªφ«Τ©ΠτώτώΆντώτώª¤τώΆν«Τ©ΠτώτώτώτώΆμτώτώΆνªΒτώΆμτώτώτώªφ */
    int music_fade_mode;                          /* «Τ©ΠτώΆμ©¬«Τ©ΠΆΒ©Μτώτώª¤τώΆν«Τ©ΠτώΆμ΅ή«Τ©Πτώτώτώτώτώª¤τώΆν«Τ©ΠτώΆμ΅ή       */
    int music_fade_flag;                          /* «Τ©ΠτώΆμ©¬«Τ©ΠΆΒ©Μτώτώª¤τώΆν«Τ©ΠτώΆμ΅ή«Τ©ΠτώΆμª¤τώΆμªφτώτώ«Òτώτώ«ΣτώΆντώ       */

    /* -------------------------------------------------------- */
    /* «Τ©Π«Τ©Π«Τ©ΠτώΆμ©°«Τ©ΠτώτώªΠ«Τ©ΠτώΆμ΅ήτώτώ«³τώΆνΆΒτώΆμτώτώτώ«Σ«Τ©ΠτώΆμª΅ */
	Sound *sound;

    /* «Τ©Πτώτώτώτώτώª¤τώΆν«Τ©ΠΆΒ©ΞτώΆνªΏτώτώτώτώΆμΆΔτώτώ«ÒτώΆμΆΔτώΆμτώ */
    int sound_loop;
    int sound_count;
    int interval_count;

    /* -------------------------------------------------------- */
	/* «Τ©ΠτώΆμªÒ«Τ©Π«Τ©Π«Τ©ΠτώΆμτώ«Τ©Πτώτώªφ */
	LvnsHistoryData *history;
	int history_size;
	int history_pos;

#ifdef USE_MGL
    /* -------------------------------------------------------- */
    /* mglvns «Τ©ΠτώΆμª¤«Τ©ΠτώΆνªΒ«Τ©ΠτώΆμΆξ«Τ©ΠτώΆμτώ«Τ©ΠτώΆμ©°τώΆμ«Ρτώτώ©¬τώΆμτώτώΆμªΡτώΆμΆμτώΆμΆΔ */
    int version;
#endif

    /* -------------------------------------------------------- */
	/* «Τ©Π«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆνΆΒ«Τ©ΠτώτώτώΆΒ«³τώΆμΆΔτώΆμτώτώτώ«ΣτώΆμ΅ήτώτώªΤ */

    void *system_depend;                /* τώΆνΆΒΆΒ«³τώτώ«³τώτώª¤«Τ©Π«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆνΆΒ«Τ©ΠτώτώτώΆΒ«³τώΆμΆΔτώΆμτώτώτώ«ΣτώΆμ«ΡτώΆνªªτώΆμªφτώτώªΠ */

	void (*flushWindow)(struct Lvns *lvns);
	void (*flushWindowArea)(struct Lvns *lvns, int x, int y, int w, int h);
	void (*drawWindow)(struct Lvns *lvns);
	void (*drawWindowArea)(struct Lvns *lvns, int x, int y, int w, int h, int x2, int y2);
	void (*clearWindow)(struct Lvns *lvns);
	void (*clearWindowArea)(struct Lvns *lvns, int x, int y, int w, int h);
	void (*setPaletteIndex)(struct Lvns *lvns, int idx, int r, int g, int b);
	void (*setPalette)(struct Lvns *lvns);
	void (*setPaletteMulti)(struct Lvns *lvns, int par16);
	void (*setPaletteScreen)(struct Lvns *lvns, int par16);
	void (*drawWindowMask)(struct Lvns *lvns, int x, int y, int state);
	void (*drawWindowSquareMask)(struct Lvns *lvns, int x, int y, int state);
	void (*setDispOffset)(struct Lvns *lvns, int xoff, int yoff, int maxoff);
	void (*putPattern)(struct Lvns *lvns, int x, int y, int index, u_char *data);
	void (*putPattern2)(struct Lvns *lvns, int x, int y, int index, u_char *data);
	void (*resetTimer)(struct Lvns *lvns, int no);
	long (*getTimer)(struct Lvns *lvns, int no);
	void (*drawBox)(struct Lvns *lvns, int x, int y, int w, int h, int idx);
	time_t (*getFileTime)(struct Lvns *lvns, const char *path);
	void (*flip)(struct Lvns *lvns);

    /* -------------------------------------------------------- */


} Lvns;

/* LvnsDisp.c τώΆμ΅ήτώΆνªΏτώΆντώτώ©°«Τ©ΠτώΆμª΅ */
void LvnsDrawTextLayerAll(Lvns *lvns);
void LvnsDrawTextLayer(Lvns *lvns);
void LvnsClearCursor(Lvns *lvns);
void LvnsDrawCursor(Lvns *lvns, LvnsCursorType cursor_type);
void LvnsClearTextCursor(Lvns *lvns);
void LvnsDrawTextCursor(Lvns *lvns);
void LvnsDispWindow(Lvns *lvns);
void LvnsUndispText(Lvns *lvns);
void LvnsDispText(Lvns *lvns);
void LvnsTextOff(Lvns *lvns);
void LvnsTextOn(Lvns *lvns);

/* LvnsText.c «Τ©ΠτώΆνΆΒ«Τ©Π«Τ©Π«Τ©Π«Τ©ΠτώΆμτώτώΆνªΏτώτώτώτώΆμΆΔτώτώ«Ò«Τ©ΠτώΆμª΅ */
void LvnsLocate(Lvns *lvns, int x, int y);
void LvnsPutChar(Lvns *lvns, int c, int attr);
void LvnsPutCharNormal(Lvns *lvns, int c, int attr);
void LvnsPuts(Lvns *lvns, const u_char *str, int attr);
void LvnsClearText(Lvns *lvns);
void LvnsSetTextOffset(Lvns *lvns, int offset);
void LvnsNewLineText(Lvns *lvns);
void LvnsDrawString(Lvns *lvns, int x, int y, const u_char *str, int attr);
void LvnsDrawStringCenter(Lvns *lvns, int y, const u_char *str, int attr);

/* LvnsMusic.c «Τ©Π«Τ©Π«Τ©ΠτώΆμ«¤τώΆμτώτώτώ«Ώτώτώ«ΣτώΆνª΅ */
void LvnsSetMusicVolume(Lvns *lvns, int no);
void LvnsStartMusic(Lvns *lvns, int no);
void LvnsStopMusic(Lvns *lvns);
void LvnsPauseMusic(Lvns *lvns);
void LvnsStartMusicLoop(Lvns *lvns, int no);
void LvnsStartMusicLoop2(Lvns *lvns, int no);
void LvnsSetNextMusic(Lvns *lvns, int no);
void LvnsSetNextMusicLoop(Lvns *lvns, int no);
void LvnsFadeMusic(Lvns *lvns);
void LvnsStartNextMusic(Lvns *lvns);
void LvnsLoopMusic(Lvns *lvns);
void LvnsWaitMusic(Lvns *lvns);
void LvnsWaitMusicFade(Lvns *lvns);

/* LvnsSound.c τώτώ«Στώτώ«¤«Τ©ΠτώΆμªΆ«Τ©Π«Τ©ΠτώΆμτώτώτώ«Ώτώτώ«ΣτώΆνª΅ */
void LvnsLoadSound(Lvns *lvns, const char* basename, int no);
void LvnsLoadSound2(Lvns *lvns, const char* name);
void LvnsStartSound(Lvns *lvns, int count);
void LvnsStopSound(Lvns *lvns);
void LvnsWaitSound(Lvns *lvns);
void LvnsLoopSound(Lvns *lvns);

/* LvnsBackEffect.c τώΆμΆξτώΆντώ«Τ©ΠτώΆμªφ«Τ©Πτώτώ©°«Τ©ΠτώΆμ©¬«Τ©ΠΆΒ©Μ«Τ©Πτώτώªφ«Τ©ΠτώΆμτώτώΆμτώτώτώ«Ώτώτώ«ΣτώΆνª΅ */
void LvnsSetBackEffect(Lvns *lvns, LvnsBackEffectInfo *info);
void LvnsSetNextBackEffect(Lvns *lvns, LvnsBackEffectInfo *info);
void LvnsBackEffectSetState(Lvns *lvns);
void LvnsBackEffect(Lvns *lvns);

/* LvnsFile.c «Τ©ΠτώΆμ©¬«Τ©Πτώτώª¤«Τ©Π«Τ©Π«Τ©ΠτώτώτώτώΆνΆΒτώΆμ΅ή«Τ©ΠτώΆνªΒ«Τ©ΠτώΆντώ«Τ©ΠτώΆνªΒ */
u_char *LvnsLoadData(Lvns *lvns, const char *name, size_t *size);
LvnsImage *LvnsLoadImage(Lvns *lvns, const char *name, LvnsImage *over);
void LvnsLoadScenario(Lvns *lvns, int scn, int blk);
void LvnsLoadScenarioBlock(Lvns *lvns, int blk);
u_char *LvnsGetScenarioText(Lvns *lvns, int no);
void LvnsLoadBackground(Lvns *lvns, const char *basename, int no);

/* LvnsEtc.c */
void LvnsInitSavePoint(Lvns *lvns, struct SavePoint *sp);
void LvnsSetSavePoint(Lvns *lvns, struct SavePoint *sp);

/* LvnsControl.c */
void LvnsMotion(Lvns *lvns, int x, int y);
void LvnsSelect(Lvns *lvns);
void LvnsCancel(Lvns *lvns);
void LvnsSetImageMode(Lvns *lvns);
void LvnsSkipTillSelect(Lvns *lvns);
void LvnsCursorUp(Lvns *lvns);
void LvnsCursorDown(Lvns *lvns);
void LvnsInputNumber(Lvns *lvns, int no);
void LvnsImageMode(Lvns *lvns);
void LvnsWaitKey(Lvns *lvns);
void LvnsWaitPage(Lvns *lvns);
int  LvnsWaitSelect(Lvns *lvns, int select_num);
void LvnsWaitClick(Lvns *lvns);

/* Lvns.c */
Lvns* LvnsNew(void);
void  LvnsInitialize(Lvns *lvns, int type, void *system_depend);
void  LvnsDestroy(Lvns *lvns);
void  LvnsFlip(Lvns *lvns, Bool disp_ok);
void  LvnsMain(Lvns *lvns);
void  LvnsWait(Lvns *lvns, int flip_cnt);
void  LvnsJump(Lvns *lvns, LvnsJumpParameter param);
void  LvnsSetDataPath(Lvns *lvns, const char *path);
void  LvnsSetSaveDataPath(Lvns *lvns, const char *path);
void  LvnsSetDispFunc(Lvns *lvns, void (*func)(Lvns *lvns, MenuData *data), void *data);

/* LvnsMenu.c */
void LvnsDrawMenu(Lvns *lvns);

/* LvnsHistory.c */
void LvnsAddHistory(Lvns *lvns, int no);
void LvnsHistoryMode(Lvns *lvns);

#ifdef USE_MGL
/* mgMain.c */
void mgLvnsQuit(Lvns *);

/* mgLvns.c */
void mgLvnsInit(Lvns *);
void mgLvnsDelete(Lvns *);
void mgLvnsStart(Lvns *);

/* mgLvnsCore.c */
void mgLvnsCoreSetLvns(Lvns *);

/* mgAction.c */
void MglGetEvent(Lvns *, int);

/* sizuku.c */
void SizukuInitialize(Lvns *);

/* kizuato.c */
void KizuatoInitialize(Lvns *);

/* toheart.c */
void ToHeartInitialize(Lvns *);
#endif


/* sizuku.c */
void SizukuInitialize(Lvns *);

// /* kizuato.c */
// void KizuatoInitialize(Lvns *);
// 
// /* toheart.c */
// void ToHeartInitialize(Lvns *);
#endif
