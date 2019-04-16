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
	 bool m1, m2, m3, m4, m5, m6;
	 string Van,Vbn,Vab, Ia,Ib,puissance,facPuissance,energetique;
	char buffer[3];
	char bufferB[5];
	uint32_t controleOk, controleBad;
	TMutex mutexGetSet, mutexControle;
	TSemaphore sem;
	unsigned char lecture;
public:
	TPartage();
	bool getM1();
	bool getM2();
	bool getM3();
	bool getM4();
	bool getM5();
	bool getM6();
	string getVan();
	string getVbn();
	string getVab();
	string getIa();
	string getIb();
	string getPuissance();
	string getFacPuis();
	string getEnergetique();
	unsigned char getLecture()const;

	void setM1(bool module);
	void setM2(bool module);
	void setM3(bool module);
	void setM4(bool module);
	void setM5(bool module);
	void setM6(bool module);
	void setVan(string valeur);
	void setVbn(string valeur);
	void setVab(string valeur);
	void setIa(string valeur);
	void setIb(string valeur);
	void setPuissance(string valeur);
	void setFactPuissance(string valeur);
	void setEnergetique(string valeur);
	void setLecture(unsigned char lec);

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
