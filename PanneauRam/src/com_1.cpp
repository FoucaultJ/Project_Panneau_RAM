#include "com_1.hpp"
#include "screen.hpp"

TCom1::TCom1(const char *name,void *shared,int priority,baudrate_t baudRate,
                                            parity_t parite,dimData_t dimData,int timeoutRxMs) :
                                                           TCom(name,NULL,priority,baudRate,parite,dimData,timeoutRxMs),
                                                           filtreNiveauGB(3),
                                                           filtreNiveauPB(3),
                                                           filtreTempGB(3),
                                                           filtreTempPB(3)
    {
    partageRam = (TRam::partageRam_t *)shared;

    firstByteReponse = true;
    len = 0;
    timeout = 0;
    cptTimeout = 0;
    memset(buffer,0,sizeof buffer);

    resultReq  = 0;
    repRequest = 0; //init semaphore

    screen->dispStr(1,3,"Com1");
    screen->dispStr(30,3,"(Erreur communication : 000000)");
    }

TCom1::~TCom1()
    {
    }

// valeur de retour (0 -> normal, 1 -> timeout, 2 -> erreur communication)
int TCom1::waitRepRequest(void)
    {
    if(com != -1)
	repRequest.take();
    else
	{
	resultReq = 1; //timeout
	cptTimeout++;
	}

    return resultReq;
    }

unsigned int TCom1::getCptTimeout(void)
    {
    return cptTimeout;
    }

void TCom1::takeCom1(void)
    {
    lockCom.take();
    }

void TCom1::releaseCom1(void)
    {
    lockCom.release();
    }

void TCom1::rxCar(unsigned char car)
    {
    if(firstByteReponse)
		{
		firstByteReponse = false;
		len = 0;
		timeout = 0;
		memset(buffer,0,sizeof buffer);
		}

    buffer[len] = car;
    len++;
    if(len > 255)
    	len = 255;
    }

