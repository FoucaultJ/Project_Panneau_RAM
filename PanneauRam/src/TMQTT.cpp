/*
 * TMQTT.cpp
 *
 *  Created on: 2017-11-07
 *      Author: jfoucault
 */

#include "TMQTT.h"

TMQTT::TMQTT(const char *id, char *host, int port):mosqpp::mosquittopp(id) {
	// TODO Auto-generated constructor stub
mosqpp::lib_init();
connect(host,port,120);
hashtable["RAM/panneau/cmd/Mode"]=&TMQTT::mode;
hashtable["RAM/panneau/cmd/ConsNivGB"]=&TMQTT::consNivGB;
hashtable["RAM/panneau/cmd/ConsNivPB"]=&TMQTT::consNivPB;
hashtable["RAM/panneau/cmd/ConsTmpPB"]=&TMQTT::consTmpPB;
hashtable["RAM/panneau/cmd/ValveGB"]=&TMQTT::valveGB;
hashtable["RAM/panneau/cmd/ValvePB"]=&TMQTT::valvePB;
hashtable["RAM/panneau/cmd/ValveEC"]=&TMQTT::valveEC;
hashtable["RAM/panneau/cmd/ValveEF"]=&TMQTT::valveEF;
hashtable["RAM/panneau/cmd/ValveEEC"]=&TMQTT::valveEEC;
hashtable["RAM/panneau/cmd/ValveEEF"]=&TMQTT::valveEEF;
hashtable["RAM/panneau/cmd/Pompe"]=&TMQTT::pompe;
hashtable["RAM/alarmes/cmd/NivLhGB"]=&TMQTT::GBMax;
hashtable["RAM/alarmes/cmd/NivLbGB"]=&TMQTT::GBMin;
hashtable["RAM/alarmes/cmd/NivLhPB"]=&TMQTT::PBMax;
hashtable["RAM/alarmes/cmd/NivLbPB"]=&TMQTT::PBMin;
hashtable["RAM/alarmes/cmd/TempLhPB"]=&TMQTT::TpPBMax;
hashtable["RAM/alarmes/cmd/TempLbPB"]=&TMQTT::TpPBMin;

	memset(tab,0,sizeof(tab));
}

void TMQTT::mode(string val){
	if(val =="manuel"){
		ram->setMode(1);
		screen->dispStr(51,8,"1");
	}
	else if(val =="auto"){
		ram->setMode(0);
		screen->dispStr(51,8,"0");
	}

}
void TMQTT::consNivGB(string val){ //normalement ouvert(inverse)
	double result=stod(val);
	if(result<=100 && result>=0){
		ram->setConsigneNiveauGrosBassin(result);
	}
}
void TMQTT::consNivPB(string val){
	double result=stod(val);
	if(result<=100 && result>=0){
		ram->setConsigneNiveauPetitBassin(result);
	}
}
void TMQTT::consTmpPB(string val){
	double result=stod(val);
	if(result<=100 && result>=0){
		ram->setConsigneTemperaturePetitBassin(result);
	}
}
void TMQTT::valveGB(string val){
	double result=stod(val);
	if(result<=100 && result>=0){
		result=100-result;
		ram->setValveGrosBassin(result);
	}
}
void TMQTT::valvePB(string val){
	double result=stod(val);
	if(result<=100 && result>=0){
		ram->setValvePetitBassin(result);
	}
}
void TMQTT::valveEC(string val){
	double result=stod(val);
	if(result<=100 && result>=0){
		ram->setValveEauChaude(result);
	}
}
void TMQTT::valveEF(string val){
	double result=stod(val);
	if(result<=100 && result>=0){
		result=100-result;
		ram->setValveEauFroide(result);
	}
}
void TMQTT::valveEEC(string val){
	if(val =="ON")
	ram->setEauChaude(1);
	else if(val=="OFF")
		ram->setEauChaude(0);
}
void TMQTT::valveEEF(string val){
	if(val =="ON")
	ram->setEauFroide(1);
	else if(val=="OFF")
		ram->setEauFroide(0);
}
void TMQTT::pompe(string val){
	if(val =="ON")
	ram->setPompe(1);
	else if(val=="OFF")
		ram->setPompe(0);
}
void TMQTT::GBMax(string val){
	double result=stod(val);
	char buffer[10];
		if(result<=100 && result>=0){
			ram->setGBMax(result);
			sprintf(buffer,"%lf",result);
			screen->dispStr(1,60,buffer);
		}
}
void TMQTT::GBMin(string val){
	double result=stod(val);
	char buffer[10];
		if(result<=100 && result>=0){
			ram->setGBMin(result);
			sprintf(buffer,"%lf",result);
			screen->dispStr(1,61,buffer);
		}
}
void TMQTT::PBMax(string val){
	double result=stod(val);
	char buffer[10];
		if(result<=100 && result>=0){
			ram->setPBMax(result);
			ram->setGBMin(result);
			sprintf(buffer,"%lf",result);
			screen->dispStr(1,62,buffer);
		}
}
void TMQTT::PBMin(string val){
	double result=stod(val);
		if(result<=100 && result>=0){
			ram->setPBMin(result);
		}
}
void TMQTT::TpPBMax(string val){
	double result=stod(val);
		if(result<=100 && result>=0){
			ram->setTpPBMax(result);
		}
}
void TMQTT::TpPBMin(string val){
	double result=stod(val);
		if(result<=100 && result>=0){
			ram->setTpPBMin(result);
		}
}

TMQTT::~TMQTT() {
	// TODO Auto-generated destructor stub
	mosqpp::lib_cleanup();
}

void TMQTT::on_connect(int rc) {
	screen->dispStr(1,14,"connect!!!");
}

void TMQTT::on_message(const struct mosquitto_message* message) {
	string topic = message->topic;
	string payload = (char*)message->payload;
	auto it =hashtable.find(topic);
	if(it!=hashtable.end())
		it->second(this,payload);
}

void TMQTT::on_subcribe(int mid, int qos_count, int* granted_pos) {
	screen->dispStr(1,12,"subcribe ok");
}
