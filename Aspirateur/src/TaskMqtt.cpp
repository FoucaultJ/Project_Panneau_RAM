/*
 * TaskMqtt.cpp
 *
 *  Created on: 2017-11-23
 *      Author: jfoucault
 */

#include "TaskMqtt.hpp"
#include "screen.hpp"

TaskMqtt::TaskMqtt(const char *name,sharedData_t *shared,int policy,int priority,destruction_t destruction) :
TThread(name,shared,policy,priority,destruction) {
	// TODO Auto-generated constructor stub
	mqtt = new TMQTT((char*)"1",(char*)"172.17.50.103");
}

TaskMqtt::~TaskMqtt() {
	// TODO Auto-generated destructor stub
	delete mqtt;
}

void TaskMqtt::task(void)
    {
    // variable locale
	int mid =69;



    // synchronisation démarrage tâche
    signalStart();

    mqtt->subscribe(&mid,"RAM/shopvac/cmd/#",0);
    while(1)
	{

    	mqtt->publish(NULL,"RAM/shopvac/etats",partage.getProgression().size(),partage.getProgression().c_str());


	//traitement


    	if(mqtt->loop()!=0)
    		mqtt->reconnect();

	if(thread.destruction == DESTRUCTION_SYNCHRONE)
	    {
	    // point de destruction
	    pthread_testcancel();
	    }

	sleep(1);
	}
    }
