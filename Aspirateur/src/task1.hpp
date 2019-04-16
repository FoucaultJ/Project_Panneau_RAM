#ifndef TTASK1_HPP
#define TTASK1_HPP

#include "thread.hpp"

#include "TPartage.hpp"
class TTask1 : public TThread
    {
private:
    public:
	TTask1(const char *name,sharedData_t *shared,int policy,int priority,destruction_t destruction);
	~TTask1();

	void task(void);
    };

#endif //TTASK1_HPP
