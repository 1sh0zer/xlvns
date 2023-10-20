#ifndef __LvnsMenu_h_
#define __LvnsMenu_h_

typedef struct {
	int xpos;
	int ypos;
	int width;
	int height;
	u_char* str;
	int attr;
} MenuLine;

#define STRW(str) ((sizeof(str)-1)/2*CHARSIZE)
#define CENTER(str) ((WIDTH2-STRW(str))/2)
#define MENULINE(y, str, attr) {CENTER(str), YPOS(y), STRW(str), CHARSIZE, str, attr}

typedef struct MenuData {
	MenuLine *line;
	int select_num;
	int demo_choice;
	int (*menu)(struct Lvns *lvns, int select);
	/*
	  ����  0: ��˥塼��³
	  ���� -1: ����󥻥�(��˥塼��λ)
	  ����  +: ���������λ(��˥塼��λ)
	*/
	void (*disp)(struct Lvns *lvns, struct MenuData *menu_data);
} MenuData;

void LvnsDefaultMenuDisp(struct Lvns *lvns, MenuData *data);
int  LvnsMenu(struct Lvns *, MenuData *data, int cancelok);

#endif
