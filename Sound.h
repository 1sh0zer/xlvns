#ifndef __Sound_h
#define __Sound_h

typedef struct {
	void *depend;
    void (*open)(void *dep);                    /* SOUND$B=i4|2=(B         */
    void (*close)(void *dep);                   /* SOUND$B=*N;(B           */
    void (*load)(void *dep, const char *name);  /* $B%G!<%?$N%m!<%I(B      */
    void (*start)(void *dep);                   /* $B:F@83+;O(B            */
    void (*stop)(void *dep);                    /* $BDd;_(B                */
    int  (*getState)(void *dep);                /* $B1iAUCf%A%'%C%/(B      */
} Sound;

#define SoundOpen(m)     if ((m) && (m)->open)  (m)->open((m)->depend)
#define SoundClose(m)    if ((m) && (m)->close) (m)->close((m)->depend)
#define SoundLoad(m,n)   if ((m) && (m)->load)  (m)->load((m)->depend,n)
#define SoundStart(m)    if ((m) && (m)->start) (m)->start((m)->depend)
#define SoundStop(m)     if ((m) && (m)->stop)  (m)->stop((m)->depend)
#define SoundGetState(m) (((m) && (m)->getState) ? (m)->getState((m)->depend):1)

#endif
