/*
 * LEAF Visual Novel System For MGL2
 * (c) Copyright 2001 TF <tf@denpa.org>
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: pakconv.c,v 1.5 2001/08/11 12:30:07 tf Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pakconv.h"

static void convert(LeafPack *, PakWriter *);
static void fix_color(Image *);
static void set_transparent_color(Image *, int, int);
void lvnsfont_conv(u_char *src, u_char *dst, size_t size);
void lvnsfont_conv2(u_char *src, u_char *dst, size_t size);
void resize_24to12( u_char *src[], u_char *dst[]);

int
main(int argc, char *argv[])
{
    LeafPack *leafpack;
    PakWriter *pw;

    if (argc != 3) {
	fprintf(stderr, "usage: pakconv input.pak output.pak\n");
	exit(1);
    }

    if ((leafpack = leafpack_new(argv[1])) == NULL) {
	fprintf(stderr, "Can't open package %s.\n", argv[1]);
	exit(1);
    }
    pw = pakwriter_new(argv[2], leafpack->file_num);

    convert(leafpack, pw);

    pakwriter_delete(pw);
    leafpack_delete(leafpack);

    return 0;
}

static void
convert(LeafPack *lp, PakWriter *pw)
{
    int i;
    char *p;
    int idx, size, csize;
    int w, h, xo, yo;
    u_char *data, *data2, *data3;
    Image *img = NULL;

    for (i = 0; i < lp->file_num; i++) {
	char *name = lp->files[i].name;
	fprintf(stderr, "converting %s...", name);

	/*
	 * check image 
	 */
	if ((idx = leafpack_find(lp, name)) < 0) {
	    fprintf(stderr, "can't find image:%s\n", name);
	    continue;
	}

	/*
	 * extract image 
	 */
	if ((data = leafpack_extract(lp, idx, &size)) == NULL) {
	    fprintf(stderr, "can't extract image:%s\n", name);
	    continue;
	}

	p = strchr(name, '.') + 1;
	if (!strcasecmp(p, "lfg")) {
	    img = lvnsimage_load_lfg(data, NULL);
	    w = img->rwidth / 2;
	    h = img->rheight / 2;
	    xo = img->xoffset / 2;
	    yo = img->yoffset / 2;
	    if (!strcmp("OP2_MN_W.LFG", name)) {
	      /* �������� */
	      fprintf(stderr, "[correct color]...");
	      fix_color(img);
	    }
	    make_hsb_image(img);
	    data2 = pack_img(img->hsb_data, w, h, xo, yo, &size);
	    data3 = (u_char *)calloc(size, sizeof(u_char));
	    csize = lz77_compress(data2, size, data3);
	    put_compressed_file(pw, data3, size, csize, name);
	    free(data3);
	    free(data2);
	} else if (!strcasecmp(p, "lf2")) {
	    img = lvnsimage_load_lf2(data, NULL);
	    w = img->rwidth / 2;
	    h = img->rheight / 2;
	    xo = img->xoffset / 2;
	    yo = img->yoffset / 2;
	    if (!strcmp("TITLE.LF2", name)) {
	      /* Ʃ���������� */
	      fprintf(stderr, "[correct transparent color]...");
	      set_transparent_color(img, 0, 0);
	    }
	    make_hsb_image(img);
	    data2 = pack_img(img->hsb_data, w, h, xo, yo, &size);
	    data3 = (u_char *)calloc(size, sizeof(u_char));
	    csize = lz77_compress(data2, size, data3);
	    put_compressed_file(pw, data3, size, csize, name);
	    free(data3);
	    free(data2);

	} else if (!strcasecmp(p, "sdt")) {
	    fprintf(stderr, "[SDT]...");
	    img = lvnsimage_load_sdt(data, name);
	    w = img->rwidth / 2;
	    h = img->rheight / 2;
	    xo = img->xoffset / 2;
	    yo = img->yoffset / 2;
	    make_hsb_image(img);
	    data2 = pack_img(img->hsb_data, w, h, xo, yo, &size);
	    data3 = (u_char *) calloc(size, sizeof(u_char));
	    csize = lz77_compress(data2, size, data3);
	    put_compressed_file(pw, data3, size, csize, name);
	    free(data3);
	    free(data2);

	} else if (!strcasecmp(p, "knj")) {
	    // ���ߤϼ�, ���ξ��Τ�ͭ��
	    //   24x24(dep1)��12x12(dep2)���Ѵ�����٤ΥХåե�
	    //   ��ʸ��������Υ��������((24 * 24) / (12 * 12 * 2))
	    //   �Ȥʤ�Τǥ������ϥ��ꥸ�ʥ��1/2�ˤʤ롣
            data2 = malloc( size / 2);

	    switch( lp->type){
            case LPTYPE_SIZUWIN:
            case LPTYPE_KIZUWIN:
                lvnsfont_conv( data, data2, size);
                break;

            case LPTYPE_TOHEART:
            	lvnsfont_conv2( data, data2, size);
                break;

            default:
	    	fprintf(stderr, "[as is]...");
	    	put_file(pw, data, size, name);
	        free( data2);
		continue;
            }

	    // data3 = (u_char *) calloc(size, sizeof(u_char));
	    // csize = lz77_compress(data2, size, data3);
	    // put_compressed_file(pw, data3, size, csize, name);
	    put_file(pw, data2, size / 2, name);
	    //free( data);
	    free( data2);
	    // free( data3);

	} else {
	    fprintf(stderr, "[as is]...");
	    put_file(pw, data, size, name);
	}

	image_delete(img);
	img = NULL;
	free(data);
	fprintf(stderr, "done.\n");
    }
}


