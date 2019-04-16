#include "task1.hpp"
#include "screen.hpp"
#include "com_1.hpp"


TTask1::TTask1(const char *name,sharedData_t *shared,int policy,int priority,destruction_t destruction) :
                                                            TThread(name,shared,policy,priority,destruction)
    {
    screen->dispStr(1,3,"Task 1 :");

    }

TTask1::~TTask1()
    {
    }
//Envoye le poids à la réception d'un m et envoye un m à la réception d'un GO sur mqtt.
void TTask1::task(void)
    {

    // variable locale

    signalStart();

    char buffer[20];
   // char tab[20];
    while(1)
	{
	//traitement     // je dois envoyer les commande: <M> ou <A> ou <P75755757> <D> pour commander l'automate (API)


		if(partage.getGo()==1 && partage.getGo2()==1){

					com1->sendTx((void *)"<M>",3);
				screen->dispStr(1,23,"M est envoyer");
				 partage.setGo(0);
			}
			else if(partage.getGo2()){

			//sprintf(buffer,"<P%07.1f>",test);
			sprintf(buffer,"<P%07.1f>",partage.getPoids());
			 screen->dispStr(1,20,"PIKACHU");
			 screen->dispStr(1,22,buffer);
			 com1->sendTx(buffer,strlen(buffer));
			 partage.setGo2(0);
			// startStop =0;
			}







	if(thread.destruction == DESTRUCTION_SYNCHRONE)
	    {
	    // point de destruction
	    pthread_testcancel();
	    }

	sleep(2);   // 250 ms
	}
    }
