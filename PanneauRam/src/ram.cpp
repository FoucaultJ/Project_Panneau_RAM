#include "ram.hpp"
#include "com_1.hpp"
#include "screen.hpp"

TRam *ram = NULL;

TRam::TRam(const char *name,sharedData_t *shared,int policy,int priority,destruction_t destruction,int noCpu) :
							    TThread(name,shared,policy,priority,destruction,noCpu)
    {
    partageRam.status.mode                   = 1;	// manuel
    partageRam.status.pompe                  = 0;
    partageRam.status.eauChaude              = 0;
    partageRam.status.eauFroide              = 0;
    partageRam.status.debordementGrosBassin  = 1;      // pas de debordement
    partageRam.status.debordementPetitBassin = 1;      // pas de debordement

    partageRam.status.valveGrosBassin        = 0;
    partageRam.status.valvePetitBassin       = 0;
    partageRam.status.valveEauChaude         = 0;
    partageRam.status.valveEauFroide         = 0;
    partageRam.status.niveauGrosBassin       = 0;
    partageRam.status.niveauPetitBassin      = 0;
    partageRam.status.temperatureGrosBassin  = 0;
    partageRam.status.temperaturePetitBassin = 0;

    partageRam.status.consigneNiveauGrosBassin       = 0;
    partageRam.status.consigneNiveauPetitBassin      = 0;
    partageRam.status.consigneTemperaturePetitBassin = 0;
    partageRam.status.GBMax= 95;
    partageRam.status.GBMin= 5;
    partageRam.status.PBMax= 95;
    partageRam.status.PBMin= 5;
    partageRam.status.TpPBMax= 40;
    partageRam.status.TpPBMin= 0;

    // init semaphore pour synchronisation
    partageRam.synControl = 0;
    partageRam.synAlarme = 0;

    com_1 = new TCom1("/dev/ttyUSB0",(void *)&partageRam,priority + 10,TCom::b115200,TCom::pNONE,TCom::dS8,30);
    com_1->start();

    screen->dispStr(1,4,"Acquisition : ");
    }

TRam::~TRam()
    {
    init();

    sleep(1); // attendre avant de detruire le port de communication

    if(com_1)
	delete com_1;
    }

// Tâche acquisition
void TRam::task(void)
    {
    // variable locale
    char strCar[2] = {'-','\0'};
    char bufCmd[10];
    float tempsSleep;
    ram_t status;

    // synchronisation démarrage tâche
    signalStart();

    while(1)
	{
    tempsProcess.startMesure();

	//traitement
	if(strCar[0] == '|')
		strCar[0] = '-';
	else
		strCar[0] = '|';
	screen->dispStr(15,4,strCar);

	getStatusRam(&status);

	// Acquisition
	// Lecture AI (module 1)
	bufCmd[0] = '#';
	bufCmd[1] = '0';
	bufCmd[2] = '1';
	bufCmd[3] = '\x0D';
	com_1->takeCom1();
	com_1->setSignalTimeout(1);
	com_1->sendTx(bufCmd,4);

	if(com_1->waitRepRequest() != 0)
	    {
	    sprintf(bufCmd,"%06d",com_1->getCptTimeout());
	    screen->dispStr(54,3,bufCmd);
	    }
	com_1->releaseCom1();

	// Lecture DI (module 5)
	bufCmd[0] = '@';
	bufCmd[1] = '0';
	bufCmd[2] = '5';
	bufCmd[3] = '\x0D';
	com_1->takeCom1();
	com_1->setSignalTimeout(1);
	com_1->sendTx(bufCmd,4);

	if(com_1->waitRepRequest() != 0)
	    {
	    sprintf(bufCmd,"%06d",com_1->getCptTimeout());
	    screen->dispStr(54,3,bufCmd);
	    }
	com_1->releaseCom1();

	if(status.mode == 1)  // mode manuel
	    {
		//partageRam.synControl.take();
	    // déclencle tâche alarme s'il y a lieu
	    if( (status.debordementGrosBassin == 0) || (status.debordementPetitBassin == 0) )
		{

		partageRam.synAlarme.release();
		init();
		}
	    }
	else // mode auto
	    {
	    if( (status.debordementGrosBassin == 0) || (status.debordementPetitBassin == 0) )
		{
		partageRam.synAlarme.release();
		init();
		}
	    // déclenche tâches alarme et control
	    partageRam.synAlarme.release();
	    partageRam.synControl.release();
	    }

	if(thread.destruction == DESTRUCTION_SYNCHRONE)
	    {
	    // point de destruction
	    pthread_testcancel();
	    }

	tempsProcess.stopMesure();
	tempsSleep = 150000 - tempsProcess.mesure_us();

	if(tempsSleep <= 0)
		tempsSleep = 1000;
	usleep(tempsSleep);   // 150 ms en tenant compte du temps d'exécution de la boucle
	}

    }

