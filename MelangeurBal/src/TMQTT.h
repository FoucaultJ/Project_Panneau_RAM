/*
 * TMQTT.h
 *
 *  Created on: 2017-11-07
 *      Author: jfoucault
 */

#ifndef TMQTT_H_
#define TMQTT_H_
#include <mosquittopp.h>
#include <unordered_map>
#include <functional>
#include <string>
#include "screen.hpp"
#include "TPartage.hpp"
#include "com_1.hpp"
#include "com_melangeur.hpp"
extern TPartage partage;
using namespace std;


class TMQTT : public mosqpp::mosquittopp
{
private:
		char tab[10];
	unordered_map<string,function<void(TMQTT*,string)>>hashtable;
		void mode(string mod);
		void moteurA(string moteur);
		void moteurB(string moteur);
		void moteurC(string moteur);
		void recetteA(string recette);
		void recetteB(string recette);
		void recetteC(string recette);
		void recetteGO(string val);
		void doneAspirateur(string fini);





public:
	TMQTT(const char *id, char*host, int port=1883);
	virtual ~TMQTT();
	void on_connect(int rc);
	void on_message(const struct mosquitto_message *message);
	void on_subcribe(int mid, int qos_count, int * granted_pos);

};

#endif /* TMQTT_H_ */
