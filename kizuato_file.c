/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: kizuato_file.c,v 1.14 2001/08/05 10:58:09 go Exp $
 *
 */

/* 
 * �� �����������
 */

#include <stdio.h>
#include "kizuato.h"

/**
 * �ե�����̾�η���
 */
static void
KizuatoFileName(Lvns *lvns, char *dst, size_t len, int no)
{
    snprintf(dst, len, "%s/kizuato%d.dat", lvns->savedata_path, no);
}


#define PUTLONG(d) putc(((d) >> 24) & 0xff, fp); putc(((d) >> 16) & 0xff, fp); putc(((d) >> 8) & 0xff, fp); putc(((d) >> 0) & 0xff, fp)


/**
 * ���ߤΤ�����ؤξ��֤���¸
 */
void
KizuatoSave(Lvns *lvns)
{
    char name[FILENAME_MAX];
    FILE *fp;
    int i;
    KizuatoState *state = (KizuatoState *)lvns->system_state;

    KizuatoFileName(lvns, name, sizeof name, state->siori_no);
    dprintf((stderr, "save: %s\n", name));

    if ((fp = fopen(name, "w")) == NULL) {
	perror("SizukuSave");
	return;
    }

    /* ���ʥꥪ�ֹ�    */
    putc(lvns->savepoint.scn, fp);

    /* �֥�å��ֹ� */  
    putc(lvns->savepoint.blk, fp);

    /* ���ʥꥪ�Υ��ե��å� */
    PUTLONG(lvns->savepoint.scn_offset);

    /* �������� */
    putc(lvns->savepoint.bg_type, fp);
    putc(lvns->savepoint.bg_no, fp);
    putc(0, fp); /* BG PAL */
    putc(lvns->savepoint.character_no[0], fp);
    putc(lvns->savepoint.character_no[1], fp);
    putc(lvns->savepoint.character_no[2], fp);

    /* ̤���ե饰 */
    for (i=0; i<KIZUATO_SCN_NO;i++) {
	putc(state->seen_flag[i], fp);
    }

    /* ����ե饰 */
    for (i=0; i<KIZUATO_FLAG_NO;i++) {
	putc(state->flag_save[i], fp);
    }

    /* ����ǥ��󥰸������� */
    for (i=0; i<sizeof state->ending;i++) {
	putc(state->ending[i], fp);
    }

    /* BGM �ξ��� */
    putc(lvns->savepoint.current_music, fp);

    fclose(fp);
}

/**
 * ���ߤΤ����꤫��ξ��֤Υ���
 */
void
KizuatoLoad(Lvns *lvns)
{
    char name[FILENAME_MAX];
    FILE *fp;
    int i;
    KizuatoState *state = (KizuatoState *)lvns->system_state;

    KizuatoFileName(lvns, name, sizeof name, state->siori_no);
    dprintf((stderr, "load: %s\n", name));

    if ((fp = fopen(name, "r")) == NULL) {
        KizuatoSioriInit(lvns);
	return;
    }

    /* ���ߤΥ��ʥꥪ�ֹ�    */
    lvns->savepoint.scn = fgetc(fp);

    /* ���ߤΥ֥�å��ֹ�    */
    lvns->savepoint.blk = fgetc(fp);

    /* ���ʥꥪ���ե��å� */
    {
        int a,b,c,d;
        a = getc(fp);
        b = getc(fp);
        c = getc(fp);
        d = getc(fp);
        lvns->savepoint.scn_offset = (a <<24) + (b<<16) + (c<<8) + d;
    }
	
    /* �������� */
    lvns->savepoint.bg_type   = getc(fp);
    lvns->savepoint.bg_no     = getc(fp);
    getc(fp); /* BG PAL */
    lvns->savepoint.character_no[0] = getc(fp);
    lvns->savepoint.character_no[1] = getc(fp);
    lvns->savepoint.character_no[2] = getc(fp);

	
    /* ̤���ե饰 */
    for (i=0; i<KIZUATO_SCN_NO;i++) {
		state->seen_flag[i] = getc(fp);
    }
	
    /* ����ե饰 */
    for (i=0; i<KIZUATO_FLAG_NO;i++) {
		state->flag_save[i] = getc(fp);
    }
	
    /* ����ǥ��󥰸������� */
    for (i=0; i<sizeof state->ending;i++) {
		state->ending[i] = getc(fp);
    }
	
    /* BGM �ξ��� */
    lvns->savepoint.current_music = getc(fp);

    fclose(fp);
}

