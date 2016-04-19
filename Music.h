#ifndef __Music_h
#define __Music_h

typedef struct {
	void *depend;
    void (*open)(void *dep);                 /* BGM$B=i4|2=(B       */
    void (*close)(void *dep);                /* BGM$B=*N;(B         */
    void (*start)(void *dep, int no);        /* $B1iAU3+;O(B        */
    void (*stop)(void *dep);                 /* $B1iAU=*N;(B        */
    void (*pause)(void *dep);                /* $B1iAU0l;~Dd;_(B    */
    void (*setVolume)(void *dep, int no);    /* $B%\%j%e!<%`;XDj(B  */
    int  (*getState)(void *dep);             /* $B1iAUCf%A%'%C%/(B  */
} Music;

#define MusicOpen(m)        if ((m) && (m)->open)  (m)->open((m)->depend)
#define MusicClose(m)       if ((m) && (m)->close) (m)->close((m)->depend)
#define MusicStart(m,n)     if ((m) && (m)->start) (m)->start((m)->depend,n)
#define MusicStop(m)        if ((m) && (m)->pause) (m)->stop((m)->depend)
#define MusicPause(m)       if ((m) && (m)->pause) (m)->pause((m)->depend)
#define MusicSetVolume(m,v) if ((m) && (m)->setVolume) (m)->setVolume((m)->depend,v)
#define MusicGetState(m) (((m) && (m)->getState) ? (m)->getState((m)->depend):1)

#endif