/*
 * �ե���Ƚ̾�(��/����)
 */
void
lvnsfont_conv( u_char *src_bmap, u_char *dst_bmap, size_t size)
{
     int pos;
     u_char c;
     u_char *src[25];
     u_char *dst[12];

     for( pos = 0; pos < 25; pos++){
	src[ pos] = malloc( 25 * sizeof( u_char));
     }

     for( pos = 0; pos < 12; pos++){
	dst[ pos] = malloc( 12 * sizeof( u_char));
     }
     
     for( pos = 0; pos < size;){
         int x, y; 
         for( x = 0; x < 3; x++){
             for( y = 0; y < 24; y++){
                 int i;

                 c = *src_bmap++;
		 pos++;

                 for( i = 0; i < 8; i++){
                     if( c & ( 0x80 >> i)){
                         src[y][ x * 8 + i] = 1;
                     } else {
                         src[y][ x * 8 + i] = 0;
                     }
                 }
             }
         }

	 resize_24to12( src, dst);

         for( y = 0; y < 12; y++, dst_bmap += 3){
             dst_bmap[0] = dst[y][0] << 6  | dst[y][1] << 4 |
                           dst[y][2] << 2  | dst[y][3];
             dst_bmap[1] = dst[y][4] << 6  | dst[y][5] << 4 |
                           dst[y][6] << 2  | dst[y][7];
             dst_bmap[2] = dst[y][8] << 6  | dst[y][9] << 4 |
                           dst[y][10] << 2 | dst[y][11];
         }
     }

     for( pos = 0; pos < 25; pos++){
	free( src[ pos]);
     }

     for( pos = 0; pos < 12; pos++){
	free( dst[ pos]);
     }
}

/*
 * �ե���Ƚ̾�(ToHeart��)
 */
void
lvnsfont_conv2( u_char* src_bmap, u_char* dst_bmap, size_t size)
{
     int pos;
     u_char c;
     u_char *src[25];
     u_char *dst[12];

     for( pos = 0; pos < 25; pos++){
	src[ pos] = malloc( 25 * sizeof( u_char));
     }

     for( pos = 0; pos < 12; pos++){
	dst[ pos] = malloc( 12 * sizeof( u_char));
     }
     
     for( pos = 0; pos < size;){
         int x, y; 
         for( y = 0; y < 24; y++){
             for( x = 0; x < 3; x++){
                 int i;

                 c = *src_bmap++;
		 pos++;

                 for( i = 0; i < 8; i++){
                     if( c & ( 0x80 >> i)){
                         src[y][ x * 8 + i] = 1;
                     } else {
                         src[y][ x * 8 + i] = 0;
                     }
                 }
             }
         }

	 resize_24to12( src, dst);

         for( y = 0; y < 12; y++, dst_bmap += 3){
             dst_bmap[0] = dst[y][0] << 6  | dst[y][1] << 4 |
                           dst[y][2] << 2  | dst[y][3];
             dst_bmap[1] = dst[y][4] << 6  | dst[y][5] << 4 |
                           dst[y][6] << 2  | dst[y][7];
             dst_bmap[2] = dst[y][8] << 6  | dst[y][9] << 4 |
                           dst[y][10] << 2 | dst[y][11];
         }
     }

     for( pos = 0; pos < 25; pos++){
	free( src[ pos]);
     }

     for( pos = 0; pos < 12; pos++){
	free( dst[ pos]);
     }
}

/*
 * 24x24(depth 1) -> 12x12(depth 2) (3x3ʿ�경)
 */
#define __ORG_WIDTH__	24
#define __ORG_HEIGHT__	24
#define __TGT_WIDTH__	12
#define __TGT_HEIGHT__	12

