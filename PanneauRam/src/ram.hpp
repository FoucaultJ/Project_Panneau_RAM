#ifndef RAM_HPP
#define RAM_HPP

#include "thread.hpp"
#include "mutex.hpp"
#include "sem.hpp"
#include "temps.hpp"

/*
 * 100% = 20ma
 * 50%  = 12ma
 * 0%   = 4ma
 */
#define CourantToPourcent(val) (double)(((val) - 4.0) * 6.25)
#define PourcentToCourant(val) (double)(((val) / 6.25) + 4.0)
#define AjustPourcent(val)    ((val < 0)? 0 : ((val > 100)? 100 : val))
#define CourantToTempGrosBassin(val)  ((7.1151 * ((val) - 4.0)) - 16.0)
#define CourantToTempPetitBassin(val) ((7.1151 * ((val) - 4.0)) - 4.0)
#define TempToCourantPetitBassin(val) ((((val) + 4.0) / 7.1151) + 4.0)

class TCom1;

class TRam : public TThread
    {
    public:
	struct ram_t
	    {
	    bool mode;	// Manuel/Auto
	    bool pompe;
	    bool eauChaude;
	    bool eauFroide;
	    bool debordementGrosBassin;
	    bool debordementPetitBassin;

	    double valveGrosBassin;//ouverture et fermeture des valves.
	    double valvePetitBassin;
	    double valveEauChaude;
	    double valveEauFroide;
	    double niveauGrosBassin;//valeur actuels dans les bassins
	    double niveauPetitBassin;
	    double temperatureGrosBassin;
	    double temperaturePetitBassin;

	    double consigneNiveauGrosBassin;//consigne pour notre automatique
	    double consigneNiveauPetitBassin;
	    double consigneTemperaturePetitBassin;
	    double GBMax;
	    double GBMin;
	    double PBMax;
	    double PBMin;
	    double TpPBMax;
	    double TpPBMin;
	    };

	struct partageRam_t
	    {
	    ram_t status;
	    TMutex lock;
	    TSemaphore synControl;
	    TSemaphore synAlarme;
	    };
    private:
	partageRam_t partageRam;
	TCom1 *com_1;
	TTemps tempsProcess;
    public:
	TRam(const char *name,sharedData_t *shared,int policy,int priority,destruction_t destruction,int noCpu = -1);
	~TRam();

	// tâche acquisition
	void task(void);

	void init(void);

	// Actionneur
	void setPompe(bool onOff);
	void setEauFroide(bool onOff);
	void setEauChaude(bool onOff);
	bool getPompe(void);
	bool getEauFroide(void);
	bool getEauChaude(void);

	void setValveGrosBassin(double ouverturePourcent);
	void setValvePetitBassin(double ouverturePourcent);
	void setValveEauChaude(double ouverturePourcent);
	void setValveEauFroide(double ouverturePourcent);
	double getValveGrosBassin(void);
	double getValvePetitBassin(void);
	double getValveEauChaude(void);
	double getValveEauFroide(void);

	// Capteur
	bool getDebordementGrosBassin(void);
	bool getDebordementPetitBassin(void);
	double getNiveauGrosBassin(void);
	double getNiveauPetitBassin(void);
	double getTemperatureGrosBassin(void);
	double getTemperaturePetitBassin(void);

	// Consigne
	void setMode(bool manAuto);
	bool getMode(void);
	void setConsigneNiveauGrosBassin(double niveauPourcent);
	void setConsigneNiveauPetitBassin(double niveauPourcent);
	void setConsigneTemperaturePetitBassin(double temperature);
	double getConsigneNiveauGrosBassin(void);
	double getConsigneNiveauPetitBassin(void);
	double getConsigneTemperaturePetitBassin(void);
	// Partage
	partageRam_t * getPartageRam(void);
	void setStatusRam(ram_t *status);
	void getStatusRam(ram_t *status);
	//alarmes
	double getGBMax(void);
	void setGBMax(double val);
	double getGBMin(void);
	void setGBMin(double val);
	double getPBMax(void);
	void setPBMax(double val);
	double getPBMin(void);
	void setPBMin(double val);
	double getTpPBMax(void);
	void setTpPBMax(double val);
	double getTpPBMin(void);
	void setTpPBMin(double val);
    };

extern TRam *ram;

#endif //RAM_HPP
