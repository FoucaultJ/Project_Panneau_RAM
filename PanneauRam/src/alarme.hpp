#ifndef ALARME_HPP
#define ALARME_HPP

#include "thread.hpp"
#include "ram.hpp"

class TAlarme : public TThread
    {
    public:

    private:
	TRam::partageRam_t *partageRam;

    public:
	TAlarme(const char *name,sharedData_t *shared,int policy,int priority,destruction_t destruction,int noCpu = -1);
	~TAlarme();

	// tâche control
	void task(void);

    };

extern TAlarme *alarme;

#endif //ALARME_HPP
