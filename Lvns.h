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

/* EUC ��ԏ�Џ�ԏ�������������폫ԏ�������ޏ�ԏ�Џ�ԏ�Џ�ԏ�������ޏ�ԏ�����������ԏ������쏢쏫ԏ�����������ԏ�Џ�ԏ�Џ�ԏ��������� JIS ��ԏ�Џ�ԏ�������������폫ԏ�������ޏ�ԏ����������������������� */
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

/* �����������������������ԏ�ԏ������쏪�����쏫���������������������������������쏩���ԏ������폪����������ԏ�������������쏪���ԏ������쏪� MGL2 �����������쏢ԏ������쏢����������ԏ�Џ�ԏ�Џ�ԏ������������������ԏ��������� */
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
 * MGL2 �����������쏢ԏ������쏪�����쏫����������ԏ��������ԏ������쏪� X �����������쏢ԏ�������������������쏫ԏ�Џ�ԏ�����������ԏ������������̏�ԏ��������ҏ�����������������������ԏ�ԏ�����������ԏ�Џ�ԏ�Џ�ԏ������쏪�����쏫���������������������ԏ�ԏ��������������������폢�ԏ������쏪�����폪���ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ������������������ԏ��
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
	int scn;  /* ��ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ��������������������쏪я�ԏ������폪� */
	int blk;  /* ��ԏ������쏪я�ԏ��������ď�ԏ������쏢쏫ԏ��������������������쏪я�ԏ������폪� */
	int no;   /* ��ԏ������폢�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ��������������������쏪я�ԏ������폪� */
} LvnsHistoryData;        /* ��ԏ�������������쏢�����쏫���ԏ�������������������폫ԏ������������쏢�������� */

typedef struct {
    char *command;
    int enable;
} LvnsCommandInfo;   /* ��ԏ�Џ�����������������������������������������폪�����쏢�������� */

