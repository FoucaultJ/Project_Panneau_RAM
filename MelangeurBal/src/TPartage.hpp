/*
 * TPartage.hpp
 *
 *  Created on: 2017-10-03
 *      Author: jfoucault
 */

#ifndef TPARTAGE_HPP_
#define TPARTAGE_HPP_

#include "mutex.hpp"
#include <stdint.h>
#include "thread.hpp"
#include <string>
using namespace std;

class TPartage {
private:

	string poid, tare,unite,mode,moteurA,moteurB,moteurC,recetteA,recetteB,recetteC,recetteGo,aspirateur;
	int etat;
	char somme;
	char buffer[3];
	char bufferB[5];
	uint32_t controleOk, controleBad;
	TMutex mutexGetSet, mutexControle;
	TSemaphore sem;

	//	etape_t nEtape;
public:
	TPartage();
	string getPoid();
	string getTare();
	string getUnite();
	char* getSomme();
	void setPoid(string pTab);
	void setTare(string pTab);
	void setUnite(string pTab);
	void setSomme(char ptab);

	string getMode() const;
	string getMoteurA() const;
	string getMoteurB() const;
	string getMoteurC() const;
	string getRecetteA() const;
	string getRecetteB() const;
	string getRecetteC() const;
	string getRecetteGo() const;
	string getGoAspirateur() const;
	int getEtat() const;
	void setMode(string mod);
	void setMoteurA(string mot);
	void setMoteurB(string mot);
	void setMoteurC(string mot);
	void setRecetteA(string r);
	void setRecetteB(string r);
	void setRecetteC(string r);
	void setRecetteGo(string r);
	void setGoAspirateur(string go);
	void setEtat(int etats);

	void takeSem(void);
	void releaseSem(void);
	uint32_t getControleOk(void);
	char * getControleBad(void);
	void protectGetSet(void);
	void unProtectGetSet(void);
	void crementControleOK(void);
	void crementControleBad(void);
	virtual ~TPartage();
/*	etape_t getEtape() const {
			return nEtape;
		}

	void setEtape(etape_t etape) {
			nEtape = etape;
		}*/

};
extern TPartage partage;
#endif /* TPARTAGE_HPP_ */
