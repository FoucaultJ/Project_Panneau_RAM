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
	mqtt = new TMQTT((char*)"3",(char*)"172.17.50.103");
}

TaskMqtt::~TaskMqtt() {
	// TODO Auto-generated destructor stub
	delete mqtt;
}

void TaskMqtt::task(void)
    {
    // variable locale
	int mid =69;
	 char buffer[20];
	 string pompe,froid,chaud,overFloG="ALR_GB_OVF",overFloP="ALR_PB_OVF";
	 string nivMaxG="ALR_GB_NIV_MAX",nivMaxP="ALR_PB_NIV_MAX",nivMinG="ALR_GB_NIV_MIN",nivMinP="ALR_PB_NIV_MIN";
	 string tmpMax="ALR_PB_TMP_MAX", tmpMin="ALR_PB_TMP_MIN";
    // synchronisation démarrage tâche
    signalStart();

   mqtt->subscribe(&mid,"RAM/panneau/cmd/#",0);
   mqtt->subscribe(&mid,"RAM/alarmes/cmd/#",0);
    while(1)
	{
    	sprintf(buffer,"%lf",ram->getNiveauGrosBassin());
    	mqtt->publish(NULL,"RAM/panneau/etats/NivGB",strlen(buffer),buffer);

    	sprintf(buffer,"%lf",ram->getNiveauPetitBassin());

    	mqtt->publish(NULL,"RAM/panneau/etats/NivPB",strlen(buffer),buffer);

    	sprintf(buffer,"%lf",ram->getTemperaturePetitBassin());
    	mqtt->publish(NULL,"RAM/panneau/etats/TmpPB",strlen(buffer),buffer);

    	sprintf(buffer,"%lf",100-ram->getValveGrosBassin());
    	mqtt->publish(NULL,"RAM/panneau/etats/ValveGB",strlen(buffer),buffer);

    	sprintf(buffer,"%lf",ram->getValvePetitBassin());
    	mqtt->publish(NULL,"RAM/panneau/etats/ValvePB",strlen(buffer),buffer);

    	sprintf(buffer,"%lf",ram->getValveEauChaude());
    	mqtt->publish(NULL,"RAM/panneau/etats/ValveEC",strlen(buffer),buffer);

    	sprintf(buffer,"%lf",100-ram->getValveEauFroide());
    	mqtt->publish(NULL,"RAM/panneau/etats/ValveEF",strlen(buffer),buffer);

    	if(ram->getEauChaude()==1){
    		chaud ="ON";
    	}
    	else if(ram->getEauChaude()==0){
    		chaud ="OFF";
    	}
    	if(ram->getEauFroide()==1){
    	    froid ="ON";
    	}
    	else if(ram->getEauFroide()==0){
    	   froid ="OFF";
    	 }
    	if(ram->getPompe()==1){
    	    pompe ="ON";
    	 }
    	 else if(ram->getPompe()==0){
    	    pompe ="OFF";
    	 }
    	mqtt->publish(NULL,"RAM/panneau/etats/ValveEEC",chaud.size(),chaud.c_str());

    	mqtt->publish(NULL,"RAM/panneau/etats/ValveEEF",froid.size(),froid.c_str());

    	mqtt->publish(NULL,"RAM/panneau/etats/Pompe",pompe.size(),pompe.c_str());

	//traitement
    	if(ram->getDebordementGrosBassin()==0){
    		mqtt->publish(NULL,"RAM/alarmes/etats",overFloG.size(),overFloG.c_str());
    	}
    	if(ram->getDebordementPetitBassin()==0){
    		mqtt->publish(NULL,"RAM/alarmes/etats",overFloP.size(),overFloP.c_str());
    	}
    	if(ram->getPompe()==1){
    		if(ram->getConsigneNiveauGrosBassin()>5){
				if(ram->getNiveauGrosBassin()>=ram->getGBMax()){
					mqtt->publish(NULL,"RAM/alarmes/etats",nivMaxG.size(),nivMaxG.c_str());
				}
				if(ram->getNiveauGrosBassin()<=ram->getGBMin()){
					mqtt->publish(NULL,"RAM/alarmes/etats",nivMinG.size(),nivMinG.c_str());
				}
    		}
    		if(ram->getConsigneNiveauPetitBassin()>5){
				if(ram->getNiveauPetitBassin()>=ram->getPBMax()){
					mqtt->publish(NULL,"RAM/alarmes/etats",nivMaxP.size(),nivMaxP.c_str());
				}
				if(ram->getNiveauPetitBassin()<=ram->getPBMin()){
					mqtt->publish(NULL,"RAM/alarmes/etats",nivMinP.size(),nivMinP.c_str());
				}
    		}
    		if(ram->getConsigneTemperaturePetitBassin()>0){
				if(ram->getTemperaturePetitBassin()<=ram->getTpPBMax()){
					mqtt->publish(NULL,"RAM/alarmes/etats",tmpMin.size(),tmpMin.c_str());
				}
				if(ram->getTemperaturePetitBassin()>=ram->getTpPBMin()){
							mqtt->publish(NULL,"RAM/alarmes/etats",tmpMax.size(),tmpMax.c_str());
				}
    		}
    	}

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
