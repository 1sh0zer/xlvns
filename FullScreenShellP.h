/*
 * FullScreenShell
 * $Id: FullScreenShellP.h,v 1.1 2001/06/09 15:20:24 go Exp $
 */

#ifndef __FullScreenShellP_h
#define __FullScreenShellP_h

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/ShellP.h>
#include <X11/extensions/xf86vmode.h>
#define MINMAJOR 0
#define MINMINOR 5

#include "FullScreenShell.h"

typedef struct {
	XtPointer extension;
} FullScreenShellClassPart;

typedef struct _FullScreenShellClassRec {
  	CoreClassPart             core_class;
	CompositeClassPart        composite_class;
	ShellClassPart            shell_class;
	WMShellClassPart          wm_shell_class;
	VendorShellClassPart      vendor_shell_class;
	TopLevelShellClassPart    top_level_shell_class;
	ApplicationShellClassPart application_shell_class;
	SessionShellClassPart     session_shell_class;
	FullScreenShellClassPart  fullscreen_shell_class;
} FullScreenShellClassRec;

extern FullScreenShellClassRec fullScreenShellClassRec;

typedef struct _FullScreenShellPart {
	Bool enable_screen_mode;          /* �����̥⡼�ɤ��Ȥ��뤫? */
	int vid_count;                    /* �⡼�ɤο�              */
	XF86VidModeModeInfo **modeinfos;  /* �⡼�ɾ��� (0 ���ǽ�)   */
	Bool screen_mode;                 /* ���ߤξ���              */
	Dimension width;                  /* Window ���᤹���Υ����� */
	Dimension height;                 /* Window ���᤹���Υ����� */
	int getsize;
} FullScreenShellPart;

typedef struct _FullScreenShellRec {
	CorePart 	     core;
	CompositePart 	     composite;
	ShellPart 	     shell;
	WMShellPart	     wm;
	VendorShellPart	     vendor;
	TopLevelShellPart    topLevel;
	ApplicationShellPart application;
	SessionShellPart     session;
	FullScreenShellPart  fullscreen;
} FullScreenShellRec;

/* convinient defines */
#define COREWIDTH  (fssw->core.width)
#define COREHEIGHT (fssw->core.height)
#define FSSW       (fssw->fullscreen)

/* semi public functions */
void FullScreenShellRestoreMode(FullScreenShellWidget fssw);

#endif
