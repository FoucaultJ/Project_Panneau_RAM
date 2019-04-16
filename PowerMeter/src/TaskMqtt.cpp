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
	mqtt = new TMQTT((char*)"8",(char*)"172.17.50.103");
}

TaskMqtt::~TaskMqtt() {
	// TODO Auto-generated destructor stub
	delete mqtt;
}

void TaskMqtt::task(void)
    {
    // variable locale




    // synchronisation démarrage tâche
    signalStart();

    while(1)
	{

    	mqtt->publish(NULL,"RAM/powermeter/etats/Van",partage.getVan().size(),partage.getVan().c_str());

    	mqtt->publish(NULL,"RAM/powermeter/etats/Vbn",partage.getVbn().size(),partage.getVbn().c_str());

    	mqtt->publish(NULL,"RAM/powermeter/etats/Vab",partage.getVab().size(),partage.getVab().c_str());

    	mqtt->publish(NULL,"RAM/powermeter/etats/Ia",partage.getIa().size(),partage.getIa().c_str());

    	mqtt->publish(NULL,"RAM/powermeter/etats/Ib",partage.getIb().size(),partage.getIb().c_str());

    	mqtt->publish(NULL,"RAM/powermeter/etats/KW",partage.getPuissance().size(),partage.getPuissance().c_str());

    	mqtt->publish(NULL,"RAM/powermeter/etats/KWh",partage.getEnergetique().size(),partage.getEnergetique().c_str());

    	mqtt->publish(NULL,"RAM/powermeter/etats/FP",partage.getFacPuis().size(),partage.getFacPuis().c_str());
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