typedef struct {

    int width;                   /* ����폢������������������������������쏢�ԏ������������쏪�����쏪�������������ԏ������쏪���ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ��������� */
    int height;             
    int colors;                  /* �����ޏ�ԏ������쏢ď��Ώ�ԏ������쏪������������쏢����������������������������������������       */ 

    const char *leafpack_name;   /* ��ԏ������쏢������������폫ԏ�������������쏢����������ԏ�����������ԏ������쏢쏫ԏ�����������ԏ������쏩���ԏ�����������ԏ�Џ�ԏ�Џ�ԏ������������쏪�����쏫�����쏫�����쏪�   */
    const char *scnpack_name;    /* ��ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ�������������쏢����������ԏ�����������ԏ������쏢쏫ԏ�����������ԏ������쏩���ԏ�����������ԏ�Џ�ԏ�Џ�ԏ������������쏪�����쏫�����쏫�����쏪� */
    const char *scn_name;        /* ��ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ�����������ԏ������쏩���ԏ�����������ԏ�Џ�ԏ�Џ�ԏ����������ԏ������쏫������������폫ԏ�Џ�ԏ������쏪�����쏫�����쏫�����쏪� */
    const char *fonttable_name;  /* ��ԏ�Џ�ԏ�������������폫ԏ�������ޏ�ԏ�Џ�ԏ�Џ�ԏ��������Џ�ԏ�Џ�ԏ�������������폫ԏ�����������ԏ������폢������������폫ԏ������쏪я�ԏ������������쏪�����쏫�����쏫�����쏪� */

	MenuData *main_menu;

	void (*start)(struct Lvns *lvns);    /* ����폪�����������쏫����������ԏ������폪�       */
	void (*jingle)(struct Lvns *lvns);   /* ��ԏ��������ӏ�ԏ��������Џ�ԏ�Џ�����ԏ��������     */
	void (*opening)(struct Lvns *lvns);  /* ��ԏ��������������������폫ԏ�Џ��Ώ�ԏ�����������ԏ��������Џ�ԏ�Џ��� */
	void (*title)(struct Lvns *lvns);    /* ��ԏ�����������ԏ�Џ�ԏ�Џ�ԏ�����������ԏ��������     */
	void (*main)(struct Lvns *lvns);     /* ��ԏ���������ԏ�Џ�ԏ�Џ�ԏ���������       */

    void (*mergeCharacter)(struct Lvns *lvns);   /* ��ԏ�Џ�ԏ��������쏫ԏ�������ޏ�ԏ�����������ԏ�����������ԏ����������������������� */
    void (*clearScreen)(struct Lvns *lvns);      /* ��ԏ������쏫�����쏢�����쏫� VRAM ��ԏ������쏪�����쏫�����쏢쏫ԏ��������� */
    void (*loadBG)(struct Lvns *lvns, int no);   /* ����쏢����������ԏ������쏪�����폢������ޏ�ԏ������폪�ԏ�����������ԏ������폪� */

    LvnsCommandInfo* (*getCommandList)(struct Lvns *lvns);           /* ��ԏ�Џ��������������������쏢������������������������������������쏢ď��� */
    void (*execCommand)(struct Lvns *lvns, const char *command);     /* ��ԏ�Џ��������������������쏢���������������������     */
    void (*drawChar)(struct Lvns *lvns, int x, int y, int, int);     /* ����쏪�������ӏ�ԏ�������������������폪�������������     */

	void (*dispHistory)(struct Lvns *lvns, int pos);

    int cursor_key;   /* ��ԏ������������������폢������������쏪�����쏪������������쏪ԏ�ԏ������쏢�ԏ������쏪���ԏ�Џ�ԏ�������������폫ԏ������폪���ԏ�������� */
    int cursor_page;  /* ��ԏ�����������ԏ������쏫������������폫ԏ��������������������쏪ԏ�ԏ������쏢�ԏ������쏪���ԏ�Џ�ԏ�������������폫ԏ������폪���ԏ�������� */

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
	/* ��ԏ������쏢������������폫ԏ�����������ԏ�����������ԏ�������ޏ�ԏ��������������������폫ԏ��������� */

    LvnsSystemState *system_state;     /* ��ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ������폢�ԏ����������ԏ�Џ�ԏ�Џ�ԏ������폢������������폫ԏ��������� */

    char *data_path;        /* ��ԏ������쏢������������폫ԏ��������������������������������������쏫�����폪������������쏫���ԏ�����������ԏ�Џ�ԏ�� */
    char *savedata_path;    /* ��ԏ������쏢������������폫ԏ�������������쏪�����쏫��������������ӏ�������쏪�����쏢�����쏪� */
 
    LeafPack *leafpack;     /* ��ԏ������쏢������������폫ԏ�������������쏢����������ԏ�����������ԏ������쏢쏫ԏ�����������ԏ������쏩���ԏ�����������ԏ�Џ�ԏ�Џ�ԏ�������� */
    LeafPack *scnpack;      /* ��ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ�������������쏢����������ԏ�����������ԏ������쏢쏫ԏ�����������ԏ������쏩���ԏ�����������ԏ�Џ�ԏ�Џ�ԏ�������� */
    u_char *leaf_font_data; /* Leaf FONT ��ԏ������쏪���ԏ������쏢������������폫ԏ���������   */

    u_char *leaf_to_euc;    /* ��ԏ�Џ�ԏ�������������폫ԏ�������ޏ�ԏ�Џ�ԏ�Џ�ԏ��������Џ�ԏ�Џ�ԏ�������������폫ԏ�����������ԏ������폢������������폫ԏ������쏪я�ԏ�������� Leaf -> EUC  */
    u_short *jis_to_leaf;   /* ��ԏ�Џ�ԏ�������������폫ԏ�������ޏ�ԏ�Џ�ԏ�Џ�ԏ��������Џ�ԏ�Џ�ԏ�������������폫ԏ�����������ԏ������폢������������폫ԏ������쏪я�ԏ�������� JIS Pack -> Leaf */

    /* ------------------------------------------------------ */
    /* ��ԏ�����������ԏ������폪������������폫ԏ�Џ�ԏ�� */

    Bool force_skip;      /* ������ӏ�ԏ�Џ�ԏ�������������쏪������������폪���������ԏ������쏢ԏ����������ԏ�Џ�ԏ�Џ�ԏ������쏢쏫ԏ�����������ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ��           */
    Bool key_click_fast;  /* ��ԏ�������������폫ԏ�����������ԏ�����������ԏ������쏢쏫ԏ�����������ԏ������쏢�����쏪������������폪���������ԏ��������ҏ�ԏ��������������������������������폪���������������ԏ��������я�ԏ��������� */
	Bool fast_when_seen;  /* ����쏫������������폢�����������쏪������������폪���������ԏ������쏪�����쏢��������ԏ�������������������폪���������������ԏ�Џ�ԏ�Џ�ԏ��������           */
    Bool enable_effect;       /* ����������������ԏ������쏪���ԏ��������ҏ�ԏ�����������ԏ������쏩� */
    Bool enable_effect_back;  /* ����쏢����������ԏ������쏪�����������������ԏ������쏪���ԏ��������ҏ�ԏ�����������ԏ������쏩� */

    Bool seen_mode;       /* ����쏢�����쏫�����쏪������������쏫������������폢������ޏ�ԏ�����������ԏ������폪�ԏ������쏪���ԏ�Џ�ԏ�Џ�ԏ�������������������폫ԏ��������(for DEBUG) */
    Bool demo_mode;       /* ��ԏ��������������������폫ԏ�����������ԏ������쏢ԏ����������ԏ�������������������폫ԏ�������� */

    /* ------------------------------------------------------ */
    /* ��ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ������폢�ԏ����������ԏ�����������ԏ�������ޏ�ԏ��������������������폫ԏ��������� */

    Bool image_dark;    /* �����������폪������������폪�ԏ������쏪���������쏢폫ԏ�Џ�ԏ��                      */

    int  char_wait_time;         /* ����쏪�������ӏ�ԏ�������������������폪������������������������쏪ԏ�ԏ������쏢�ԏ�������������쏫�����쏪�     */
    
    Bool fast_text;    /* ��ԏ������폢�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�����������ԏ�Џ�ԏ�Џ�ԏ������쏢��������������ԏ�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ������쏩���ԏ�������ޏ�ԏ�Џ���       */
    Bool fast_disp;    /* ����������폪���������������ԏ�����������ԏ������쏩���ԏ�Џ��̏�ԏ�����������ԏ�����������ԏ�Џ�ԏ�Џ�ԏ������쏢쏫ԏ�����������ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ������쏩���ԏ�������ޏ�ԏ�Џ��� */

	jmp_buf jump;

    /* ------------------------------------------------------ */
    /* �����������폪������������폪�ԏ������쏪�����쏫�������������������쏪� ��ԏ������쏢������������폫ԏ�����������ԏ������쏪�����쏪�����쏫�������������������쏢�������� */
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
        int scn;         /* ��ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ��������������������쏪я�ԏ������폪�����쏪�����쏫�������������������쏢�������� */
        int blk;         /* ��ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ�����������ԏ������쏪я�ԏ��������ď�ԏ������쏢쏫ԏ�������������쏪�����쏫�������������������쏢��������       */
        long scn_offset; /* ��ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ�����������ԏ������쏢������������폫ԏ�������������쏪�����쏫�������������������쏢����������ԏ�Џ�ԏ�������������폫ԏ������폪���ԏ�������� */
        int current_music;
    } savepoint;
    struct SavePoint selectpoint;

    /* ---------------------------------------------------------- */
    /* ����쏢�������������������������쏫�������������������쏪� */

	/* ��ԏ�Џ�����������������������������������������폪�����쏫�������������������쏪� */
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
    /* ��ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ�����������ԏ��������������������폫ԏ�Џ�ԏ�� */

    int start_scn_num;    /* ��ԏ�������������폢�����ԏ�����������ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ��������������������쏪я�ԏ������폪�(for DEBUG) */

    u_char *scn_data;     /* ��ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ�����������ԏ������쏢������������폫ԏ���������             */
    u_char *scn_cur_head; /* ��ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ�����������ԏ������쏢������������폫ԏ��������������������������폢�ԏ��         */
    const  u_char *scn_cur;      /* ��ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ�����������ԏ������쏢������������폫ԏ�����������ԏ�Џ�ԏ�������������폫ԏ������폪���ԏ��������     */
    u_char *scn_text;     /* ��ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ�����������ԏ������폢�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ���������           */

    size_t  scn_length;
	Bool    seen;         /* ����쏫������������폢������ޏ�ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ�����������ԏ������쏩���ԏ�������ޏ�ԏ�Џ���         */

    int scn_current;                       /* �����������폪������������폪�ԏ������쏪���ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ��������������������쏪я�ԏ������폪�     */
    int blk_current;                       /* �����������폪������������폪�ԏ������쏪���ԏ�Џ�ԏ�Џ�ԏ������쏪���ԏ�����������ԏ�����������ԏ������쏪я�ԏ��������ď�ԏ������쏢쏫ԏ��������� */


    /* ------------------------------------------------------ */
    /* ��ԏ������폢�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�����������ԏ�����������ԏ�Џ�ԏ�Џ�ԏ������������폪�����������쏢�������ҏ�ԏ������쏪� */

