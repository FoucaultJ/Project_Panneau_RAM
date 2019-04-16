#include "alarme.hpp"
#include "screen.hpp"

TAlarme *alarme = NULL;

TAlarme::TAlarme(const char *name,sharedData_t *shared,int policy,int priority,destruction_t destruction,int noCpu) :
                                                            TThread(name,shared,policy,priority,destruction,noCpu)
    {
    partageRam = ram->getPartageRam();

    screen->dispStr(1,6,"Alarme :");
    }

TAlarme::~TAlarme()
    {
    }

void TAlarme::task(void)
    {
    // variable locale
    char strCar[2] = {'-','\0'};

    // synchronisation démarrage tâche
    signalStart();

    while(1)
		{
		//traitement
		if(strCar[0] == '|')
			strCar[0] = '-';
		else
			strCar[0] = '|';
		screen->dispStr(11,6,strCar);

		// attendre acquisition complété ou débordement détecté
		partageRam->synAlarme.take();
		if(ram->getDebordementGrosBassin()==0 ||ram->getDebordementPetitBassin()==0){
			ram->init();

		}

		if(thread.destruction == DESTRUCTION_SYNCHRONE)
			{
			// point de destruction
			pthread_testcancel();
			}
		}
    }


