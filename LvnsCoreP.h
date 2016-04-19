/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: LvnsCoreP.h,v 1.54 2001/07/18 20:39:15 go Exp $
 *
 */

#ifndef __LvnsCoreP_h
#define __LvnsCoreP_h

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/CoreP.h>

#include <sys/time.h>

#include "LvnsCore.h"
#include "Lvns.h"

#include "sximage.h"
#include "cdinfo.h"

typedef struct {
	XtPointer extension;
} LvnsCoreClassPart;

typedef struct _LayerCoreClassRec {
    CoreClassPart   core_class;
    LvnsCoreClassPart lvnscore_class;
} LvnsCoreClassRec;

extern LvnsCoreClassRec lvnsCoreClassRec;

#ifndef CDDEVICE
#define CDDEVICE "/dev/cd0a"
#endif
#ifndef AUDIODEVICE
#define AUDIODEVICE "/dev/audio"
#endif

typedef struct _ImageCorePart {

    Lvns *info;         /* ���ܾ��� */

    /* ----------------------------------------------------------- */
    SuperXImage *sximage;     /* ���᡼��ɽ������� ximage + pixmap */
    int xoff, yoff;

    /* ---------------------------------------------------------- */
    /* CD-DA �����ѥǡ��� */
    CDInfo *cdinfo;
	Music music;

    /* -------------------------------------------------------- */
    /* ������ɺ����� */
    XtInputId sound_ctl_id;     /* X���Ͻ�����      */
    int sound_ctl_fd;           /* �ץ������̿��� */
	Sound sound;

    /* -------------------------------------------------------- */
    /* �꥽���������� */
    int  type;                    /* ���ʥꥪ���󥸥���� */
    Bool            root_mode;    /* �롼�ȥ�����ɥ���ɽ������ */
    Bool            seen_mode;    /* ��ʸ���ɤȤߤʤ��⡼��(for DEBUG) */
    Bool            demo_mode;    /* �����ȥǥ�⡼�� */
    int         latitude_dark;    /* ���̤��Ť����ε��٤λ���          */
    Bool        key_click_fast;   /* ʸ��ɽ������ */
#ifdef DEBUG
	int         scenario_number;  /* ���ʥꥪ�ֹ������ */
#endif
    /* -------------------------------------------------------- */
    /* �����٥������� */
    struct timeval timer[10];
    Bool noshared;

    /* -------------------------------------------------------- */
	/* Flip ������ */
	long wait_clock;
	long wait_count;

} LvnsCorePart;

typedef struct _LvnsCoreRec {
    CorePart        core;
    LvnsCorePart    lvnscore;
} LvnsCoreRec;

/* convinient defines */

#define COREWIDTH  (lcw->core.width)
#define COREHEIGHT (lcw->core.height)
#define LCW        (lcw->lvnscore)

/* semi public functions */
void LvnsCoreInitMusic(LvnsCoreWidget lcw);
void LvnsCoreInitSound(LvnsCoreWidget lcw);

#endif
