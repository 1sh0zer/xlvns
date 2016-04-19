#import <Cocoa/Cocoa.h>
#import "NSLvnsImage.h"

#include <sys/time.h>
#include "Lvns.h"

@interface LvnsController : NSView
{
	/* �`��ΏۃE�C���h�E */
    IBOutlet id mainWindow;

	/* LVNS ���\���� */
    Lvns *lvns;
	
	/* Flip �����p */
	NSTimeInterval wait_clock;
	NSTimeInterval wait_count;

	/* �����`��o�b�t�@�p */
	NSLvnsImage *image;

	/* �^�C�}�ێ��p */
    struct timeval timer[10];
}	

- (IBAction) Select              : (id) sender;
- (IBAction) Cancel              : (id) sender; 
- (IBAction) ImageMode           : (id) sender;
- (IBAction) SkipTillSelect      : (id) sender ;
- (IBAction) SkipTillSelectForce : (id) sender;
- (IBAction) CursorUp            : (id) sender;
- (IBAction) CursorDown          : (id ) sender;


@end


