#import "LvnsController.h"

#define TICKSPERSEC 1.0

@implementation LvnsController

// NSView ���\�b�h�Q

/* �ĕ`�� */
- (void) drawRect : (NSRect) arBounds
{
	[image draw : arBounds];
	[[NSGraphicsContext currentContext] flushGraphics];
}

// �C�x���g����

- (void)mouseMoved:(NSEvent *)theEvent
{
}

- (void)mouseDown:(NSEvent *)theEvent
{
	[self Select : self];
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
	[self Cancel : self];
} 

- (void)keyDown:(NSEvent *)theEvent
{
}

// actions 

- (IBAction) Select : (id) sender
{
	LvnsSelect(lvns);
}

- (IBAction) Cancel : (id) sender
{
	LvnsCancel(lvns);
}

- (IBAction) ImageMode : (id) sender
{
}

- (IBAction) SkipTillSelect : (id) sender
{
}

- (IBAction) SkipTillSelectForce : (id) sender
{
}

- (IBAction) CursorUp : (id) sender
{
	LvnsCursorUp(lvns);
}

- (IBAction) CursorDown : (id) sender
{
	LvnsCursorDown(lvns);
}

// privates

// Lvns System Depend Functions

/** 
 * �S��ʂ����t���b�V�����Ď��ۂ̉�ʂɔ��f������
 */
static void 
FlushWindow(Lvns *lvns)
{
	LvnsController *self = lvns->system_depend;
	[self setNeedsDisplay : true];
	//[self lockFocus];
	//[self->image draw : NSMakeRect(0,0,WIDTH,HEIGHT)];
	//[self unlockFocus];
}

/**
 * ��ʂ𕔕��I�Ƀ��t���b�V�����Ď��ۂ̉�ʂɔ��f������
 */
static void
FlushWindowArea(Lvns *lvns, int x, int y, int w, int h)
{
	LvnsController *self = lvns->system_depend;
	[self setNeedsDisplayInRect : NSMakeRect(x,y,w,h)];
	//[self lockFocus];
	//[self->image draw : NSMakeRect(x,HEIGHT-y-1,w,h)];
	//[self unlockFocus];
}

/**
 * �S�̂�`��(�t���b�V������) 
 */
static void
DrawWindow(Lvns *lvns) 
{
	LvnsController *self = lvns->system_depend;
	[self->image copy : lvns->vram : 0 : 0];
}

/**
 * �����`��(�t���b�V������) 
 */
static void
DrawWindowArea(Lvns *lvns, int x, int y, int w, int h, int x2, int y2)
{
	LvnsController *self = lvns->system_depend;
	[self->image copyArea : lvns->vram : x : y : w : h : x2 : y2];
}

/**
 * �S��ʉ�ʏ���(�t���b�V������)
 */
static void
ClearWindow(Lvns *lvns)
{
	LvnsController *self = lvns->system_depend;
	[self->image clearArea : 0 : 0 : WIDTH : HEIGHT : lvns->vram->black];
}

/**
 * ������ʏ���(�t���b�V������)
 */
static void
ClearWindowArea(Lvns *lvns, int x, int y, int w, int h)
{
	LvnsController *self = lvns->system_depend;
	[self->image clearArea : x : y : w : h : lvns->vram->black];
}

/**
 * ����̃C���f�b�N�X�ɃJ���[�����蓖�Ă�
 */
static void
SetPaletteIndex(Lvns *lvns, int index, int r, int g, int b)
{
	LvnsController *self = lvns->system_depend;
	[self->image setPalette : index r:r g:g b:b];
}

/**
 * �p���b�g�ݒ�(�ʏ�)
 * �S����ݒ肷��c
 */
static void
MySetPalette(Lvns *lvns)
{
	LvnsController *self = lvns->system_depend;
	[self->image setPal : lvns->vram];
}

/**
 * �p���b�g�ݒ�(multiple)
 */
static void
SetPaletteMulti(Lvns *lvns, int par16)
{
	LvnsController *self = lvns->system_depend;
	[self->image setPalMulti : lvns->vram : par16];
}

/**
 * �p���b�g�ݒ�(screen)
 */
static void
SetPaletteScreen(Lvns *lvns, int par16)
{
	LvnsController *self = lvns->system_depend;
	[self->image setPalScreen : lvns->vram : par16];
}

/* 
 * �}�X�N�p�^�[���ŃE�C���h�E�ɑ΂��ĕ`����s��(�t���b�V������)
 */
static void
DrawWindowMask(Lvns *lvns, int x, int y, int state)
{
	LvnsController *self = lvns->system_depend;
	[self->image copyMaskUnit : lvns->vram : x : y : x : y : state ];
}

/* 
 * ��`�p�^�[���ŃE�B���h�E�ɑ΂��ĕ`����s��(�t���b�V������)
 */
static void
DrawWindowSquareMask(Lvns *lvns, int x, int y, int state)
{
	LvnsController *self = lvns->system_depend;
	[self->image copySquareMaskUnit : lvns->vram : x : y : x : y : state];
}

