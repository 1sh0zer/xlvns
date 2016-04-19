/*
 * FullScreenShell
 * $Id: FullScreenShell.c,v 1.2 2001/06/27 20:37:33 go Exp $
 */

#include <stdio.h>
#include "FullScreenShellP.h"

/* resources */
#define Offset(x) (XtOffsetOf(FullScreenShellRec, fullscreen.x))
static XtResource resources[] = {
    {XtNscreenMode, XtCScreenMode, XtRBool, sizeof(Bool),
     Offset(screen_mode), XtRImmediate, (XtPointer)False},
};
#undef Offset

/* actions */
static void ScreenMode(Widget w, XEvent *e, String *p, Cardinal *n);
static void WindowMode(Widget w, XEvent *e, String *p, Cardinal *n);
static void ScreenWindow(Widget w, XEvent *e, String *p, Cardinal *n);
static void Enter(Widget w, XEvent *e, String *p, Cardinal *n);
static void Map(Widget w, XEvent *e, String *p, Cardinal *n);
static XtActionsRec actions[] = {
    {"screen", ScreenMode},
    {"window", WindowMode},
    {"scrwin", ScreenWindow},
    {"enter",  Enter},
    {"map",    Map},
};

/* translations */
static char translations[] =
   "Meta<Key>Return: scrwin()\n"
   "<Enter>: enter()\n"
   "<Map>: map()";

/* class methods */
static void Initialize(Widget, Widget, ArgList args, Cardinal * num);
static void Destroy(Widget);
static void Realize(Widget, XtValueMask *, XSetWindowAttributes *);
static void Resize(Widget widget);

static CompositeClassExtensionRec fullScreenCompositeClassExtension = {
    /* next_extension	*/	NULL,
    /* record_type	    */	NULLQUARK,
    /* version		    */	XtCompositeExtensionVersion,
    /* record_size	    */	sizeof(CompositeClassExtensionRec),
    /* accepts_objects	*/	TRUE,
    /* allows_change_managed_set */ FALSE
};

#define SUPERCLASS ((WidgetClass)&sessionShellClassRec)
FullScreenShellClassRec fullScreenShellClassRec = {
  {
    /* superclass         */    SUPERCLASS,
    /* class_name         */    "FullScreenShell",
    /* size               */    sizeof(FullScreenShellRec),
    /* Class Initializer  */	NULL,
    /* class_part_initialize*/	NULL,
    /* Class init'ed ?    */	FALSE,
    /* initialize         */    Initialize,
    /* initialize_notify  */	NULL,	
    /* realize            */    Realize,
    /* actions            */    actions,
    /* num_actions        */    XtNumber(actions),
    /* resources          */    resources,
    /* resource_count     */	XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    FALSE,
    /* compress_exposure  */    TRUE,
    /* compress_enterleave*/    FALSE,
    /* visible_interest   */    FALSE,
    /* destroy            */    Destroy,
    /* resize             */    Resize,
    /* expose             */    NULL,
    /* set_values         */    NULL,
    /* set_values_hook    */	NULL,		
    /* set_values_almost  */	XtInheritSetValuesAlmost,
    /* get_values_hook    */	NULL,
    /* accept_focus       */    NULL,
    /* intrinsics version */	XtVersion,
    /* callback offsets   */    NULL,
    /* tm_table		      */	translations,
    /* query_geometry	  */	NULL,
    /* display_accelerator*/	NULL,
    /* extension	      */	NULL
  },{
    /* geometry_manager   */    XtInheritGeometryManager,
    /* change_managed     */    XtInheritChangeManaged,
    /* insert_child	  */	    XtInheritInsertChild,
    /* delete_child	  */	    XtInheritDeleteChild,
    /* extension	  */	    (XtPointer)&fullScreenCompositeClassExtension
  },{
    /* extension	  */	NULL
  },{
    /* extension	  */	NULL
  },{
    /* extension	  */	NULL
  },{
    /* extension	  */	NULL
  },{
    /* extension	  */	NULL
  },{
    /* extension      */    NULL
  },{
    /* extension      */    NULL
  }
};

WidgetClass fullScreenShellWidgetClass = (WidgetClass)(&fullScreenShellClassRec);

/* -------------------------------------------------------------------------- 
 * private functions
 */

static XF86VidModeModeInfo *
checkModeLinesRes(XF86VidModeModeInfo **modeinfos, int count, int w, int h)
{
    int i;
    for (i=0;i<count;i++) { 
        if (modeinfos[i]->hdisplay == w && modeinfos[i]->vdisplay == h)  
            return modeinfos[i];
    }
    return NULL;
}

/**
 * $BA42hLL%b!<%I$KA+0\(B
 */
