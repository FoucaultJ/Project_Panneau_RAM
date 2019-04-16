#include "task1.hpp"
#include "screen.hpp"
//extern uint32_t startStop;
extern TCom *com2;
TTask1::TTask1(const char *name,sharedData_t *shared,int policy,int priority,destruction_t destruction) :
                                                            TThread(name,shared,policy,priority,destruction)
    {
    screen->dispStr(1,3,"Task 1 :");
    memset(tab,0,sizeof(tab));
    etat =partage.getEtat();
    }

TTask1::~TTask1()
    {
    }

void TTask1::task(void)
    {
    // variable locale
	float poidsB=0,recetA=0,recetB=0,recetC=0,totalB=0,totalC=0;

    // synchronisation démarrage tâche
    signalStart();


        while(1)
    	{
    	//traitement     // je dois envoyer les commande: <MMA> ou <MAA> Dépendant du poid et de la consigne. Celui-ci doit être = pour chaque moteur
        				 // pour pouvoir actionner l'aspirateur à la fin.
        //	partage.releaseSem();
        //	partage.takeSem();
        	if(partage.getMode()=="auto"){
        	sscanf(partage.getPoid().c_str(),"%f",&poidsB);
        	sscanf(partage.getRecetteA().c_str(),"%f",&recetA);
        	sscanf(partage.getRecetteB().c_str(),"%f",&recetB);
        	sscanf(partage.getRecetteC().c_str(),"%f",&recetC);
        	etat=partage.getEtat();
        	switch(etat){
        	case 1://attente attendre dans recette go le start. mettre partage.setETat.
        		break;
        	case 2://moteurA
        		sprintf(tab,"<MM%c>",'A');
        		com2->sendTx(tab,strlen(tab));
        		screen->dispStr(1,20,"Moteur A");
        		partage.setEtat(3);
        		break;
        	case 3://attente echo
        		screen->dispStr(10,20,"EchoA");
        		break;
        	case 4://if poid>=consigneA
        		if(poidsB>=recetA){
        			screen->dispStr(1,21,"Mo");
        		sprintf(tab,"<MA%c>",'A');
        		com2->sendTx(tab,strlen(tab));
        		partage.setEtat(5);
        		}
        		break;
        	case 5://echo
        		screen->dispStr(1,18,"echo MAA");

        		break;
        	case 6:
        		sprintf(tab,"<MM%c>",'B');
        		screen->dispStr(1,21,"Moteur B");
    		    com2->sendTx(tab,strlen(tab));
    		    partage.setEtat(7);
        		break;
        	case 7://attente echo B
        		screen->dispStr(15,21,"EchoB");
        		break;
        	case 8://if poid consigne
        		totalB=recetA+recetB;
        		if(poidsB>=totalB){
        			screen->dispStr(1,22,"Moteur C");
            		sprintf(tab,"<MA%c>",'B');
            		com2->sendTx(tab,strlen(tab));
            		partage.setEtat(9);
        		}
        		break;
        	case 9://echo
        		screen->dispStr(1,18,"echo MAB");
        		break;
        	case 10:
        		sprintf(tab,"<MM%c>",'C');
        		com2->sendTx(tab,strlen(tab));
        		partage.setEtat(11);
        		break;
        	case 11://attente echo
        		screen->dispStr(15,22,"EchoC");
        	   break;
        	case 12://if poid consigne
        		totalC=recetA+recetB+recetC;
        		if(poidsB>=totalC){
         		sprintf(tab,"<MA%c>",'C');
         		com2->sendTx(tab,strlen(tab));
         		 partage.setEtat(13);
        		}
        	   break;
        	case 13:
        		//publish GO
        		screen->dispStr(1,24,"aspirateur");
        		partage.setGoAspirateur("GO");
        		partage.setEtat(14);
        	   break;
        	case 14:
        		break;

//peut-être 2 autre étape pour pouvoir attendre le finish
        		}
        	}



        //	sprintf(buffer,"<P%07.1f>",partage.getPoids());
        	// screen->dispStr(1,20,"ee");


        	// com2->sendTx(buffer,strlen(buffer));


	if(thread.destruction == DESTRUCTION_SYNCHRONE)
	    {
	    // point de destruction
	    pthread_testcancel();
	    }

	usleep(250000);   // 250 ms
	}
    }
