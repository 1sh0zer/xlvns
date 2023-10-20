/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: sximage.h,v 1.15 2001/07/01 14:29:52 go Exp $
 *
 */
#ifndef _sximage_x_h_
#define _sximage_x_h_

#define STORE16_LSB(p,d)  (p[0]= (d) & 0xff, p[1]= ((d) >> 8) & 0xff)
#define STORE24_LSB(p,d)  (p[0]= (d) & 0xff, p[1]= ((d) >> 8) & 0xff, p[2]= ((d) >> 16) & 0xff)
#define STORE32_LSB(p,d)  (p[0]= (d) & 0xff, p[1]= ((d) >> 8) & 0xff, p[2]= ((d) >> 16) & 0xff, p[3]= ((d) >> 24) & 0xf)

#define STORE16_HSB(p,d)  (p[1]= (d) & 0xff, p[0]= ((d) >> 8) & 0xff)
#define STORE24_HSB(p,d)  (p[2]= (d) & 0xff, p[1]= ((d) >> 8) & 0xff, p[0]= ((d) >> 16) & 0xff)
#define STORE32_HSB(p,d)  (p[3]= (d) & 0xff, p[2]= ((d) >> 8) & 0xff, p[1]= ((d) >> 16) & 0xff, p[0]= ((d) >> 24) & 0xf)

#define SXIMAGE_TPIXEL(s,r,g,b)\
(s->tcmap->red[r] | s->tcmap->green[g] | s->tcmap->blue[b])

/*
 * XImage �˥ǥ����ץ쥤�����ɽ���оݤˤʤ륦����ɥ���
 * �ӥ��奢�뤪��ӥ��顼�ޥåפξ�������������Ρ�
 * MIT-sharad �Υ��ݡ��Ȥ�Ԥ�
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>

/* TrueColor �ѤΥ��顼�ޥå� */
typedef struct TrueColorMap {
    long red[256];
    long green[256];
    long blue[256];
} TrueColorMap;

typedef struct SuperXImage {
    
    XImage *ximage;           /* ���� XImage */
    Display *display;         /* ɽ���оݤˤʤäƤ���ǥ����ץ쥤 */
    Window window;            /* ɽ���оݤˤʤäƤ��륦����ɥ�   */
    Colormap colormap;        /* ���Ѥ��륫�顼�ޥå� */

    int xoff;                 /* Window �ȤΥ��ե��å� */
    int yoff;

    int visual_class;         /* Visual Class */

    long pixels[256];          /* pixel convert table */
    int pixel_num;             /* ���ݤ������ο� (for PseudoColor)  */

    /* for TrueColor */
    TrueColorMap *tcmap;       /* RGB->PIXEL���ִ��ѥơ��֥�    */

#ifdef MITSHM
    Bool shared_ok;             /* Shraed Extention ��ͭ�� */
    Bool shared_pixmap_ok;      /* Shared Pixmap ��ͭ��    */
    XShmSegmentInfo shminfo;    /* ��ͭ������� */
#endif

    /* with Pixmap... */
    GC gc;                      /* Default GC of Screen */
    Pixmap pixmap;              /* Pixmap */

} SuperXImage;

/*
 * ��������
 */
SuperXImage *
super_ximage_new(Display *dpy, Window win, int width, int height, int maxcol, int noshared);

void
super_ximage_set_offset(SuperXImage *sximage, int xoff, int yoff);

/*
 * XImage �����Ƥ� ���� Pixmap ��ȿ�Ǥ����� 
 */
void
super_ximage_set_pixmap(SuperXImage *sximage);

void
super_ximage_set_pixmap_area(SuperXImage *sximage, int x, int y, int w, int h);

/*
 * XImage �����Ƥ��� Pixmap �˽��Ϥ���
 */
void
super_ximage_put_pixmap(SuperXImage *sximage, Pixmap pixmap);

/*
 * �˴�
 */
void 
super_ximage_delete(SuperXImage *sximage);

/* 
 * ���̾õ�
 */
void
super_ximage_clear(SuperXImage *sximage, long pixel);

/*
 * 24��24 �Υӥåȥޥåץѥ������ pixel �ο������褹��
 */ 
void
super_ximage_put_pattern24(SuperXImage *sximage, int x, int y, long pixel, char *data);

/*
 * 24��24 �Υӥåȥޥåץѥ������ pixel �ο������褹��
 */ 
void
super_ximage_put_pattern24_2(SuperXImage *sximage, int x, int y, long pixel, char *data);

void
super_ximage_copy_area(SuperXImage *src, SuperXImage *dst, int x, int y,
                       int w, int h, int x2, int y2);
void
super_ximage_clear_area(SuperXImage *sximage, int x, int y,
                        int w, int h, long pixel);


void
super_ximage_copy_area_win(SuperXImage *sximage, 
                           int x, int y, int w, int h);

void
super_ximage_copy_area_win2(SuperXImage *sximage, 
			    int x, int y, int w, int h, int x2, int y2);

void 
super_ximage_copy_area_win_region(SuperXImage *sximage, Region region);

void
super_ximage_clear_area_win(SuperXImage *sximage, 
			    int x, int y, int w, int h,
			    long pixel);
void super_ximage_sync(SuperXImage *sximage);

void super_ximage_set_palette(SuperXImage *sximage, int index, int r, int g, int b);

#endif