void TRam::init(void)
    {
    // numérique
    setPompe(0);

    setEauFroide(0);
    setEauChaude(0);

    // analogique
    setValveGrosBassin(100);   // normalement ouverte
    setValvePetitBassin(0);    // normalement fermee

    setValveEauFroide(100);   // normalement ouverte
    setValveEauChaude(0);     // normalement fermee
    }//en d'Autre mot: les valves et les moteurs sont fermés

// Actionneur
void TRam::setPompe(bool onOff)
    {
    char bufCmd[10];

    // module 4 DO1
    sprintf(bufCmd,"#041%d%02d",0,onOff);
    bufCmd[7] = '\x0D';

    com_1->takeCom1();
    com_1->setSignalTimeout(1);
    com_1->sendTx(bufCmd,8);

    if(com_1->waitRepRequest() != 0)
	{
	sprintf(bufCmd,"%06d",com_1->getCptTimeout());
	screen->dispStr(54,3,bufCmd);
	}
    else
	{
	partageRam.lock.take();
	partageRam.status.pompe = onOff;
	partageRam.lock.release();
	}
    com_1->releaseCom1();
    }

void TRam::setEauFroide(bool onOff)
    {
    char bufCmd[10];

    // module 4 DO3
    sprintf(bufCmd,"#041%d%02d",2,onOff);
    bufCmd[7] = '\x0D';

    com_1->takeCom1();
    com_1->setSignalTimeout(1);
    com_1->sendTx(bufCmd,8);

    if(com_1->waitRepRequest() != 0)
	{
	sprintf(bufCmd,"%06d",com_1->getCptTimeout());
	screen->dispStr(54,3,bufCmd);
	}
    else
	{
	partageRam.lock.take();
	partageRam.status.eauFroide = onOff;
	partageRam.lock.release();
	}
    com_1->releaseCom1();
    }

void TRam::setEauChaude(bool onOff)
    {
    char bufCmd[10];

    // module 4 DO2
    sprintf(bufCmd,"#041%d%02d",1,onOff);
    bufCmd[7] = '\x0D';

    com_1->takeCom1();
    com_1->setSignalTimeout(1);
    com_1->sendTx(bufCmd,8);

    if(com_1->waitRepRequest() != 0)
	{
	sprintf(bufCmd,"%06d",com_1->getCptTimeout());
	screen->dispStr(54,3,bufCmd);
	}
    else
	{
	partageRam.lock.take();
	partageRam.status.eauChaude = onOff;
	partageRam.lock.release();
	}
    com_1->releaseCom1();
    }

bool TRam::getPompe(void)
    {
    bool retour;

    partageRam.lock.take();
    retour =  partageRam.status.pompe;
    partageRam.lock.release();

    return retour;
    }

bool TRam::getEauFroide(void)
    {
    bool retour;

    partageRam.lock.take();
    retour =  partageRam.status.eauFroide;
    partageRam.lock.release();

    return retour;
    }

bool TRam::getEauChaude(void)
    {
    bool retour;

    partageRam.lock.take();
    retour =  partageRam.status.eauChaude;
    partageRam.lock.release();

    return retour;
    }

void TRam::setValveGrosBassin(double ouverturePourcent)
    {
    char bufCmd[15];

    // conversion courant
    ouverturePourcent = PourcentToCourant(ouverturePourcent);
    ouverturePourcent = 24.0 -ouverturePourcent;

    // module 2 AO2
    sprintf(bufCmd,"#%02d%d%+07.3lf",2,1,ouverturePourcent);
    bufCmd[11] = '\x0D';

    com_1->takeCom1();
    com_1->setSignalTimeout(1);
    com_1->sendTx(bufCmd,12);

    if(com_1->waitRepRequest() != 0)
	{
	sprintf(bufCmd,"%06d",com_1->getCptTimeout());
	screen->dispStr(54,3,bufCmd);
	}
    else
	{
	//partageRam.lock.take();
	//partageRam.status.valveGrosBassin = ouverturePourcent;
	//partageRam.lock.release();
	}
    com_1->releaseCom1();
    }
