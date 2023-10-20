/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: LvnsCore.c,v 1.75 2001/08/05 10:58:09 go Exp $
 */

/*
 * LvnsCore  
 * XLVNS �����Τˤʤ� CoreWidget
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "LvnsCoreP.h"
#include "lvnsimage_sximage.h"

#define TICKSPERSEC 1000

static void ClassInitialize(void);
static void Initialize(Widget, Widget, ArgList args, Cardinal * num);
static void Destroy(Widget);
static void Realize(Widget, XtValueMask *, XSetWindowAttributes *);
static void Redisplay(Widget widget, XEvent *ev, Region region);
static void Resize(Widget widget);

static XtResource resources[] = {
    {"scenarioType", "ScenarioType", XtRInt, sizeof(int),
     XtOffsetOf(LvnsCoreRec, lvnscore.type), XtRImmediate, 
     (XtPointer)0 },
    {"rootMode", "RootMode", XtRBool, sizeof(Bool),
     XtOffsetOf(LvnsCoreRec, lvnscore.root_mode), XtRImmediate, 
     (XtPointer)False },
    {"demoMode", "DemoMode", XtRBool, sizeof(Bool),
     XtOffsetOf(LvnsCoreRec, lvnscore.demo_mode), XtRImmediate, 
     (XtPointer)False },
    {"seenMode", "SeenMode", XtRBool, sizeof(Bool),
     XtOffsetOf(LvnsCoreRec, lvnscore.seen_mode), XtRImmediate, 
     (XtPointer)False },
    {"latitudeDark", "LatitudeDark", XtRInt, sizeof(int),
         XtOffsetOf(LvnsCoreRec, lvnscore.latitude_dark), XtRImmediate, 
         (XtPointer)11 },
    {"keyClickFast", "KeyClickFast", XtRBool, sizeof(Bool),
         XtOffsetOf(LvnsCoreRec, lvnscore.key_click_fast), XtRImmediate, 
         (XtPointer)True },
    {"noShared", "NoShared", XtRBool, sizeof(Bool),
         XtOffsetOf(LvnsCoreRec, lvnscore.noshared), XtRImmediate, 
         (XtPointer)False },
#ifdef DEBUG
    {"scenarioNumber", "ScenarioNumber", XtRInt, sizeof(int),
     XtOffsetOf(LvnsCoreRec, lvnscore.scenario_number), XtRImmediate, 
     (XtPointer)-1 },
#endif
};

static void ClickLeft(Widget w, XEvent *ev, String *p, Cardinal *n);
static void ClickRight(Widget w, XEvent *ev, String *p, Cardinal *n);
static void Motion(Widget w, XEvent *ev, String *p, Cardinal *n);
static void ImageMode(Widget w, XEvent *ev, String *p, Cardinal *n);
static void SkipTillSelect(Widget widget, XEvent *ev, String *p, Cardinal *n);
static void SkipTillSelectForce(Widget widget, XEvent *ev, String *p, Cardinal *n);
static void CursorUp(Widget widget, XEvent *ev, String *p, Cardinal *n);
static void CursorDown(Widget widget, XEvent *ev, String *p, Cardinal *n);
static void Number(Widget widget, XEvent *ev, String *p, Cardinal *n);

static XtActionsRec actions[] =
{
    {"click_left", ClickLeft},
    {"click_right", ClickRight},
    {"motion",      Motion},
    {"image_mode", ImageMode},
    {"skip_till_select", SkipTillSelect},
    {"skip_till_select_force", SkipTillSelectForce},
    {"cursor_up", CursorUp},
    {"cursor_down", CursorDown},
    {"number", Number}
};

static char     translations[] = 
"<Motion>: motion()\n"
"<Btn1Down>: click_left()\n"
"<Btn3Down>: click_right()\n"
"<Key>Return: click_left()\n"
"<Key>Escape: click_right()\n"
"<Key>space: image_mode()\n"
"Meta<Key>s: skip_till_select_force()\n"
"<Key>s: skip_till_select()\n"
"<Key>Up: cursor_up()\n"
"<Key>Down: cursor_down()\n"
"<Key>1: number(1)\n"
"<Key>2: number(2)\n"
"<Key>3: number(3)\n"
"<Key>4: number(4)\n"
"<Key>5: number(5)\n"
"<Key>6: number(6)\n"
"<Key>7: number(7)\n"
"<Key>8: number(8)\n"
"<Key>9: number(9)\n"
;

#define superclass (&coreClassRec)
LvnsCoreClassRec lvnsCoreClassRec = {
    {
	 /* superclass         */ (WidgetClass) superclass,
	 /* class_name         */ "LvnsCore",
	 /* size               */ sizeof(LvnsCoreRec),
	 /* class_initialize   */ ClassInitialize,
	 /* class_part_initialize */ NULL,
	 /* Class init'ed      */ FALSE,
	 /* initialize         */ Initialize,
	 /* initialize_hook    */ NULL,
	 /* realize            */ Realize,
	 /* actions            */ actions,
	 /* num_actions        */ XtNumber(actions),
	 /* resources          */ resources,
	 /* resource_count     */ XtNumber(resources),
	 /* xrm_class          */ NULLQUARK,
	 /* compress_motion    */ TRUE,
	 /* compress_exposure  */ TRUE,
	 /* compress_enterleave */ TRUE,
	 /* visible_interest   */ FALSE,
	 /* destroy            */ Destroy,
     /* resize             */ Resize,
     /* expose             */ Redisplay,
	 /* set_values         */ NULL,
	 /* set_values_hook    */ NULL,
	 /* set_values_almost  */ XtInheritSetValuesAlmost,
	 /* get_values_hook    */ NULL,
	 /* accept_focus       */ NULL,
	 /* intrinsics version */ XtVersion,
	 /* callback offsets   */ NULL,
	 /* tm_table		    */ translations,
	 /* query_geometry	    */ NULL,
	 /* display_accelerator */ NULL,
	 /* extension	      */ NULL
    }
};