static void
_ToScreenMode(FullScreenShellWidget fssw)
{
	Display *dpy  = XtDisplay((Widget)fssw);
	int screen_no = XScreenNumberOfScreen(XtScreen((Widget)fssw));
    XF86VidModeModeInfo  *sel;

    if ((sel = checkModeLinesRes(FSSW.modeinfos, FSSW.vid_count, 640, 480)) == NULL &&
        (sel = checkModeLinesRes(FSSW.modeinfos, FSSW.vid_count, 800, 600)) == NULL) {
        fprintf(stderr, "warning: Not found good mode...");
        FSSW.enable_screen_mode = False;
    } else if (!XF86VidModeSwitchToMode(dpy, screen_no, sel)) {
        fprintf(stderr, "warning: Can't set modeline\n");
        FSSW.enable_screen_mode = False;
    } else {   
        XF86VidModeSetViewPort(dpy, screen_no, 0, 0);
        XWarpPointer(dpy, None, RootWindowOfScreen(XtScreen((Widget)fssw)),
                     0, 0, 0, 0, 0, 0);
        XSync(dpy, False);
        XtVaSetValues((Widget)fssw, XtNoverrideRedirect, True, NULL);
		FSSW.getsize = False;
		XtConfigureWidget((Widget)fssw, 0, 0, sel->hdisplay, sel->vdisplay, 0);
		FSSW.screen_mode = True;
    }   
}

/* -------------------------------------------------------------------------- 
 * Actions
 */

static void
ToWindowMode(FullScreenShellWidget fssw)
{
    if (FSSW.enable_screen_mode) {
		if (FSSW.screen_mode) {
			XtUnmapWidget((Widget)fssw);
			XtFullScreenShellWidgetRestoreMode(fssw);
			FSSW.getsize = False; 
			XtVaSetValues((Widget)fssw, XtNoverrideRedirect, False, NULL);
			XtResizeWidget((Widget)fssw, FSSW.width, FSSW.height, 0);
			FSSW.screen_mode = False;
			XtMapWidget((Widget)fssw);
		}
	}
}

static void
ToScreenMode(FullScreenShellWidget fssw)
{
    if (FSSW.enable_screen_mode) {
		if (!FSSW.screen_mode) {
			XtUnmapWidget((Widget)fssw);
			_ToScreenMode(fssw);
			XtMapWidget((Widget)fssw);
			XGrabPointer(XtDisplay((Widget)fssw), 
						 XtWindow((Widget)fssw), True, 0,
						 GrabModeAsync, GrabModeAsync, 
						 XtWindow((Widget)fssw), None, CurrentTime);
		}
	}
}


/**
 * $BA42hLL%b!<%I$X@Z$jBX$((B
 */
static void ScreenMode(Widget w, XEvent *e, String *p, Cardinal *n)
{
	ToScreenMode((FullScreenShellWidget)w);
}

/**
 * $B%&%#%s%I%&%b!<%I$X@Z$jBX$((B
 */
static void
WindowMode(Widget w, XEvent *e, String *p, Cardinal *n)
{
	ToWindowMode((FullScreenShellWidget)w);
}

/**
 * $BA42hLL%b!<%I$H%&%#%s%I%&%b!<%I$N@Z$jBX$((B
 */
static void
ScreenWindow(Widget w, XEvent *e, String *p, Cardinal *n)
{
    FullScreenShellWidget fssw = (FullScreenShellWidget)w;
	if (FSSW.screen_mode) {
		ToWindowMode(fssw);
	} else {
		ToScreenMode(fssw);
    }
}

/**
 * $B%9%/%j!<%s%b!<%I$N>l9g$OF~NO%U%)!<%+%9$r<hF@$9$k(B
 */
static void
Enter(Widget w, XEvent *e, String *p, Cardinal *n)
{
	FullScreenShellWidget fssw = (FullScreenShellWidget)w;
	if (FSSW.screen_mode)
		XSetInputFocus(XtDisplay(w), XtWindow(w), RevertToNone, CurrentTime);
}

/**
 * $B%9%/%j!<%s%b!<%I$N>l9g$O%]%$%s%?F~NO$r<hF@$9$k(B
 */
static void
Map(Widget w, XEvent *e, String *p, Cardinal *n)
{
	FullScreenShellWidget fssw = (FullScreenShellWidget)w;
	if (FSSW.screen_mode)
		XGrabPointer(XtDisplay(w), 
					 XtWindow(w), True, 0,
					 GrabModeAsync, GrabModeAsync, 
					 XtWindow(w), None, CurrentTime);
	FSSW.getsize = True;
}

/* ------------------------------------------------------------------- 
 * Class Mehotds
 */