void TRam::setValvePetitBassin(double ouverturePourcent)
    {
    char bufCmd[15];

    // conversion courant
    ouverturePourcent = PourcentToCourant(ouverturePourcent);
    ouverturePourcent = 24.0 -ouverturePourcent;

    // module 2 AO3
    sprintf(bufCmd,"#%02d%d%+07.3lf",2,2,ouverturePourcent);
    bufCmd[11] = '\x0D';

    com_1->takeCom1();
    com_1->setSignalTimeout(1);
    com_1->sendTx(bufCmd,12);

    if(com_1->waitRepRequest() != 0)
	{
	sprintf(bufCmd,"%06d",com_1->getCptTimeout());
	screen->dispStr(54,3,bufCmd);
	}
    else
	{
	//partageRam.lock.take();
	//partageRam.status.valvePetitBassin = ouverturePourcent;
	//partageRam.lock.release();
	}
    com_1->releaseCom1();
    }

void TRam::setValveEauChaude(double ouverturePourcent)
    {
    char bufCmd[15];

    // conversion courant
    ouverturePourcent = PourcentToCourant(ouverturePourcent);
    ouverturePourcent = 24.0 -ouverturePourcent;

    // module 2 AO1
    sprintf(bufCmd,"#%02d%d%+07.3lf",2,0,ouverturePourcent);
    bufCmd[11] = '\x0D';

    com_1->takeCom1();
    com_1->setSignalTimeout(1);
    com_1->sendTx(bufCmd,12);

    if(com_1->waitRepRequest() != 0)
	{
	sprintf(bufCmd,"%06d",com_1->getCptTimeout());
	screen->dispStr(54,3,bufCmd);
	}
    else
	{
	//partageRam.lock.take();
	//partageRam.status.valveEauChaude = ouverturePourcent;
	//partageRam.lock.release();
	}
    com_1->releaseCom1();
    }

void TRam::setValveEauFroide(double ouverturePourcent)
    {
    char bufCmd[15];

    // conversion courant
    ouverturePourcent = PourcentToCourant(ouverturePourcent);//PourcentToCourantValveEF(ouverturePourcent);
    ouverturePourcent = 24.0 -ouverturePourcent;

    // module 2 AO4
    sprintf(bufCmd,"#%02d%d%+07.3lf",2,3,ouverturePourcent);
    bufCmd[11] = '\x0D';

    com_1->takeCom1();
    com_1->setSignalTimeout(1);
    com_1->sendTx(bufCmd,12);

    if(com_1->waitRepRequest() != 0)
	{
	sprintf(bufCmd,"%06d",com_1->getCptTimeout());
	screen->dispStr(54,3,bufCmd);
	}
    else
	{
	//partageRam.lock.take();
	//partageRam.status.valveEauFroide = ouverturePourcent;
	//partageRam.lock.release();
	}
    com_1->releaseCom1();
    }

double TRam::getValveGrosBassin(void)
    {
    double retour;

    partageRam.lock.take();
    retour =  partageRam.status.valveGrosBassin;
    partageRam.lock.release();

    return retour;
    }

double TRam::getValvePetitBassin(void)
    {
    double retour;

    partageRam.lock.take();
    retour =  partageRam.status.valvePetitBassin;
    partageRam.lock.release();

    return retour;
    }

double TRam::getValveEauChaude(void)
    {
    double retour;

    partageRam.lock.take();
    retour =  partageRam.status.valveEauChaude;
    partageRam.lock.release();

    return retour;
    }

double TRam::getValveEauFroide(void)
    {
    double retour;

    partageRam.lock.take();
    retour =  partageRam.status.valveEauFroide;
    partageRam.lock.release();

    return retour;
    }

// Capteur
bool TRam::getDebordementGrosBassin(void)
    {
    bool retour;

    partageRam.lock.take();
    retour =  partageRam.status.debordementGrosBassin;
    partageRam.lock.release();

    return retour;
    }

bool TRam::getDebordementPetitBassin(void)
    {
    bool retour;

    partageRam.lock.take();
    retour =  partageRam.status.debordementPetitBassin;
    partageRam.lock.release();

    return retour;
    }

double TRam::getNiveauGrosBassin(void)
    {
    double retour;

    partageRam.lock.take();
    retour =  partageRam.status.niveauGrosBassin;
    partageRam.lock.release();

    return retour;
    }

double TRam::getNiveauPetitBassin(void)
    {
    double retour;

    partageRam.lock.take();
    retour =  partageRam.status.niveauPetitBassin;
    partageRam.lock.release();

    return retour;
    }

double TRam::getTemperatureGrosBassin(void)
    {
    double retour;

    partageRam.lock.take();
    retour =  partageRam.status.temperatureGrosBassin;
    partageRam.lock.release();

    return retour;
    }

double TRam::getTemperaturePetitBassin(void)
    {
    double retour;

    partageRam.lock.take();
    retour =  partageRam.status.temperaturePetitBassin;
    partageRam.lock.release();

    return retour;
    }