WidgetClass lvnsCoreWidgetClass = (WidgetClass) &lvnsCoreClassRec;

/* ------------------------------------------------------------------- */
static void 
ClassInitialize(void)
{
    lvnsimage_sximage_init();
}

static u_long 
getCurrentTime(void)
{
    struct timeval current;
    gettimeofday(&current, NULL);
    return current.tv_sec * 1000 + current.tv_usec / 1000;
}

static void
Initialize(Widget req, Widget new, ArgList args, Cardinal * num)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget) new;
	LCW.info     = NULL;
    LCW.sximage  = NULL;

	LvnsCoreInitMusic(lcw);
	LvnsCoreInitSound(lcw);
}

static void
Destroy(Widget widget)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget) widget;
    if (LCW.sximage)
        super_ximage_delete(LCW.sximage);
    LCW.sximage = 0;
}

#include <X11/cursorfont.h>

static void
Realize(Widget widget, XtValueMask * value_mask, XSetWindowAttributes * attr)
{
    LvnsCoreWidget lcw  = (LvnsCoreWidget) widget;
    Lvns *lvns          = LCW.info;
    Display        *dpy = XtDisplay(widget);
    Window          win;
	
    /* making new window */
    XtCreateWindow(widget, (u_int) InputOutput,
				   (Visual *) CopyFromParent, *value_mask, attr);
    win = XtWindow(widget);
	
    /* CURSOR */
    {
        Cursor cursor = XCreateFontCursor(dpy, XC_hand2);
        XDefineCursor(dpy, win, cursor);
    }
	
    /* making ximage/pixmap/workarea image */
	LCW.sximage   = super_ximage_new(XtDisplay(widget), XtWindow(widget), 
									 WIDTH, HEIGHT, COLORS, LCW.noshared);
	LCW.xoff = (COREWIDTH - WIDTH) / 2;
	LCW.yoff = (COREHEIGHT - HEIGHT) / 2;
	super_ximage_set_offset(LCW.sximage, LCW.xoff, LCW.yoff);

    super_ximage_set_palette(LCW.sximage, 0, 0, 0, 0);
    XtVaSetValues(widget, XtNbackground, LCW.sximage->pixels[0], NULL);

	LCW.wait_clock = getCurrentTime();
	LCW.wait_count = TICKSPERSEC;
}

