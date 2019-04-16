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
	mqtt = new TMQTT((char*)"2",(char*)"172.17.50.103");
	//mqtt2 = new TMQTT((char*)"1",(char*)"172.17.50.103");
}

TaskMqtt::~TaskMqtt() {
	// TODO Auto-generated destructor stub
	delete mqtt;
	//delete mqtt2;
}

void TaskMqtt::task(void)
    {
    // variable locale
	int mid =69;
	string poids="0",tare="0",unite="lbs";


    // synchronisation démarrage tâche
    signalStart();
    mqtt->subscribe(&mid,"RAM/balanceMel/cmd/#",0); ///voir pour le subcribe de l'Aspirateur
    while(1)
	{
    	if(strcmp(poids.c_str(),partage.getPoid().c_str())){
    		screen->dispStr(15,15,"je publie!!");
    	mqtt->publish(NULL,"RAM/balanceMel/etats/poids",partage.getPoid().size(),partage.getPoid().c_str());
     	mqtt->publish(NULL,"RAM/shopvac/cmd/poids",partage.getPoid().size(),partage.getPoid().c_str());
     	poids=partage.getPoid();}
    	if(strcmp(tare.c_str(),partage.getTare().c_str())){
    	mqtt->publish(NULL,"RAM/balanceMel/etats/tare",partage.getTare().size(),partage.getTare().c_str());
    	tare=partage.getTare();}
    	if(strcmp(unite.c_str(),partage.getUnite().c_str())){
    	mqtt->publish(NULL,"RAM/balanceMel/etats/unite",partage.getUnite().size(),partage.getUnite().c_str());
    	unite=partage.getUnite();}

    	if(partage.getGoAspirateur()=="GO"){
    		mqtt->publish(NULL,"RAM/shopvac/cmd/manuel",partage.getGoAspirateur().size(),partage.getGoAspirateur().c_str());
    		partage.setGoAspirateur("STOP");
    	}
    	/*else if(partage.getGoAspirateur() == "STOP"){
    		mqtt->publish(NULL,"RAM/shopvac/cmd/manuel",partage.getGoAspirateur().size(),partage.getGoAspirateur().c_str());
    	}*/

	//traitement


    	if(mqtt->loop()!=0)
    		mqtt->reconnect();

	if(thread.destruction == DESTRUCTION_SYNCHRONE)
	    {
	    // point de destruction
	    pthread_testcancel();
	    }

	sleep(2);
	}
    }
