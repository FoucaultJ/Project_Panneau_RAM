/*
 * TPartage.hpp
 *
 *  Created on: 2017-10-03
 *      Author: jfoucault
 */

#ifndef TPARTAGE_HPP_
#define TPARTAGE_HPP_
#include "sem.hpp"
#include "mutex.hpp"
#include <stdint.h>
#include "thread.hpp"
#include <string>
using namespace std;

class TPartage {
private:

	string progression;
	float poids;
	bool go,go2;
	char buffer[3];
	char bufferB[5];
	uint32_t controleOk, controleBad;
	TMutex mutexGetSet, mutexControle;
	TSemaphore sem;

public:
	TPartage();

	string getProgression();
	float getPoids();
	bool getGo();
	bool getGo2();
	void setProgression(string valeur);
	void setPoids(float valeur);
	void setGo(bool valeur);
	void setGo2(bool valeur);

	void takeSem(void);
	void releaseSem(void);
	uint32_t getControleOk(void);
	char * getControleBad(void);
	void protectGetSet(void);
	void unProtectGetSet(void);
	void crementControleOK(void);
	void crementControleBad(void);
	virtual ~TPartage();
};
extern TPartage partage;
#endif /* TPARTAGE_HPP_ */