/* 
 * ���̤κ�����
 */
static void
Redisplay(Widget widget, XEvent *ev, Region region)
{
    if (XtIsRealized(widget)) {
        LvnsCoreWidget lcw = (LvnsCoreWidget) widget;
        Lvns *lvns  = LCW.info;
        super_ximage_copy_area_win_region(LCW.sximage, region);
    }
}

/*
 * ���̤Υ������ѹ�
 */
static void
Resize(Widget widget)
{
    if (XtIsRealized(widget)) {
        LvnsCoreWidget lcw = (LvnsCoreWidget) widget;
        Lvns *lvns  = LCW.info;
		LCW.xoff = (COREWIDTH - WIDTH) / 2;
		LCW.yoff = (COREHEIGHT - HEIGHT) / 2;
		super_ximage_set_offset(LCW.sximage, LCW.xoff, LCW.yoff);
		XClearWindow(XtDisplay(widget), XtWindow(widget));
	}
}

/* ------------------------------------------------------------------- */
/* Actions */

/* �ޥ������������ư */
static void
Motion(Widget widget, XEvent *ev, String *p, Cardinal *n)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)widget;
    Lvns *lvns  = LCW.info;
    if (lvns) {
        XMotionEvent *me = (XMotionEvent *)ev;
        LvnsMotion(lvns, me->x - LCW.xoff, me->y - LCW.yoff);
    }
}

/* ���������Ԥ� */
static void
ClickLeft(Widget widget, XEvent *ev, String *p, Cardinal *n)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)widget;
    Lvns *lvns  = LCW.info;
    if (lvns)
        LvnsSelect(lvns);
}

/* ������å� */
static void
ClickRight(Widget widget, XEvent *ev, String *p, Cardinal *n)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)widget;
    Lvns *lvns  = LCW.info;
    if (lvns)
        LvnsCancel(lvns);
}

/* ʸ���õ�⡼�� */
static void
ImageMode(Widget widget, XEvent *ev, String *p, Cardinal *n)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)widget;
    Lvns *lvns  = LCW.info;
    if (lvns)
        LvnsSetImageMode(lvns);
}

/**
 * ��˥塼�������ޤǤȤФ���
 */
static void
SkipTillSelect(Widget widget, XEvent *ev, String *p, Cardinal *n)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)widget;
    Lvns *lvns  = LCW.info;
    if (lvns)
        LvnsSkipTillSelect(lvns);
}

/**
 * ����/̤�ɤ�̵�뤷�������ޤǤȤФ���
 */
static void
SkipTillSelectForce(Widget widget, XEvent *ev, String *p, Cardinal *n)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)widget;
    Lvns *lvns = LCW.info;
    if (lvns) {
        lvns->force_skip = True;
        lvns->skip       = True;
		fprintf(stderr, "force skip\n");
    }
}

static void
CursorUp(Widget widget, XEvent *ev, String *p, Cardinal *n)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)widget;
    Lvns *lvns = LCW.info;
    if (lvns)
        LvnsCursorUp(lvns);
}

static void
CursorDown(Widget widget, XEvent *ev, String *p, Cardinal *n)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)widget;
    Lvns *lvns = LCW.info;
    if (lvns)
        LvnsCursorDown(lvns);
}

