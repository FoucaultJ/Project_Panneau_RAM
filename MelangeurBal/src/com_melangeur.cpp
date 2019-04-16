/*
 * com_melangeur.cpp
 *
 *  Created on: 2017-11-30
 *      Author: jfoucault
 */

#include "com_melangeur.hpp"
#include "screen.hpp"
TCom *com2;
TCom2::TCom2(const char *name,sharedData_t *shared,int32_t priority,baudrate_t baudRate,
                                        parity_t parite,dimData_t dimData,int32_t timeoutRxMs) :
                                                      TCom(name,shared,priority,baudRate,parite,dimData,timeoutRxMs)
    {
    screen->dispStr(1,2,name);
   commande=0;
   moteur=0;
    timeout=0;
    etat =STX;
    }

TCom2::~TCom2()
    {

    }

void TCom2::rxCar(unsigned char car)
    {

	char buffer[10];
	sprintf(buffer,"%c",car);
	switch(etat){
		case STX:

			screen->dispStr(15,19,buffer);
			if(car == '<'){
				etat= CMD1;
			}
			else
				etat = STX;
			break;
		case CMD1:
			screen->dispStr(16,19,buffer);
				if(car =='M')
				 etat= CMD2;
				else
					etat=STX;
			break;
		case CMD2:
			screen->dispStr(17,19,buffer);
			if(car =='M'|| car =='A'){
				commande = car;
				etat= MOT;
			}
			else
				etat=STX;
			break;
		case MOT:
			screen->dispStr(18,19,buffer);
			if(car =='A'|| car =='B'|| car =='C'){
				moteur = car;
				etat= ETX;
			}
			else
			etat=STX;
			break;
		case ETX:
			screen->dispStr(19,19,buffer);
			if(car == '>')
			{
				if(partage.getMode()=="auto" && partage.getRecetteGo()== "ON"){
				char buffer[10];
				sprintf(buffer,"%c",moteur);
				screen->dispStr(10,19,buffer);
				if(commande =='M'){
				if(moteur =='A'){partage.setEtat(4);
				}
				else if(moteur =='B'){partage.setEtat(8);}
				else if(moteur =='C'){partage.setEtat(12);}
				}
				if(commande =='A'){
					if(moteur =='A'){partage.setEtat(6);}
					else if(moteur =='B'){partage.setEtat(10);}
				}
				}

			}
			else
				etat = STX;

			etat = STX;
			break;




	}
}
void TCom2::rxTimeout(void)
    {
    if(++timeout > 40) // timeout de 1 s
	{
	screen->dispStr(10,11,"Timeout");
	timeout = 0;
	}
    }