static void
Initialize(Widget req, Widget new, ArgList args, Cardinal * num)
{
    FullScreenShellWidget fssw = (FullScreenShellWidget)new;
	int major, minor;
	int eventBase, errorBase;

	Display *dpy = XtDisplay((Widget)fssw);

    /* $B%9%/%j!<%s%b!<%I$NM-8z(B/$BL58z%A%'%C%/(B */
	if (!XF86VidModeQueryVersion(dpy, &major, &minor)) {
		fprintf(stderr, "Can't query video extension version\n");
		FSSW.enable_screen_mode = False;
		FSSW.screen_mode = False;
	} else if (!XF86VidModeQueryExtension(dpy, &eventBase, &errorBase)) {
		fprintf(stderr, "Can't query video extension information\n");
		FSSW.enable_screen_mode = False;
		FSSW.screen_mode = False;
	} else if (major < MINMAJOR ||
			   (major == MINMAJOR && minor < MINMINOR)) {
		fprintf(stderr, "Old XFree86-VidModeExtension version(%d.%d)\n", major, minor);
		fprintf(stderr, "Required version %d.%d\n", MINMAJOR, MINMINOR);
		FSSW.enable_screen_mode = False;
		FSSW.screen_mode = False;
	} else {
		XF86VidModeModeInfo  *sel;
		if (!XF86VidModeGetAllModeLines(dpy, 
										XScreenNumberOfScreen(XtScreen(new)),
										&FSSW.vid_count, &FSSW.modeinfos)) {
			fprintf(stderr, "Can't get modeinfos\n");
			FSSW.enable_screen_mode = False;
		} else if ((sel = checkModeLinesRes(FSSW.modeinfos, FSSW.vid_count, 640, 480)) == NULL &&
				   (sel = checkModeLinesRes(FSSW.modeinfos, FSSW.vid_count, 800, 600)) == NULL) {
			fprintf(stderr, "Not found good mode...");
			FSSW.enable_screen_mode = False;
			FSSW.screen_mode = False;
		} else {
			FSSW.enable_screen_mode = True;
		}
    }
	FSSW.getsize = True;
}

static void
Destroy(Widget w)
{
    FullScreenShellWidget fssw = (FullScreenShellWidget)w;
	XtFullScreenShellWidgetRestoreMode(fssw);
}

static void 
Realize(Widget w, XtValueMask *mask, XSetWindowAttributes *attr)
{
    FullScreenShellWidget fssw = (FullScreenShellWidget)w;
	SUPERCLASS->core_class.realize(w, mask, attr);

    if (FSSW.enable_screen_mode && FSSW.screen_mode) {
		/* $B$$$C$?$s%U%i%0$r>C5n!D(B */
		FSSW.screen_mode = False;
        _ToScreenMode(fssw);
    }
}

static void
Resize(Widget widget)
{
    FullScreenShellWidget fssw = (FullScreenShellWidget)widget;
	if (SUPERCLASS->core_class.resize)
		SUPERCLASS->core_class.resize(widget);
	// $B%5%$%:$r5-O?(B
	if (FSSW.getsize) {
		FSSW.width  = COREWIDTH;
		FSSW.height = COREHEIGHT;
	}
}

/* ------------------------------------------------------------------- 
 * Public Functions
 */


Bool 
XtFullScreenShellWidgetEnableScreenMode(FullScreenShellWidget fssw)
{
	return FSSW.enable_screen_mode;
}

Bool
XtFullScreenShellWidgetScreenMode(FullScreenShellWidget fssw)
{
	return FSSW.screen_mode;
}

void 
XtFullScreenShellWidgetSetBestSize(FullScreenShellWidget fssw, 
								   Dimension width, Dimension height)
{
	FSSW.width = width;
	FSSW.height = height;
}

/**
 * $B2hLL$r:G=i$N>uBV$KLa$9(B
 */
void
XtFullScreenShellWidgetRestoreMode(FullScreenShellWidget fssw)
{
    if (FSSW.screen_mode) {
        XF86VidModeSwitchToMode(XtDisplay((Widget)fssw), 
								XScreenNumberOfScreen(XtScreen((Widget)fssw)),
								FSSW.modeinfos[0]);
		XSync(XtDisplay((Widget)fssw), False);
        FSSW.screen_mode = False;
    }
}

void
XtFullScreenShellWidgetToScreenMode(FullScreenShellWidget fssw)
{
	ToScreenMode(fssw);
}

void
XtFullScreenShellWidgetToWindowMode(FullScreenShellWidget fssw)
{
	ToWindowMode(fssw);
}

void
XtFullScreenShellWidgetScreenWindow(FullScreenShellWidget fssw)
{
	if (FSSW.screen_mode) {
		ToWindowMode(fssw);
	} else {
		ToScreenMode(fssw);
    }
}