static void
Number(Widget widget, XEvent *ev, String *p, Cardinal *n)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)widget;
    Lvns *lvns = LCW.info;
    if (lvns)
        LvnsInputNumber(lvns, p[0][0]-'0');
}

/* ------------------------------------------------------------------- */
/* Privat Functions */


/* ---------------------------------------------------------------- */
/*
 * ���� VRAM �����
 */

/** 
 * �����̤��ե�å��夷�Ƽºݤβ��̤�ȿ�Ǥ�����
 */
static void
FlushWindow(Lvns *lvns)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    super_ximage_set_pixmap_area(LCW.sximage, 0, 0, WIDTH, HEIGHT);
	super_ximage_copy_area_win(LCW.sximage, 0, 0, WIDTH, HEIGHT);
    XSync(XtDisplay((Widget)lcw), False);
}

/**
 * ���̤���ʬŪ�˥�ե�å��夷�Ƽºݤβ��̤�ȿ�Ǥ�����
 */
static void
FlushWindowArea(Lvns *lvns, int x, int y, int w, int h)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    super_ximage_set_pixmap_area(LCW.sximage, x, y, w, h);
	super_ximage_copy_area_win(LCW.sximage, x, y, w, h);
}

/**
 * ���Τ�����(�ե�å���̵��) 
 */
static void
DrawWindow(Lvns *lvns) 
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    lvnsimage_copy_to_sximage(lvns->vram, LCW.sximage, 0, 0);
}

/**
 * ��ʬ����(�ե�å���̵��) 
 */
static void
DrawWindowArea(Lvns *lvns, int x, int y, int w, int h, int x2, int y2)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    lvnsimage_copy_area_to_sximage(lvns->vram, LCW.sximage, x, y, w, h, 
								   x2, y2);
}

/**
 * �����̲��̾õ�(�ե�å���̵��)
 */
static void
ClearWindow(Lvns *lvns)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    super_ximage_clear_area(LCW.sximage, 0, 0, WIDTH, HEIGHT,
                            LCW.sximage->pixels[lvns->vram->black]);
}

/**
 * ��ʬ���̾õ�(�ե�å���̵��)
 */
static void
ClearWindowArea(Lvns *lvns, int x, int y, int w, int h)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    super_ximage_clear_area(LCW.sximage, x, y, w, h,
                            LCW.sximage->pixels[lvns->vram->black]);
}

/**
 * ����Υ���ǥå����˥��顼�������Ƥ�
 */
static void
SetPaletteIndex(Lvns *lvns, int index, int r, int g, int b)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    super_ximage_set_palette(LCW.sximage, index, r, g, b);
}

/**
 * �ѥ�å�����(�̾�)
 * ���������ꤹ���
 */
static void
SetPalette(Lvns *lvns)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    lvnsimage_set_pal_to_sximage(lvns->vram, LCW.sximage);
}

/**
 * �ѥ�å�����(multiple)
 */
static void
SetPaletteMulti(Lvns *lvns, int par16)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    lvnsimage_set_pal_to_sximage_multi(lvns->vram, LCW.sximage, par16);
}

/**
 * �ѥ�å�����(screen)
 */
static void
SetPaletteScreen(Lvns *lvns, int par16)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    lvnsimage_set_pal_to_sximage_screen(lvns->vram, LCW.sximage, par16);
}

/* 
 * �ޥ����ѥ�����ǥ�����ɥ����Ф��������Ԥ�(�ե�å���̵��)
 */
static void
DrawWindowMask(Lvns *lvns, int x, int y, int state)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend; 
	lvnsimage_copy_mask_unit_to_sximage(lvns->vram, LCW.sximage, 
										x, y, x, y, state);
}

/* 
 * ����ѥ�����ǥ�����ɥ����Ф��������Ԥ�(�ե�å���̵��)
 */
static void
DrawWindowSquareMask(Lvns *lvns, int x, int y, int state)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    lvnsimage_copy_squaremask_unit_to_sximage(lvns->vram, LCW.sximage, 
											  x, y, x, y,
											  state);
}