/*
 * �\������Ă����ʂ̃I�t�Z�b�g�����炷(�U���p)
 */
static void
SetDispOffset(Lvns *lvns, int xoff, int yoff, int maxoff)
{
}

/* 
 * �����p�^�[���\�� (��/�� �p)
 */
static void
PutPattern(Lvns *lvns, int x, int y, int index, u_char *data)
{
	LvnsController *self = lvns->system_depend;
	[self->image putPattern24_2 : x : y : index : data];
}

/* 
 * �����p�^�[���\�� (ToHeart�p)
 */ 
static void 
PutPattern2(Lvns *lvns, int x, int y, int index, u_char *data)
{
	LvnsController *self = lvns->system_depend;
	[self->image putPattern24 : x : y : index : data];
}

/* 
 * �^�C�}�����Z�b�g����
 * @param no �^�C�}�ԍ�
 */
static void
ResetTimer(Lvns *lvns, int no)
{
	LvnsController *self = lvns->system_depend;
    gettimeofday(&self->timer[no], NULL);
}

/*
 * �^�C�}�l���擾����
 * @param no �^�C�}�ԍ�
 * @return �^�C�}�l (msec�P��)
 */	// �V���N��
	// �V���N��

static long 
GetTimer(Lvns *lvns, int no)
{
	LvnsController *self = lvns->system_depend;
	struct timeval current;
    gettimeofday(&current, NULL);
    return (current.tv_sec - self->timer[no].tv_sec) * 1000 +
        (current.tv_usec - self->timer[no].tv_usec) / 1000;
}

/**
 * ��`��`�悷��
 * @param x,y,w,h �ʒu�ƃT�C�Y
 * @param idx �F�̃C���f�b�N�X�l
 */
static void
DrawBox(Lvns *lvns, int x, int y, int w, int h, int idx)
{
}

#include <sys/types.h>
#include <sys/stat.h>

/** 
 * �w�肵���t�@�C���̍X�V���t���擾����
 */
static time_t
GetFileTime(Lvns *lvns, const char *path)
{
    struct stat sb;
    if (stat(path, &sb) < 0) {
        return 0;
    }
    return sb.st_mtime;
}

- (void) flip
{
	int wait, sa;

	wait = wait_count / 60;
	if (wait > 0) {
		u_long tw;
		sa = [NSDate timeIntervalSinceReferenceDate] - wait_clock;
		if (sa > 0) wait -= sa;
		if (wait > 0) {
			NSDate *date = [[NSDate alloc] initWithTimeIntervalSinceNow: wait];
			[NSThread sleepUntilDate : date];
			[date release];
		}
		tw = [NSDate timeIntervalSinceReferenceDate];
		sa = tw - wait_clock;
		if (sa > 0) 
			wait_count -= sa * 60;
		else 
			wait_count = 0;
		wait_clock = tw;
	}
	wait_count += TICKSPERSEC;
	if (wait_count < 0) {
		wait_count = 0;
	}
}

/*
 * �`��^�C�~���O�҂�����
 */
static void
Flip(Lvns *lvns)
{
	LvnsController *self = lvns->system_depend;
	[self flip];
}

/**
 * ���C���G���W�� 
 */
- (void) runMain : (id) param
{
    LvnsMain(lvns);
}

extern int system_type;

/**
 * ����������
 */
- (void) applicationDidFinishLaunching:(NSNotification *)aNotification
{
    char *home = getenv("HOME");
    lvns = LvnsNew();

	if (home) {
		LvnsSetSaveDataPath(lvns, home);
	}
	//LvnsSetDataPath(lvns, );

	/* ���\�[�X�̔��f */
	lvns->demo_mode        = False;
	lvns->seen_mode        = False;
	lvns->latitude_dark    = 11;
	lvns->key_click_fast   = True;

	/* �V�X�e���ˑ��֐��Q�̐ݒ� */
	lvns->flushWindow          = FlushWindow;
	lvns->flushWindowArea      = FlushWindowArea;
	lvns->drawWindow           = DrawWindow;
	lvns->drawWindowArea       = DrawWindowArea;
	lvns->clearWindow          = ClearWindow;
	lvns->clearWindowArea      = ClearWindowArea;
	lvns->setPaletteIndex      = SetPaletteIndex;
	lvns->setPalette           = MySetPalette;
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

	LvnsInitialize(lvns, system_type, self);

	//�T�C�Y����
	{
		NSSize size = { WIDTH, HEIGHT };
		[mainWindow setContentSize : size];
		[mainWindow setContentView : self];
		image = [[NSLvnsImage alloc] init : size];
	}

	wait_clock = [NSDate timeIntervalSinceReferenceDate];
	wait_count = TICKSPERSEC;

	// �X���b�h�𕪂��Ė{�̂𓮂���
	[NSThread detachNewThreadSelector: @selector(runMain:) toTarget:self withObject:self];
}

@end