void TCom1::rxTimeout(void)
    {
   if(len > 0)
	{
	if(buffer[0] != '>')
	    resultReq = 2; // erreur
	else
	    {
		TRam::ram_t statusRam;

		//char bufStr[50];
		//sprintf(bufStr,"%d",len);
		//screen->dispStr(40,41,bufStr);

	    resultReq = 0;

	    if( (len == 58) && (buffer[1] == '+') ) // module AI
			{
	    	double value;

		    //char bufStr[100];
		    //buffer[59] = 0;
		    //sprintf(bufStr,"AI %d %s",len,buffer);
		    //screen->dispStr(40,42,bufStr);

			if(sscanf(&buffer[1],"%lf",&value) == 1)        // AI1
				{
				statusRam.valveEauChaude = value;

				//char bufStr[50];
				//sprintf(bufStr,"AI1 %lf",statusRam.valveEauChaude);
				//screen->dispStr(40,43,bufStr);

				statusRam.valveEauChaude = CourantToPourcent/*CourantToPourcentValveEC*/(statusRam.valveEauChaude);
				statusRam.valveEauChaude = AjustPourcent(statusRam.valveEauChaude);
				}

			if(sscanf(&buffer[8],"%lf",&value) == 1)         // AI2
				{
				statusRam.valveGrosBassin = value;

				//char bufStr[50];
				//sprintf(bufStr,"AI2 %lf",statusRam.valveGrosBassin);
				//screen->dispStr(40,44,bufStr);

				statusRam.valveGrosBassin = CourantToPourcent(statusRam.valveGrosBassin);
				statusRam.valveGrosBassin = AjustPourcent(statusRam.valveGrosBassin);
				}

			if(sscanf(&buffer[15],"%lf",&value) == 1)       // AI3
				{
				statusRam.valvePetitBassin = value;

				//char bufStr[50];
				//sprintf(bufStr,"AI3 %lf",statusRam.valvePetitBassin);
				//screen->dispStr(40,45,bufStr);

				statusRam.valvePetitBassin = CourantToPourcent(statusRam.valvePetitBassin);
				statusRam.valvePetitBassin = AjustPourcent(statusRam.valvePetitBassin);
				}

			if(sscanf(&buffer[22],"%lf",&value) == 1)         // AI4
			{
				statusRam.valveEauFroide = value;

				//char bufStr[50];
				//sprintf(bufStr,"AI4 %lf",statusRam.valveEauFroide);
				//screen->dispStr(40,46,bufStr);

				statusRam.valveEauFroide = CourantToPourcent/*CourantToPourcentValveEF*/(statusRam.valveEauFroide);
				statusRam.valveEauFroide = AjustPourcent(statusRam.valveEauFroide);
			}

			if(sscanf(&buffer[29],"%lf",&value) == 1)         // AI5
				{
				statusRam.niveauGrosBassin = value;

				//char bufStr[50];
				//sprintf(bufStr,"AI5 %lf",statusRam.valveEauFroide);
				//screen->dispStr(40,47,bufStr);

				statusRam.niveauGrosBassin = filtreNiveauGB.filtre(statusRam.niveauGrosBassin);																								// calibration
				statusRam.niveauGrosBassin = CourantToPourcent(statusRam.niveauGrosBassin + 0.1);
				statusRam.niveauGrosBassin = AjustPourcent(statusRam.niveauGrosBassin);
				}

			if(sscanf(&buffer[36],"%lf",&value) == 1)         // AI6
				{
				statusRam.niveauPetitBassin = value;

				//char bufStr[50];
				//sprintf(bufStr,"AI6 %lf",statusRam.niveauPetitBassin);
				//screen->dispStr(40,48,bufStr);

				statusRam.niveauPetitBassin = filtreNiveauPB.filtre(statusRam.niveauPetitBassin);
				statusRam.niveauPetitBassin = CourantToPourcent(statusRam.niveauPetitBassin);
				statusRam.niveauPetitBassin = AjustPourcent(statusRam.niveauPetitBassin);
				}

			if(sscanf(&buffer[43],"%lf",&value) == 1)         // AI7
				{
				statusRam.temperatureGrosBassin = value;

				//char bufStr[50];
				//sprintf(bufStr,"AI7 %lf",statusRam.temperatureGrosBassin);
				//screen->dispStr(40,49,bufStr);

				statusRam.temperatureGrosBassin = filtreTempGB.filtre(statusRam.temperatureGrosBassin);
				statusRam.temperatureGrosBassin = CourantToTempGrosBassin(statusRam.temperatureGrosBassin);
				}

			if(sscanf(&buffer[50],"%lf",&value) == 1)         // AI8
				{
				statusRam.temperaturePetitBassin = value;

				//char bufStr[50];
				//sprintf(bufStr,"AI8 %lf",statusRam.temperaturePetitBassin);
				//screen->dispStr(40,50,bufStr);

				statusRam.temperaturePetitBassin = filtreTempGB.filtre(statusRam.temperaturePetitBassin);
				statusRam.temperaturePetitBassin = CourantToTempGrosBassin(statusRam.temperaturePetitBassin);
				}

			partageRam->lock.take();
			partageRam->status.valveEauChaude = statusRam.valveEauChaude;
			partageRam->status.valveGrosBassin = statusRam.valveGrosBassin;
			partageRam->status.valvePetitBassin = statusRam.valvePetitBassin;
			partageRam->status.valveEauFroide = statusRam.valveEauFroide;
			partageRam->status.niveauGrosBassin = statusRam.niveauGrosBassin;
			partageRam->status.niveauPetitBassin = statusRam.niveauPetitBassin;
			partageRam->status.temperatureGrosBassin = statusRam.temperatureGrosBassin;
			partageRam->status.temperaturePetitBassin = statusRam.temperaturePetitBassin;
			partageRam->lock.release();
			}
	    else if(len == 6) // module DI
			{
		    //screen->dispStr(40,51,"DI");

			unsigned int di;
			sscanf(&buffer[1],"%X",&di);
			statusRam.debordementGrosBassin = di & 0x01;           // DI1
			statusRam.debordementPetitBassin = (di >> 1) & 0x01;   // DI2

			partageRam->lock.take();
			partageRam->status.debordementGrosBassin = statusRam.debordementGrosBassin;
			partageRam->status.debordementPetitBassin = statusRam.debordementPetitBassin;
			partageRam->lock.release();
			}
	    }

	len = 0;
	timeout = 0;
	firstByteReponse = true;
	signalTimeout = false;
	repRequest.release();    // syn end request
	}
    else
		{
		// timeout
		if(++timeout > 1000)  // 10 s
			{
			resultReq = 1;  //timeout
			len = 0;
			timeout = 0;
			cptTimeout++;
			firstByteReponse = true;
			signalTimeout = false;
			repRequest.release();  // syn end request
			}
		}
    }
