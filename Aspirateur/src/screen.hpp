#ifndef SCREEN_HPP
#define SCREEN_HPP

#define QUEUE_KERNEL  0
#define QUEUE_LOCAL   1

#define USE_QUEUE QUEUE_KERNEL

#include <stdint.h>
#include "thread.hpp"
#include "mutexAlloc.hpp"
#if USE_QUEUE == QUEUE_KERNEL
    #include <mqueue.h>
#else
    #include "queue.hpp"
#endif

class TScreen  : public TThread
    {
    private:
	struct message_t
	    {
	    int32_t x;
	    int32_t y;
	    char *buffer;
	    };

	#if USE_QUEUE == QUEUE_KERNEL
	mqd_t queueDisplay;
	#else
	TQueue queueDisplay;
	#endif

	void openDisplay(void);
	void closeDisplay(void);
    public:
	TScreen(void);
	~TScreen();

	void dispStr(int32_t x,int32_t y,char *strDebug);
	void dispStr(int32_t x,int32_t y,const char *strDebug);
	void dispStr(int32_t x,int32_t y,unsigned char *strDebug);
	void dispStr(int32_t x,int32_t y,const unsigned char *strDebug);

	void task(void);
    };

extern TScreen *screen;

#endif // SCREEN_HPP