/*
 * ɽ������Ƥ�����̤Υ��ե��åȤ򤺤餹(��ư��)
 */
static void
SetDispOffset(Lvns *lvns, int xoff, int yoff, int maxoff)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
#if 0
    super_ximage_clear_area_win(LCW.sximage, -maxoff, -maxoff, 
                                maxoff*2, HEIGHT+maxoff*2,
                                LCW.sximage->pixels[lvns->vram->black]);
    super_ximage_clear_area_win(LCW.sximage, -maxoff, -maxoff, 
                                WIDTH+maxoff*2, maxoff*2,
                                LCW.sximage->pixels[lvns->vram->black]);
    super_ximage_clear_area_win(LCW.sximage, +WIDTH-maxoff, 0, 
                                maxoff*2, HEIGHT+maxoff,
                                LCW.sximage->pixels[0]);
    super_ximage_clear_area_win(LCW.sximage, 0, +HEIGHT-maxoff, 
                                WIDTH+maxoff, maxoff*2,
                                LCW.sximage->pixels[lvns->vram->black]);
#endif
    super_ximage_copy_area_win2(LCW.sximage, -maxoff*2, -maxoff*2, WIDTH+maxoff*4, HEIGHT+maxoff*4, -maxoff*2 + xoff, -maxoff*2 + yoff);
}

/* 
 * ʸ���ѥ�����ɽ�� (��/�� ��)
 */
static void
PutPattern(Lvns *lvns, int x, int y, int index, u_char *data)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    super_ximage_put_pattern24_2(LCW.sximage, 
                                 x, y, LCW.sximage->pixels[index], data);
}

/* 
 * ʸ���ѥ�����ɽ�� (ToHeart ��)
 */
static void
PutPattern2(Lvns *lvns, int x, int y, int index, u_char *data)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    super_ximage_put_pattern24(LCW.sximage, 
                               x, y, LCW.sximage->pixels[index], data);
}

/* 
 * �����ޤ�ꥻ�åȤ���
 * @param no �������ֹ�
 */
static void
ResetTimer(Lvns *lvns, int no)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    gettimeofday(&LCW.timer[no], NULL);
}

/*
 * �������ͤ��������
 * @param no �������ֹ�
 * @return �������� (msecñ��)
 */
static long 
GetTimer(Lvns *lvns, int no)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    struct timeval current;
    gettimeofday(&current, NULL);
    return (current.tv_sec - LCW.timer[no].tv_sec) * 1000 +
        (current.tv_usec - LCW.timer[no].tv_usec) / 1000;
}

/**
 * ��������褹��
 * @param x,y,w,h ���֤ȥ�����
 * @param idx ���Υ���ǥå�����
 */
static void
DrawBox(Lvns *lvns, int x, int y, int w, int h, int idx)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
    sximage_box(LCW.sximage, x, y, w, h, idx);
}

#include <sys/types.h>
#include <sys/stat.h>

/** 
 * ���ꤷ���ե�����ι������դ��������
 */
static time_t
GetFileTime(Lvns *lvns, const char *path)
{
    struct stat sb;
    if (stat(path, &sb) < 0) {
        perror(path);
        return 0;
    }
    return sb.st_mtime;
}

/*
 * ���襷�󥯥��Ԥ�����
 */
