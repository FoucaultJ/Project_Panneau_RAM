//============================================================================
// Name        : RAM
// Author      : SG
// Version     :
// Copyright   : Your copyright notice
// Description : Controleur Panneau RAM
//============================================================================

#include <string>
using namespace std;

#include "clavier.hpp"
#include "screen.hpp"
#include "temps.hpp"
#include "alarme.hpp"
#include "control.hpp"
#include "ram.hpp"
#include "version.hpp"
#include "TaskMqtt.hpp"
#include "PID.hpp"
#include <iostream>
int main(int argc, char *argv[])
    {
    char car;

    uint16_t noCpu = 0;

    // Initialisation task Principal
    TThread::initTaskMain(SCHED_RR,noCpu);

    // Création Clavier,console et horloge
    clavier = new TClavier();

    temps   = new TTemps();
    screen  = new TScreen();
    screen->start();

    screen->dispStr(1,1,"RAM Init...");

    // Création tâches
    noCpu = 1;
    ram     = new TRam("Ram",NULL,SCHED_RR,80,TThread::DESTRUCTION_ASYNCHRONE,noCpu);

    noCpu = 2;
    alarme  = new TAlarme("Alarme",NULL,SCHED_RR,79,TThread::DESTRUCTION_SYNCHRONE,noCpu);
    control = new TControl("Control",NULL,SCHED_RR,78,TThread::DESTRUCTION_SYNCHRONE,noCpu);
    TaskMqtt *taskmqt = new TaskMqtt("Taskmqtt",NULL,SCHED_RR,85,TThread::DESTRUCTION_ASYNCHRONE);

    // Démarrage tâches
    sleep(1);

    alarme->start();
    control->start();
    ram->start();
    taskmqt->start();

    // attendre que les taches soit demarrees
    sleep(1);

    //ram->init();
    char affiche[30];
    // Traitement tâche principale
    string message;
    message = "RAM";
    screen->dispStr(1,1,message.c_str());
    screen->dispStr(1,8,"Pompe:");
    screen->dispStr(1,9,"eauFroide:");
    screen->dispStr(1,10,"eauChaude:");
    screen->dispStr(1,11,"valveGB:");
    screen->dispStr(1,12,"valvePB:");
    screen->dispStr(1,13,"valveEF:");
    screen->dispStr(1,14,"valveEC:");

    screen->dispStr(1,16,"Niveau GB:");
    screen->dispStr(1,17,"Niveau PB:");
    screen->dispStr(1,18,"Tmp PB:");




    screen->dispStr(30,8,"m-Mode:1-man 0-auto");//1-auto 2-manuel
    screen->dispStr(30,9,"a-Consigne GB:");
    screen->dispStr(30,10,"b-Consigne PB:");
    screen->dispStr(30,11,"c-Consigne TempPB:");
    screen->dispStr(30,12,"d-KP Niv GB:");
    screen->dispStr(30,13,"e-KP Niv PB:");
    screen->dispStr(30,14,"f-KP Tmp PB:");
    screen->dispStr(30,15,"g-KI Niv GB:");
    screen->dispStr(30,16,"h-KI Niv PB:");
    screen->dispStr(30,17,"i-KI tmp PB:");
    screen->dispStr(30,20,"Erreur Niv GB:");
    screen->dispStr(30,21,"Erreur Niv PB:");
    screen->dispStr(30,22,"Erreur Tmp PB:");
 double demande;
 bool modes;


    do
      {
    	car =' ';
        sprintf(affiche,"%lf",ram->getNiveauGrosBassin());
         screen->dispStr(15,16,affiche);
         sprintf(affiche,"%lf",ram->getNiveauPetitBassin());
         screen->dispStr(15,17,affiche);
         sprintf(affiche,"%lf",ram->getTemperaturePetitBassin());
          screen->dispStr(15,18,affiche);



      // clavier
      if(clavier->kbhit())
		  {
		  car = clavier->getch();


		  }
      if(car =='m'){
      		     cin >> modes;
      		     ram->setMode(modes);
      		     sprintf(affiche,"%d",ram->getMode());
      		     screen->dispStr(51,8,affiche);
      		    // ram->setEauChaude(0);
      		            }
      		  if(car =='a'){
      		      	  cin >> demande;
      		      	  ram->setConsigneNiveauGrosBassin(demande);
      		      	  sprintf(affiche,"%lf",ram->getConsigneNiveauGrosBassin());
      		      	  screen->dispStr(51,9,affiche);
      		        }
      		 if(car =='b'){
      		      	  cin >> demande;
      		      	    	  ram->setConsigneNiveauPetitBassin(demande);
      		      	    	  sprintf(affiche,"%lf",ram->getConsigneNiveauPetitBassin());
      		      	    	  screen->dispStr(51,10,affiche);
      		              }
      		if(car =='c'){
      		      	  cin >> demande;
      		      	     ram->setConsigneTemperaturePetitBassin(demande);
      		      	     sprintf(affiche,"%lf",ram->getConsigneTemperaturePetitBassin());
      		      	     screen->dispStr(51,11,affiche);
      		              }
      		        if(car =='d'){
      		      	  cin >> demande;
      		      	  control->setKpGB(demande);
      		      	  sprintf(affiche,"%lf",control->getKpGB());
      		      	  screen->dispStr(51,12,affiche);
      		              }
      		  if(car =='e'){
  		      	  cin >> demande;
  		      	  control->setKpPB(demande);
  		      	  sprintf(affiche,"%lf",control->getKpPB());
  		      	  screen->dispStr(51,13,affiche);

      		              }
      		if(car =='f'){
      				cin >> demande;
      				control->setKpTPB(demande);
      				sprintf(affiche,"%lf",control->getKpTPB());
      				screen->dispStr(51,14,affiche);
      		              }
      		        if(car =='g'){
      		        	cin >> demande;
      		        	control->setKiGB(demande);
      		        	sprintf(affiche,"%lf",control->getKiGB());
      		        	screen->dispStr(51,15,affiche);
      		              }
      		      if(car =='h'){
      		            	cin >> demande;
      		            	control->setKiPB(demande);
      		            	sprintf(affiche,"%lf",control->getKiPB());
      		            	screen->dispStr(51,16,affiche);
      		            	}
      		    if(car =='i'){
      		       cin >> demande;
      		       control->setKiTPB(demande);
      		      sprintf(affiche,"%lf",control->getKiTPB());
      		        screen->dispStr(51,17,affiche);
      		        }
      		        if(car =='8'){
      		      	  ram->init();
      		      	  //ram->setEauChaude(0);
      		      	  //ram->setEauFroide(0);
      		        }
      		        if(car =='p'){
      		         ram->setPompe(1);
      		         screen->dispStr(12,8,"ON ");

      		        }
       usleep(1000);
       }
    while( (car != 'q') && (car != 'Q') );

    // Destruction tâches
    if(control)
    	delete control;
    if(alarme)
    	delete alarme;
    if(ram)
    	delete ram;
    if(taskmqt)
   	delete taskmqt;
    // Destruction Clavier,console et horloge
    if(clavier)
    	delete clavier;
    if(temps)
    	delete temps;
    if(screen)
    	delete screen;


    return 0;
    }

