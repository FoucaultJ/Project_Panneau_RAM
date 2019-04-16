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
/*hashtable["RAM/powermeter/"]=&TMQTT::messageModule1; ///poser comme question si nous devons set par le mqtt
hashtable["RAM/powermeter/"]=&TMQTT::messageModule2;
hashtable["RAM/powermeter/"]=&TMQTT::messageModule3;
hashtable["RAM/powermeter/"]=&TMQTT::messageModule4;
hashtable["RAM/powermeter/"]=&TMQTT::messageModule5;
hashtable["RAM/powermeter/"]=&TMQTT::messageModule6;*/
}



TMQTT::~TMQTT() {
	// TODO Auto-generated destructor stub
	mosqpp::lib_cleanup();
}

void TMQTT::on_connect(int rc) {
	screen->dispStr(1,14,"connect!!!");
}

void TMQTT::on_message(const struct mosquitto_message* message) {
	/*string topic = message->topic;
	string payload = (char*)message->payload;
	auto it =hashtable.find(topic);
	if(it!=hashtable.end())
		it->second(this,payload);*/
}

void TMQTT::on_subcribe(int mid, int qos_count, int* granted_pos) {
	screen->dispStr(1,12,"subcribe ok");
}