static void
Flip(Lvns *lvns)
{
    LvnsCoreWidget lcw = (LvnsCoreWidget)lvns->system_depend;
	XtAppContext app = XtWidgetToApplicationContext((Widget)lcw);

    XSync(XtDisplay((Widget)lcw), False);

	// X �Υ��٥�Ƚ���
	while (!XtIsRealized((Widget)lcw) || XtAppPending(app)) {
		XEvent event;
		XtAppNextEvent(app, &event);
		XtDispatchEvent(&event);
	}

	// ��λ����
	if (XtAppGetExitFlag(app)) {
		LvnsJump(lvns, LVNS_JUMP_END);
	} 

	// �����ߥ���������
	{
		int wait, sa;
		wait = LCW.wait_count / 60;
		if (wait > 0) {
			u_long tw;
			sa = getCurrentTime() - LCW.wait_clock;
			if (sa > 0) wait -= sa;
			if (wait > 0) usleep(wait);
			tw = getCurrentTime();
			sa = tw - LCW.wait_clock;
			if (sa > 0) 
				LCW.wait_count -= sa * 60;
			else 
				LCW.wait_count = 0;
			LCW.wait_clock = tw;
		}
		LCW.wait_count += TICKSPERSEC;
		if (LCW.wait_count < 0) {
			LCW.wait_count = 0;
		}
	}			
}

/* ------------------------------------------------------------------- */
/* Public Functions */

int
LvnsCoreGetBestWidth(LvnsCoreWidget lcw)
{
    Lvns *lvns = LCW.info;
    return WIDTH;
}

int
LvnsCoreGetBestHeight(LvnsCoreWidget lcw)
{
    Lvns *lvns = LCW.info;
    return HEIGHT;
}

/*
 * �������饳�ޥ�ɤ�¹�
 */
void
LvnsCoreExecCommand(LvnsCoreWidget lcw, const char *command)
{
    Lvns *lvns = LCW.info;
    if (lvns->system_state->execCommand)
		lvns->system_state->execCommand(lvns, command);
}

/*
 * ���ޥ�ɾ���μ���
 */
LvnsCommandInfo *
LvnsCoreGetCommandList(LvnsCoreWidget lcw)
{
    Lvns *lvns = LCW.info;
    if (lvns->system_state->getCommandList)
		return lvns->system_state->getCommandList(lvns);
    else
		return NULL;
}

/*
 * LVNS �������³
 */ 
void
LvnsCoreSetLvns(LvnsCoreWidget lcw, Lvns *lvns)
{
	LCW.info = lvns;

	/* SE ��Ϣ��������� */
	lvns->sound = &LCW.sound;

	/* BGM��Ϣ����� */
	lvns->music = &LCW.music;

	/* �꥽������ȿ�� */
	lvns->demo_mode        = LCW.demo_mode;
	lvns->seen_mode        = LCW.seen_mode;
	lvns->latitude_dark    = LCW.latitude_dark;
	lvns->key_click_fast   = LCW.key_click_fast;

#ifdef DEBUG
	lvns->start_scn_num     = LCW.scenario_number;
#endif

	/* �����ƥ��¸�ؿ��������� */
	lvns->flushWindow          = FlushWindow;
	lvns->flushWindowArea      = FlushWindowArea;
	lvns->drawWindow           = DrawWindow;
	lvns->drawWindowArea       = DrawWindowArea;
	lvns->clearWindow          = ClearWindow;
	lvns->clearWindowArea      = ClearWindowArea;
	lvns->setPaletteIndex      = SetPaletteIndex;
	lvns->setPalette           = SetPalette;
	lvns->setPaletteMulti      = SetPaletteMulti;
	lvns->setPaletteScreen     = SetPaletteScreen;
	lvns->drawWindowMask       = DrawWindowMask;
	lvns->drawWindowSquareMask = DrawWindowSquareMask;
	lvns->setDispOffset        = SetDispOffset;
	lvns->putPattern           = PutPattern;
	lvns->putPattern2          = PutPattern2;
	lvns->resetTimer           = ResetTimer;
	lvns->getTimer             = GetTimer;
	lvns->drawBox              = DrawBox;
	lvns->getFileTime          = GetFileTime;
	lvns->flip                 = Flip;

	LvnsInitialize(lvns, LCW.type, lcw);
	XtResizeWidget((Widget)lcw, WIDTH, HEIGHT, 0);
}



