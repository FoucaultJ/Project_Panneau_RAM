#include "com_1.hpp"
#include "screen.hpp"

TCom *com1;

TCom1::TCom1(const char *name,sharedData_t *shared,int32_t priority,baudrate_t baudRate,
                                        parity_t parite,dimData_t dimData,int32_t timeoutRxMs) :
                                                      TCom(name,shared,priority,baudRate,parite,dimData,timeoutRxMs)
    {
    screen->dispStr(1,2,name);

    timeout=0;
    demande= 0;
    commande=STX;
    memset(tab,0,sizeof(tab));
    }

TCom1::~TCom1()
    {

    }

void TCom1::rxCar(unsigned char car)
    {

screen->dispStr(1,21,"Je recois");
		switch(commande){
		case STX:

			if(car == '<'){
				commande= CMD;
				tab[0]=car;
			}
			else
				commande = STX;
			break;
		case CMD:
				demande=car;
				tab[1]=car;
				commande= END;
			break;
		case END:
			if(car == '>')
			{
				tab[2]=car;
				screen->dispStr(10,24,tab);
				if(demande =='M'||demande =='P'){
					partage.setProgression("PROCESSING");
					partage.setGo2(1);
					screen->dispStr(1,24,"P ou M est recu");
					//partage.releaseSem();
					//startStop =1;
				}
				if(demande =='A'||demande =='D'){
					partage.setProgression("FINISHED");
					//partage.takeSem();
					partage.setGo2(0);
					//startStop =0;
					if(demande=='D'){
						com1->sendTx(tab,3);
						screen->dispStr(1,22,"J'ai eu done");

					}
				}


			}
			else
				commande = STX;

			commande = STX;
			break;



				}


    }

void TCom1::rxTimeout(void)
    {
    if(++timeout > 40) // timeout de 1 s
	{
	screen->dispStr(10,11,"Timeout");
	partage.releaseSem();
	timeout = 0;
	}
    }
