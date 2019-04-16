#include "control.hpp"
#include "screen.hpp"

TControl *control = NULL;

TControl::TControl(const char *name,sharedData_t *shared,int policy,int priority,destruction_t destruction,int noCpu) :
                                                                            TThread(name,shared,policy,priority,destruction,noCpu)
    {
    partageRam = ram->getPartageRam();

    screen->dispStr(1,5,"Control :");
    }

TControl::~TControl()
    {
    }
double TControl::getKpGB() const{
	return pidGB.getKp();
}
void TControl::setKpGB(double val){
	pidGB.setKp(val);
}
double TControl::getKiGB() const{
	return pidGB.getKi();
}
void TControl::setKiGB(double val){
	pidGB.setKi(val);
}
double TControl::getKpPB() const{
	return pidPB.getKp();
}
void TControl::setKpPB(double val){
	pidPB.setKp(val);
}
double TControl::getKiPB() const{
	return pidPB.getKi();
}
void TControl::setKiPB(double val){
	pidPB.setKi(val);
}
double TControl::getKpTPB() const{
	return pidTPB.getKp();
}
void TControl::setKpTPB(double val){
	pidTPB.setKp(val);
}
double TControl::getKiTPB() const{
	return pidTPB.getKi();
}
void TControl::setKiTPB(double val){
	pidTPB.setKi(val);
}

void TControl::task(void)
    {
    // variable locale
    char strCar[2] = {'-','\0'};
    char affiche[20],afficher[20];
   double ouverture,ouvPB, ouvTPB;
    // synchronisation démarrage tâche
    signalStart();
    setKpGB(7.2);
    setKiGB(0.1);
    setKpPB(1.35);
    setKiPB(0.025);
    setKpTPB(10);
    setKiTPB(0.1);

    while(1)
		{
		//traitement
		if(strCar[0] == '|')
			strCar[0] = '-';
		else
			strCar[0] = '|';
		screen->dispStr(11,5,strCar);

		// attendre acquisition complété
		partageRam->synControl.take();
		if(partageRam->status.mode==0){
			if(partageRam->status.pompe==0 || partageRam->status.mode==1){
				pidGB.resetSommation();
				pidPB.resetSommation();
				pidTPB.resetSommation();
			}
	//faire mon code de PID / objet de pid methode calcule de pid  auto
			ouverture=pidGB.calculePid(ram->getNiveauGrosBassin(),ram->getConsigneNiveauGrosBassin());
			ouverture =100-ouverture;
			ram->setValveGrosBassin(ouverture);
			sprintf(affiche,"%lf",ouverture);
			screen->dispStr(51,20,affiche);
			sprintf(affiche,"%lf",pidGB.getSommation());
			screen->dispStr(52,1,affiche);
			//pid Petit bassin
			ouvPB=pidPB.calculePidPetitB(ram->getNiveauPetitBassin(),ram->getConsigneNiveauPetitBassin());
			ram->setValvePetitBassin(ouvPB);
			sprintf(afficher,"%lf",ouvPB);
			screen->dispStr(51,21,afficher);
			sprintf(affiche,"%lf",pidPB.getSommation());
			screen->dispStr(52,2,affiche);
			//Pid temperature chaude
			ouvTPB=pidTPB.calculePidTemp(ram->getTemperaturePetitBassin(),ram->getConsigneTemperaturePetitBassin());
			sprintf(afficher,"%lf",ouvTPB);
			screen->dispStr(51,22,afficher);
			if(ouvTPB<0){
			ouvTPB =100+ouvTPB;
			if(ouvTPB>100){
				ouvTPB=100;
			}
			else if(ouvTPB <0){
				ouvTPB=0;
			}
			ram->setValveEauFroide(ouvTPB);
			ram->setValveEauChaude(0);
			ram->setEauFroide(1);
			ram->setEauChaude(0);
			}
			else if(ouvTPB>0){
				if(ouvTPB>100){
					ouvTPB=100;
				}
				else if(ouvTPB <0){
					ouvTPB=0;
				}
			ram->setValveEauFroide(0);
			ram->setValveEauChaude(ouvTPB);
			ram->setEauFroide(0);
			ram->setEauChaude(1);
			}

		}

		if(thread.destruction == DESTRUCTION_SYNCHRONE)
			{
			// point de destruction
			pthread_testcancel();
			}
		usleep(100000);
		}
    }
