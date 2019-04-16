#include "task1.hpp"
#include "screen.hpp"

#include "Elkor.h"
#include "Elkor.c"


TTask1::TTask1(const char *name,sharedData_t *shared,int policy,int priority,destruction_t destruction) :
                                                            TThread(name,shared,policy,priority,destruction)
    {
    screen->dispStr(1,3,"Task 1 :");
    }

TTask1::~TTask1()
    {
    }

void TTask1::task(void)
    {

    // variable locale

   unsigned char sendTab[8]={0x02,0x03,0x03,0x14,0x00,0x02,0x00,0x00};//0x02,0x03,0x03,0x14,0,2,CKEKSOM MSB(haut), ChekSomLSB (bas)             ADD_REG_VOLTAGE_A_N
    //calculerCRC(unsigned char* input, unsigned int len); le tab et la longueur du tableau
    ///Semaphore entre chaque pour permettre la synchronisation avec le rxCar du TCom1
    // synchronisation démarrage tâche
    unsigned char sendTab2[8]={0x02,0x03,0x03,0x16,0x00,0x02,0x00,0x00}; //Vbn
    unsigned char sendTab3[8]={0x02,0x03,0x03,0x1A,0x00,0x02,0x00,0x00}; //Vab
     unsigned char sendTab4[8]={0x02,0x03,0x03,0x20,0x00,0x02,0x00,0x00};  //Ia
      unsigned char sendTab5[8]={0x02,0x03,0x03,0x22,0x00,0x02,0x00,0x00}; //Ib
       unsigned char sendTab6[8]={0x02,0x03,0x03,0x02,0x00,0x02,0x00,0x00}; //traite de puissance
    unsigned char sendTab7[8]={0x02,0x03,0x03,0x38,0x00,0x02,0x00,0x00}; //Facteur
   unsigned char sendTab8[8]={0x02,0x03,0x03,0x00,0x00,0x02,0x00,0x00}; //consomation energie

    signalStart();
    unsigned int ckc =0;
     //char buffer[20];
    while(1)
	{
	//traitement

    //	partage.releaseSem();
    	//traitement du Va-n
    	ckc =0;
    	ckc= calculerCRC(sendTab, 6); //fait le check somme
    	sendTab[6]=ckc >>8;

    	sendTab[7]= ckc & 0x00FF;

    	//sprintf(buffer,"%d",ckc);
    	 screen->dispStr(1,20,"ee");

    	 com1->sendTx(sendTab,8); // envoye au powerMeter

    	partage.setLecture(1);

    	partage.takeSem();
    	usleep(200000);
    	ckc =0;
    	screen->dispStr(1,21,"Vbn");

    	//traitement du Vb-n


    	ckc= calculerCRC(sendTab2, 6); //fait le check somme
    	sendTab2[6]=ckc >>8;

    	sendTab2[7]= ckc & 0x00FF;
    	partage.setLecture(2);

    	com1->sendTx(sendTab2,8); // envoye au powerMeter


    	partage.takeSem();
    	usleep(200000);
    	ckc =0;
    	screen->dispStr(5,21,"Vab  s");
    	//traitement du Va-b

    	//screen->dispStr(1,16,sendTab);

    	ckc= calculerCRC(sendTab3, 6); //fait le check somme
    	sendTab3[6]=ckc >>8;

    	sendTab3[7]= ckc & 0x00FF;

    	com1->sendTx(sendTab3,8); // envoye au powerMeter
    	partage.setLecture(3);

    	partage.takeSem();
    	usleep(200000);
    	ckc =0;
    	screen->dispStr(10,21,"Ia");
    	//traitement du Ia

    	ckc= calculerCRC(sendTab4, 6); //fait le check somme
    	sendTab4[6]=ckc >>8;

    	sendTab4[7]= ckc & 0x00FF;

    	com1->sendTx(sendTab4,8); // envoye au powerMeter
    	partage.setLecture(4);

    	partage.takeSem();
    	usleep(200000);
    	screen->dispStr(15,21,"Ib");
    	//traitement du Ib
    	ckc =0;
    	ckc= calculerCRC(sendTab5, 6); //fait le check somme
    	sendTab5[6]=ckc >>8;

    	sendTab5[7]= ckc & 0x00FF;

    	com1->sendTx(sendTab5,8); // envoye au powerMeter
    	partage.setLecture(5);

    	 partage.takeSem();
    	 usleep(200000);
    	//traitement du puissance
    	 ckc =0;
    	 ckc= calculerCRC(sendTab6, 6); //fait le check somme
    	sendTab6[6]=ckc >>8;

    	sendTab6[7]= ckc & 0x00FF;

    	com1->sendTx(sendTab6,8); // envoye au powerMeter
    	partage.setLecture(6);

    	partage.takeSem();
    	usleep(200000);
    	//traitement du facteur puissance
    	ckc =0;
    	ckc= calculerCRC(sendTab7, 6); //fait le check somme
    	sendTab7[6]=ckc >>8;

    	sendTab7[7]= ckc & 0x00FF;


    	com1->sendTx(sendTab7,8); // envoye au powerMeter

    	partage.setLecture(7);

    	partage.takeSem();
    	usleep(200000);
    	//traitement du consommation energetique
    	ckc =0;
    	ckc= calculerCRC(sendTab8, 6); //fait le check somme
    	sendTab8[6]=ckc >>8;

    	sendTab8[7]= ckc & 0x00FF;


    	com1->sendTx(sendTab8,8); // envoye au powerMeter
    	partage.setLecture(8);

    	partage.takeSem();
    	usleep(200000);


	if(thread.destruction == DESTRUCTION_SYNCHRONE)
	    {
	    // point de destruction
	    pthread_testcancel();
	    }

	//usleep(250000);   // 250 ms
	}
    }
