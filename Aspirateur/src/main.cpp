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

#include "version.hpp"
#include "TaskMqtt.hpp"
#include "PID.hpp"
#include <iostream>
#include "task1.hpp"
#include "com_1.hpp"
#include "TPartage.hpp"
#include <fstream>

int main(int argc, char *argv[])
    {
    char car;
    char bTab[12];
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
   // ram     = new TRam("Ram",NULL,SCHED_RR,80,TThread::DESTRUCTION_ASYNCHRONE,noCpu);

    noCpu = 2;

    TaskMqtt *taskmqt = new TaskMqtt("Taskmqtt",NULL,SCHED_RR,85,TThread::DESTRUCTION_ASYNCHRONE);
    TTask1 *task1 = new TTask1("Task 1",NULL,SCHED_RR,90,TThread::DESTRUCTION_ASYNCHRONE);
    com1= new TCom1("/dev/ttyUSB0",NULL,89,TCom::b9600,TCom::pNONE,TCom::dS8);

    // Démarrage tâches

    taskmqt->start();
    task1->start();
    com1->start();
    // attendre que les taches soit demarees

    // Traitement tâche principale


    do
          {
          // Traitement

        	screen->dispStr(1,7,"Process: ");
        	screen->dispStr(11,7,partage.getProgression().c_str());
        	screen->dispStr(1,8,"Poids: ");
        	sprintf(bTab,"<P%07.1f>",partage.getPoids());
        	screen->dispStr(11,8,bTab);

          if(clavier->kbhit())
    	  {
    	  car = clavier->getch();

    	  if( (car != 't') && (car != 'T') )
    	      {

    	      }
    	  }
          }
        while( (car != 'q') && (car != 'Q') );

        // Destruction tâches
        if(task1)
    	delete task1;
        if(taskmqt)
    	delete taskmqt;
        if(com1)
    	delete com1;


        // Destruction Clavier et console
        if(clavier)
    	delete clavier;
        if(screen)
    	delete screen;

        return 0;
        }

