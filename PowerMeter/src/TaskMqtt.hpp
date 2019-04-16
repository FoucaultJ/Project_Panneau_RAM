/*
 * TaskMqtt.hpp
 *
 *  Created on: 2017-11-23
 *      Author: jfoucault
 */

#ifndef TASKMQTT_HPP_
#define TASKMQTT_HPP_

#include "thread.hpp"
#include "TMQTT.h"
#include "TPartage.hpp"
extern TPartage partage;

class TaskMqtt : public TThread
{
	private:
	TMQTT *mqtt;
	public:
	TaskMqtt(const char *name,sharedData_t *shared,int policy,int priority,destruction_t destruction);
	~TaskMqtt();

	void task(void);
};

#endif /* TASKMQTT_HPP_ */