// Consigne
void TRam::setMode(bool manAuto)
    {
    partageRam.lock.take();
    partageRam.status.mode = manAuto;
    partageRam.lock.release();

    setPompe(0);

    setValveGrosBassin(100);   // normalement fermee
    setValvePetitBassin(0);    // normalement ouverte

    setValveEauFroide(100);    // normalement fermee
    setValveEauChaude(0);      // normalement ouverte
    setEauChaude(0);
    if(manAuto == 1)    // manuel
	{
	setEauFroide(0);
	setEauChaude(0);
	}
    }

bool TRam::getMode(void)
    {
    bool retour;

    partageRam.lock.take();
    retour =  partageRam.status.mode;
    partageRam.lock.release();

    return retour;
    }

void TRam::setConsigneNiveauGrosBassin(double niveauPourcent)
    {
    partageRam.lock.take();
    partageRam.status.consigneNiveauGrosBassin = niveauPourcent;
    partageRam.lock.release();
    }

void TRam::setConsigneNiveauPetitBassin(double niveauPourcent)
    {
    partageRam.lock.take();
    partageRam.status.consigneNiveauPetitBassin = niveauPourcent;
    partageRam.lock.release();
    }

void TRam::setConsigneTemperaturePetitBassin(double temperature)
    {
    partageRam.lock.take();
    partageRam.status.consigneTemperaturePetitBassin = temperature;
    partageRam.lock.release();
    }
double TRam::getConsigneNiveauGrosBassin(void){
    double retour;

    partageRam.lock.take();
    retour =  partageRam.status.consigneNiveauGrosBassin;
    partageRam.lock.release();

    return retour;
}
double TRam::getConsigneNiveauPetitBassin(void){
    double retour;

    partageRam.lock.take();
    retour =  partageRam.status.consigneNiveauPetitBassin;
    partageRam.lock.release();

    return retour;
}
double TRam::getConsigneTemperaturePetitBassin(void){
    double retour;

    partageRam.lock.take();
    retour =  partageRam.status.consigneTemperaturePetitBassin;
    partageRam.lock.release();

    return retour;
}
double TRam::getGBMax(void){
	double retour;
	partageRam.lock.take();
	    retour =  partageRam.status.GBMax;
	    partageRam.lock.release();
	    return retour;
}
void TRam::setGBMax(double val){
	partageRam.lock.take();
	    partageRam.status.GBMax = val;
	    partageRam.lock.release();
}
double TRam::getGBMin(void){
	double retour;
	partageRam.lock.take();
	    retour =  partageRam.status.GBMin;
	    partageRam.lock.release();
	    return retour;
}
void TRam::setGBMin(double val){
	partageRam.lock.take();
	    partageRam.status.GBMin = val;
	    partageRam.lock.release();
}
double TRam::getPBMax(void){
	double retour;
	partageRam.lock.take();
	    retour =  partageRam.status.PBMax;
	    partageRam.lock.release();
	    return retour;
}
void TRam::setPBMax(double val){
	partageRam.lock.take();
	    partageRam.status.PBMax = val;
	    partageRam.lock.release();
}
double TRam::getPBMin(void){
	double retour;
	partageRam.lock.take();
	    retour =  partageRam.status.PBMin;
	    partageRam.lock.release();
	    return retour;
}
void TRam::setPBMin(double val){

	partageRam.lock.take();
	    partageRam.status.PBMin = val;
	    partageRam.lock.release();

}
double TRam::getTpPBMax(void){
    double retour;

    partageRam.lock.take();
    retour =  partageRam.status.TpPBMax;
    partageRam.lock.release();
    return retour;
}
void TRam::setTpPBMax(double val){
	partageRam.lock.take();
	    partageRam.status.TpPBMax = val;
	    partageRam.lock.release();
}
double TRam::getTpPBMin(void){
    double retour;

    partageRam.lock.take();
    retour =  partageRam.status.TpPBMin;
    partageRam.lock.release();
    return retour;
}
void TRam::setTpPBMin(double val){
	partageRam.lock.take();
	    partageRam.status.TpPBMin = val;
	    partageRam.lock.release();
}
// Partage
TRam::partageRam_t *TRam::getPartageRam(void)
    {
    return &partageRam;
    }

void TRam::setStatusRam(ram_t *status)
    {
    partageRam.lock.take();
    partageRam.status = *status;
    partageRam.lock.release();
    }

void TRam::getStatusRam(ram_t *status)
    {
    partageRam.lock.take();
    *status = partageRam.status;
    partageRam.lock.release();
    }


