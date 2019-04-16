//============================================================================
// Name        : TemplateThreadCom.cpp
// Author      : SG
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
#include "com_melangeur.hpp"
using namespace std;
uint32_t startStop =0;

int main(int argc, char *argv[])
    {
    char car;


    // Initialisation task Principal
    TThread::initTaskMain();

    // Création Clavier et console
    clavier = new TClavier();
    screen  = new TScreen();
    screen->start();


    // Création tâches
    TTask1 *task1 = new TTask1("Task 1",NULL,SCHED_RR,90,TThread::DESTRUCTION_ASYNCHRONE);
    TaskMqtt *taskmqt = new TaskMqtt("Taskmqtt",NULL,SCHED_RR,85,TThread::DESTRUCTION_ASYNCHRONE);
    TCom1   *com1   = new TCom1("/dev/ttyUSB0",NULL,90,TCom::b9600,TCom::pPAIR,TCom::dS7,25); // 115200 bauds, aucune parite, 8 bits
      com2   = new TCom2("/dev/ttyUSB1",NULL,91,TCom::b9600,TCom::pNONE,TCom::dS8,25);
    // Démarrage tâches
    task1->start();
    taskmqt->start();
    com1->start();
    com2->start();
    // Traitement tâche principale
    screen->dispStr(1,1,"Test Com (SG  25/08/2016)");


    do
      {
      // Traitement

      screen->dispStr(1,7,partage.getPoid().c_str());
      screen->dispStr(10,7,partage.getUnite().c_str());
      screen->dispStr(1,8,partage.getTare().c_str());
      screen->dispStr(1,10,partage.getSomme());
      screen->dispStr(1,11,"Message rejeter:");
      screen->dispStr(18,11,partage.getControleBad());
      screen->dispStr(1,15,"moteur A:");
      screen->dispStr(1,16,"moteur B:");
      screen->dispStr(1,17,"moteur C:");
      if(clavier->kbhit())
	  {
	  car = clavier->getch();

	  if( (car != 't') && (car != 'T') )
	      {
	     // com1->setSignalTimeout(true);
	      com1->sendTx(&car,1);
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
    if(com2)
    	delete com2;
    // Destruction Clavier et console
    if(clavier)
	delete clavier;
    if(screen)
	delete screen;

    return 0;
    }
