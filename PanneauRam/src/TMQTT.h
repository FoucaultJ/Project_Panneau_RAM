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
#include "ram.hpp"
#include "com_1.hpp"
//extern TPartage partage;
using namespace std;


class TMQTT : public mosqpp::mosquittopp
{
private:
		char tab[10];
	unordered_map<string,function<void(TMQTT*,string)>>hashtable;
		void mode(string val);
		void consNivGB(string val);
		void consNivPB(string val);
		void consTmpPB(string val);
		void valveGB(string val);
		void valvePB(string val);
		void valveEC(string val);
		void valveEF(string val);
		void valveEEC(string val);
		void valveEEF(string val);
		void pompe(string val);
		void GBMax(string val);
		void GBMin(string val);
		void PBMax(string val);
		void PBMin(string val);
		void TpPBMax(string val);
		void TpPBMin(string val);




public:
	TMQTT(const char *id, char*host, int port=1883);
	virtual ~TMQTT();
	void on_connect(int rc);
	void on_message(const struct mosquitto_message *message);
	void on_subcribe(int mid, int qos_count, int * granted_pos);

};

#endif /* TMQTT_H_ */