#define TEXT_WIDTH  26
#define TEXT_HEIGHT 16

    struct {
        /* ��ԏ������폢�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�����������ԏ�����������ԏ�Џ�ԏ�Џ�ԏ������������쏢����������ԏ������쏫�����쏢�����쏫�VRAM */
		struct TextVramLine {
			int offset;
			struct TextVram {
				int code;       /* ����쏪�������ӏ�ԏ�����������ԏ������쏪���ԏ�Џ�ԏ�������������폫ԏ��������       */
				int attribute;  /* ����쏪�������ӏ�ԏ�����������ԏ������쏪�������������������         */
			} column[TEXT_WIDTH];
		} row[TEXT_HEIGHT];
        int cur_x;
        int cur_y;
        int o_cur_x;
        int o_cur_y;
    } tvram[2];
    int current_tvram;

    /* ��ԏ�Џ�ԏ�������������폫ԏ������폪���ԏ����������ԏ������쏪�����쏫�������������������쏪� */
    int cursor_state;
    
    /* ��ԏ������폢�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�����������ԏ�Џ�ԏ�������������폫ԏ������폪���ԏ����������ԏ������쏪�����쏫�������������������쏪� */
    int text_cursor_state;     

	/* ����������폪������������������������������� */
    int text_attr;

	/* ����쏪�������ӏ�ԏ�����������ԏ�Џ�ԏ������폪���������ԏ�Џ�ԏ�����������ԏ�������돫ԏ�����������ԏ������폢�ԏ�Џ�ԏ�Џ�ԏ����������ԏ�Џ�ԏ��? */
	Bool text_written;

	/* ��ԏ������폢�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�������������������폪���������������ԏ�������������������폫ԏ�������ޏ�ԏ�Џ�ԏ�Џ�ԏ�������ޏ�ԏ������쏩���ԏ�Џ�ԏ�� */
	Bool text_mode;

    /* ------------------------------------------------------- */
    /* ��ԏ������������쏢������������������쏢�ԏ����������ԏ������쏪� */

    LvnsImage *background; /* ����쏢����������ԏ������쏪���ԏ������쏢������������폫ԏ�������������쏪�����쏫ҏ�ԏ�������������쏢��������        */
    LvnsImage *vram;       /* ����쏢����������ԏ������쏪���ԏ�����������������������������������������������������쏢����������ԏ������쏫�����쏢�����쏫� VRAM */

    int latitude;          /* �����������폪������������폪�ԏ������쏪���ԏ������������쏢����������ԏ������쏪�����쏪����������ԏ����������ԏ�Џ�ԏ�� 0-255 */
    int latitude_dark;     /* ��ԏ������������쏪�����쏪���ԏ�Џ�ԏ�Џ�������쏢폫ԏ�Џ�ԏ�Џ�ԏ�����������ԏ������쏪���ԏ�Џ�������쏢�����쏫���ԏ������쏪���ԏ����������������������� */

    int sepia_mode_next;   /* ��������������ԏ������쏪���ԏ������������쏪�����쏪������������쏫���ԏ�������������������������ԏ�����������ԏ������쏢ԏ�Џ�ԏ�Џ�ԏ������쏪ԏ�ԏ�Џ�ԏ�������������쏫�����������폪������������� */
    int sepia_mode;        /* ��ԏ�Џ�ԏ�Џ�ԏ������쏪ԏ�ԏ�Џ�ԏ�������������쏫�����������폪������������� */

    LvnsBackEffectInfo *effect_back;
    LvnsBackEffectInfo *effect_back_next;
    int effect_back_state;                 /* ����쏢����������ԏ������쏪���ԏ�����������ԏ������쏩���ԏ�Џ��̏�ԏ�����������ԏ�������������쏢����������ԏ�Џ�ԏ�Џ�ԏ������폢������������폫ԏ��������� */
