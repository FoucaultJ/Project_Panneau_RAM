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
hashtable["RAM/shopvac/cmd/manuel"]=&TMQTT::recetteGO;
hashtable["RAM/shopvac/cmd/poids"]=&TMQTT::recettePoids;
	memset(tab,0,sizeof(tab));
}
void TMQTT::recetteGO(string val) {

if(val=="GO"){
	partage.setGo(1);
	sprintf(tab,"<P%07.1f>",partage.getPoids());
	com1->sendTx(tab,strlen(tab));

	/*sprintf(tab,"%d", partage.getGo());
	screen->dispStr(5,22,tab);*/
	//fonctionne?
	/*sprintf(tab,"<%c>",'M');
	com1->sendTx(tab,strlen(tab));
	screen->dispStr(1,20,tab);*/

}
else if(val=="STOP"){
	sprintf(tab,"<%c>",'A');
		com1->sendTx(tab,strlen(tab));

}

}

void TMQTT::recettePoids(string val) {
	//sprintf(tab,"<P%.1f>",val);
	float valfloat;
	sscanf(val.c_str(),"%f",&valfloat);
	partage.setPoids(valfloat);
	//com1->sendTx(tab,8);
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

