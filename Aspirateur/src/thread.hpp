#ifndef THREAD_HPP
#define THREAD_HPP

/*
 * Option de configuration de l'environnement de développement
 *
 * CFLAGS
 * -D_GNU_SOURCE -D_REENTRANT
 *
 * LDFLAGS
 * -lpthread -lrt
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>     // PTHREAD_STACK_MIN
#include <sys/mman.h>   // mlockall
#include "sem.hpp"
#include "mutexAlloc.hpp"

struct sharedData_t
    {
    pthread_barrier_t barriere;
    };

class TThread
    {
    public:
	enum destruction_t
	    {
	    DESTRUCTION_SYNCHRONE,
	    DESTRUCTION_ASYNCHRONE
	    };

	struct threads_t
	    {
	    int32_t id;
	    const char *name;
	    void * (*pf)(void *);
	    sharedData_t *shared;
	    int32_t policy;                // SCHED_OTHER, SCHED_RR, SCHED_FIFO
	    int32_t priority;              // 1 @ 99 si la police est différente de SCHED_OTHER
	    destruction_t destruction;
	    int32_t noCpu;
	    pthread_t pthread;
	    };

    protected:
	threads_t thread;

	static TSemaphore sync;
	static uint16_t nbObjet;
	static TThread *ptrThis;

	void initTask(threads_t *thread);
	void createTask(threads_t *thread);
	void deleteTask(threads_t *thread);

	static void *handler(void *arg);

    public:
	TThread(const char *name,struct sharedData_t *shared,int32_t policy,int32_t priority,destruction_t destruction,int32_t cpu  = -1);
	virtual ~TThread();

	virtual void start(void);
	virtual void signalStart(void);
	virtual void task(void) = 0;

	static void initTaskMain(int32_t policy = SCHED_FIFO,int32_t cpu  = -1);  // SCHED_OTHER, SCHED_RR, SCHED_FIFO
    };

#endif //THREAD_HPP
