//============================================================================
// Name        : TemplateThreadCom.cpp
// Author      : Julie Foucault et SG
// Version     :
// Copyright   : Your copyright notice
// Description : Template Thread Com
//============================================================================

#include "clavier.hpp"
#include "screen.hpp"
#include "task1.hpp"
#include "com_1.hpp"
#include "TPartage.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include "sem.hpp"
#include "TaskMqtt.hpp"

using namespace std;



int main(int argc, char *argv[])
    {
    char car;
    //char bTab[10];

    // Initialisation task Principal
    TThread::initTaskMain();

    // Création Clavier et console
    clavier = new TClavier();
    screen  = new TScreen();
    screen->start();

    // Création tâches
    TTask1 *task1 = new TTask1("Task 1",NULL,SCHED_RR,90,TThread::DESTRUCTION_ASYNCHRONE);
    TaskMqtt *taskmqt = new TaskMqtt("Taskmqtt",NULL,SCHED_RR,85,TThread::DESTRUCTION_ASYNCHRONE);
    com1= new TCom1("/dev/ttyUSB0",NULL,89,TCom::b9600,TCom::pNONE,TCom::dS8,25); // 9600 bauds, parite pair, 7 bits

    // Démarrage tâches
    task1->start();
    taskmqt->start();
    com1->start();

    // Traitement tâche principale
    screen->dispStr(1,1,"Test Com (SG  25/08/2016)");


    do
      {
      // Traitement

    	screen->dispStr(1,7,"Van:");
    	screen->dispStr(10,7,partage.getVan().c_str());
    	screen->dispStr(1,8,"Vbn:");
    	screen->dispStr(10,8,partage.getVbn().c_str());
    	screen->dispStr(1,9,"Vab:");
    	screen->dispStr(10,9,partage.getVab().c_str());
    	screen->dispStr(1,10," Ia:");
    	screen->dispStr(10,10,partage.getIa().c_str());
    	screen->dispStr(1,11," Ib:");
    	screen->dispStr(10,11,partage.getIb().c_str());
    	screen->dispStr(1,12,"puissance(KW):");
    	screen->dispStr(17,12,partage.getPuissance().c_str());
    	screen->dispStr(1,13,"Facteur puissance(%):");
    	screen->dispStr(25,13,partage.getFacPuis().c_str());
    	screen->dispStr(1,14,"consommation(KWh):");
    	screen->dispStr(25,14,partage.getEnergetique().c_str());

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