#ifdef FONT_PLUS
    u_char*          font_file;
    LvnsFont	     *font;
#endif /* FONT_PLUS */

	void (*disp_func)(struct Lvns *lvns, MenuData *data); /* ������������ޏ�ԏ������쏢�����������폪�����������������쏫���������������������� */
	void *disp_func_param;

    /* -------------------------------------------------------- */
    /* ��ԏ�Џ�ԏ�Џ�ԏ������쏫������������폢��������������ӏ�ԏ������쏪� */
	Music *music;

    /* ��ԏ�������������������폫ԏ�Џ�������폪�����������쏢������������쏢�������� */
    int current_music;                            /* �����������폪������������폪�������������쏢�����쏩�����쏢�����폪���ԏ������쏪���ԏ�Џ�ԏ�Џ�ԏ������쏫�     */
    Bool loop_music;                              /* ��ԏ�������������������폫ԏ�Џ��Ώ�ԏ�Џ�ԏ�Џ�ԏ�������ޏ�ԏ������쏩���ԏ�Џ�ԏ��       */
    int next_music;                               /* ��������������ԏ������쏪���ԏ�Џ�ԏ�������������폫ԏ��������Џ�ԏ�Џ�ԏ�Џ�ԏ�������ޏ�ԏ������쏪���ԏ�Џ�ԏ�Џ�ԏ������쏫� */
    Bool loop_next_music;                         /* ��ԏ�������������������폫ԏ�Џ��Ώ�ԏ�Џ�ԏ�Џ�ԏ�������ޏ�ԏ������쏩���ԏ�Џ�ԏ��       */

	/* ��ԏ������쏫���ԏ�����������ԏ��������������������폫ԏ������������폪�����������쏢������������쏢�������� */
    int current_music_volume;                     /* �����������폪������������폪�ԏ������쏪���ԏ������쏫���ԏ�����������ԏ��������������������폫ԏ�������������������폪��������������� */
    int music_fade_mode;                          /* ��ԏ������쏩���ԏ�Џ��������������폫ԏ�������ޏ�ԏ�������������������폫ԏ��������       */
    int music_fade_flag;                          /* ��ԏ������쏩���ԏ�Џ��������������폫ԏ�������ޏ�ԏ������쏪�����쏪����������������������       */

    /* -------------------------------------------------------- */
    /* ��ԏ�Џ�ԏ�Џ�ԏ������쏩���ԏ��������Џ�ԏ�������������������폢��������������ӏ�ԏ������쏪� */
	Sound *sound;

    /* ��ԏ�������������������폫ԏ�Џ�������폪�����������쏢������������쏢�������� */
    int sound_loop;
    int sound_count;
    int interval_count;

    /* -------------------------------------------------------- */
	/* ��ԏ������쏪ҏ�ԏ�Џ�ԏ�Џ�ԏ�����������ԏ��������� */
	LvnsHistoryData *history;
	int history_size;
	int history_pos;

