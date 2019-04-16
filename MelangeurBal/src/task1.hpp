#ifndef TTASK1_HPP
#define TTASK1_HPP

#include "thread.hpp"
#include "TPartage.hpp"
#include "com_melangeur.hpp"
class TTask1 : public TThread
    {
    private:
	/*enum etape_t{ //etape = partage.getEtat();
					START,//break
					SENDA,//recetteGO
					Aecho,//attente
					TestA,//if poid consigne
					SENDB,//sendMA sendMM
					Becho,//attente
					TestB,//if poid consigne
					SENDC,//sendMA sendMM
					Cecho,//attente
					TestC,//if poid consigne
					ASP//start aspirateur si finish est arrivée, on change l'état pour start.
				}etape;*/
	int etat;
	char tab[10];

    public:
	TTask1(const char *name,sharedData_t *shared,int policy,int priority,destruction_t destruction);
	~TTask1();


	void task(void);
    };

#endif //TTASK1_HPP
