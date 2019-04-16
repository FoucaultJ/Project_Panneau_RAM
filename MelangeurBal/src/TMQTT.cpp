/*
 * TMQTT.cpp
 *
 *  Created on: 2017-11-07
 *      Author: jfoucault
 */

#include "TMQTT.h"
extern TCom *com2;
TMQTT::TMQTT(const char *id, char *host, int port):mosqpp::mosquittopp(id) {
	// TODO Auto-generated constructor stub
mosqpp::lib_init();
connect(host,port,120);
hashtable["RAM/balanceMel/cmd/mode"]=&TMQTT::mode;
hashtable["RAM/balanceMel/cmd/motA"]=&TMQTT::moteurA;
hashtable["RAM/balanceMel/cmd/motB"]=&TMQTT::moteurB;
hashtable["RAM/balanceMel/cmd/motC"]=&TMQTT::moteurC;
hashtable["RAM/balanceMel/cmd/recetteA"]=&TMQTT::recetteA;
hashtable["RAM/balanceMel/cmd/recetteB"]=&TMQTT::recetteB;
hashtable["RAM/balanceMel/cmd/recetteC"]=&TMQTT::recetteC;
hashtable["RAM/balanceMel/cmd/recetteGo"]=&TMQTT::recetteGO;
hashtable["RAM/shopvac/etats"]=&TMQTT::doneAspirateur;
	memset(tab,0,sizeof(tab));
}
void TMQTT::mode(string mod) {

	partage.setMode(mod);
	if(mod =="auto"){
		sprintf(tab,"<MA%c>",'A');
		com2->sendTx(tab,strlen(tab));
		sprintf(tab,"<MA%c>",'B');
		com2->sendTx(tab,strlen(tab));
		sprintf(tab,"<MA%c>",'C');
		com2->sendTx(tab,strlen(tab));
		partage.setEtat(1);
	}
	else if(mod =="manuel"){
		partage.setRecetteGo("OFF");
		partage.setEtat(1);
	}

}
void TMQTT::moteurA(string moteur) {
	if(moteur=="ON"){
	//moteur A en action
		sprintf(tab,"<MM%c>",'A');
		com2->sendTx(tab,strlen(tab));
		screen->dispStr(10,15,tab);
		}
	else if(moteur =="OFF"){
		//moteur A en arret
		sprintf(tab,"<MA%c>",'A');
		com2->sendTx(tab,strlen(tab));
		screen->dispStr(10,15,tab);
	}
}
void TMQTT::moteurB(string moteur) {
	if(moteur=="ON"){
	//moteur B en action
		sprintf(tab,"<MM%c>",'B');
		com2->sendTx(tab,strlen(tab));
		screen->dispStr(10,16,tab);
		}
	else if(moteur =="OFF"){
		//moteur B en arret
		sprintf(tab,"<MA%c>",'B');
		com2->sendTx(tab,strlen(tab));
		screen->dispStr(10,16,tab);
	}
}
void TMQTT::moteurC(string moteur) {
	if(moteur=="ON"){
	//moteur C en action
		sprintf(tab,"<MM%c>",'C');
		com2->sendTx(tab,strlen(tab));
		screen->dispStr(10,17,tab);
		}
	else if(moteur =="OFF"){
		//moteur C en arret
		sprintf(tab,"<MA%c>",'C');
		com2->sendTx(tab,strlen(tab));
		screen->dispStr(10,17,tab);
	}
}
void TMQTT::recetteA(string recette) {
	partage.setRecetteA(recette);
}
void TMQTT::recetteB(string recette) {
	partage.setRecetteB(recette);

}
void TMQTT::recetteC(string recette) {
	partage.setRecetteC(recette);
}
void TMQTT::recetteGO(string val) {
	if(val =="ON"){
		screen->dispStr(1,16,"mqtt recu go");
	partage.setRecetteGo(val);
	partage.setEtat(2);
	}
	else if(val == "OFF"){
		partage.setRecetteGo(val);
		partage.setEtat(1);
		sprintf(tab,"<MA%c>",'A');
		com2->sendTx(tab,strlen(tab));
		sprintf(tab,"<MA%c>",'B');
		com2->sendTx(tab,strlen(tab));
		sprintf(tab,"<MA%c>",'C');
		com2->sendTx(tab,strlen(tab));
	}
}
void TMQTT::doneAspirateur(string fini){
	if(fini=="FINISHED"){
		partage.setEtat(1);
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