#ifdef USE_MGL
    /* -------------------------------------------------------- */
    /* mglvns ��ԏ������쏪���ԏ������폪�ԏ������쏢ԏ�����������ԏ������쏩�����쏫�������������������쏪�����쏢�����쏢� */
    int version;
#endif

    /* -------------------------------------------------------- */
	/* ��ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ������폢�ԏ����������������쏢���������������������������� */

    void *system_depend;                /* ����폢�������������������ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ������폢�ԏ����������������쏢�������������������쏫�����폪�����쏪�������� */

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

/* LvnsDisp.c ����������폪���������������ԏ������쏪� */
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

/* LvnsText.c ��ԏ������폢�ԏ�Џ�ԏ�Џ�ԏ�Џ�ԏ�������������폪�����������쏢�������ҏ�ԏ������쏪� */
void LvnsLocate(Lvns *lvns, int x, int y);
void LvnsPutChar(Lvns *lvns, int c, int attr);
void LvnsPutCharNormal(Lvns *lvns, int c, int attr);
void LvnsPuts(Lvns *lvns, const u_char *str, int attr);
void LvnsClearText(Lvns *lvns);
void LvnsSetTextOffset(Lvns *lvns, int offset);
void LvnsNewLineText(Lvns *lvns);
void LvnsDrawString(Lvns *lvns, int x, int y, const u_char *str, int attr);
void LvnsDrawStringCenter(Lvns *lvns, int y, const u_char *str, int attr);

/* LvnsMusic.c ��ԏ�Џ�ԏ�Џ�ԏ������쏫��������������������������폪� */
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

/* LvnsSound.c ����������������ԏ������쏪���ԏ�Џ�ԏ���������������������������폪� */
void LvnsLoadSound(Lvns *lvns, const char* basename, int no);
void LvnsLoadSound2(Lvns *lvns, const char* name);
void LvnsStartSound(Lvns *lvns, int count);
void LvnsStopSound(Lvns *lvns);
void LvnsWaitSound(Lvns *lvns);
void LvnsLoopSound(Lvns *lvns);

/* LvnsBackEffect.c ����쏢����������ԏ������쏪���ԏ�����������ԏ������쏩���ԏ�Џ��̏�ԏ�����������ԏ����������������������������������폪� */
void LvnsSetBackEffect(Lvns *lvns, LvnsBackEffectInfo *info);
void LvnsSetNextBackEffect(Lvns *lvns, LvnsBackEffectInfo *info);
void LvnsBackEffectSetState(Lvns *lvns);
void LvnsBackEffect(Lvns *lvns);

/* LvnsFile.c ��ԏ������쏩���ԏ�����������ԏ�Џ�ԏ�Џ�ԏ������������폢������ޏ�ԏ������폪�ԏ�����������ԏ������폪� */
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