void
resize_24to12( u_char *src[], u_char *dst[])
{
     int x, y;
     static int btbl[ 10] = { 0, 0, 1, 1, 2, 2, 2, 3, 3, 3};
     /*
      * �̾����δ��ܺ�ɸ�μ����
      * �����»����Ǿ��ˤ���ٰʲ����ͤˤ��롣
      *
      *                         6   7   8   9   10  11
      *                         |   |   |   |   |   |
      * 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N 
      *   |   |   |   |   |   |
      *   0   1   2   3   4   5
      *
      *  3x3��ʿ�경�ʤΤ�ξü��1dot�ϴ��ܺ�ɸ�ˤ��ʤ�
      */
     /*
      * �����ΰ��ʲ��Τ褦��ʬ��
      *
      * +-----------+-----------+
      * |           |           |
      * |     A     |     B     |
      * |  X:0��11  |  X:12��22 |
      * |  Y:0��11  |  Y:0��11  |
      * |           |           |
      * +-----------+-----------+
      * |           |           |
      * |     C     |     D     |
      * |  X:0��11  |  X:12��22 |
      * |  Y:12��22 |  Y:12��22 |
      * |           |           |
      * +-----------+-----------+
      */
      for( y = 1; y < (__ORG_HEIGHT__ / 2); y += 2){
          // �ΰ�A������
          for( x = 1; x < (__ORG_WIDTH__ / 2); x += 2){
              dst[y / 2][x / 2] = btbl[   src[ y - 1][ x - 1]
                                        + src[ y - 1][ x + 0]
                                        + src[ y - 1][ x + 1]
                                        + src[ y + 0][ x - 1]
                                        + src[ y + 0][ x + 0]
                                        + src[ y + 0][ x + 1]
                                        + src[ y + 1][ x - 1]
                                        + src[ y + 1][ x + 0]
                                        + src[ y + 1][ x + 1]];
          }

          // �ΰ�B������
          for( x = (__ORG_WIDTH__ / 2); x < (__ORG_WIDTH__ - 1); x += 2){
              dst[y / 2][x / 2] = btbl[   src[ y - 1][ x - 1]
                                        + src[ y - 1][ x + 0]
                                        + src[ y - 1][ x + 1]
                                        + src[ y + 0][ x - 1]
                                        + src[ y + 0][ x + 0]
                                        + src[ y + 0][ x + 1]
                                        + src[ y + 1][ x - 1]
                                        + src[ y + 1][ x + 0]
                                        + src[ y + 1][ x + 1]];
          }
      }

      for( y = (__ORG_HEIGHT__ / 2); y < (__ORG_HEIGHT__ - 1); y += 2){
          // �ΰ�C������
          for( x = 1; x < (__ORG_WIDTH__ / 2); x += 2){
              dst[y / 2][x / 2] = btbl[   src[ y - 1][ x - 1]
                                        + src[ y - 1][ x + 0]
                                        + src[ y - 1][ x + 1]
                                        + src[ y + 0][ x - 1]
                                        + src[ y + 0][ x + 0]
                                        + src[ y + 0][ x + 1]
                                        + src[ y + 1][ x - 1]
                                        + src[ y + 1][ x + 0]
                                        + src[ y + 1][ x + 1]];
          }

          // �ΰ�D������
          for( x = (__ORG_WIDTH__ / 2); x < (__ORG_WIDTH__ - 1); x += 2){
              dst[y / 2][x / 2] = btbl[   src[ y - 1][ x - 1]
                                        + src[ y - 1][ x + 0]
                                        + src[ y - 1][ x + 1]
                                        + src[ y + 0][ x - 1]
                                        + src[ y + 0][ x + 0]
                                        + src[ y + 0][ x + 1]
                                        + src[ y + 1][ x - 1]
                                        + src[ y + 1][ x + 0]
                                        + src[ y + 1][ x + 1]];
          }
     }
}
#undef __ORG_WIDTH__
#undef __ORG_HEIGHT__
#undef __TGT_WIDTH__
#undef __TGT_HEIGHT__


/*
 * �ֺ��ץ����ץ˥󥰤Τ���ο�����
 */
static void
fix_color(Image *img)
{
  int i;

  for (i = 0; i < 16; i++) {
    if (img->palette[i][0] == 0xff && img->palette[i][1] == 0xff && img->palette[i][2] == 0xff) {
      img->palette[i][0] = 0x0;
      img->palette[i][1] = 0x0;
      img->palette[i][2] = 0x0;
    }
  }
}

static void
set_transparent_color(Image *img, int x, int y)
{
  img->transparent = img->data[y * img->rwidth + x];
}
