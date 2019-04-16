#include "com_1.hpp"
#include "screen.hpp"

TCom1::TCom1(const char *name,sharedData_t *shared,int32_t priority,baudrate_t baudRate,
                                        parity_t parite,dimData_t dimData,int32_t timeoutRxMs) :
                                                      TCom(name,shared,priority,baudRate,parite,dimData,timeoutRxMs)
    {
    screen->dispStr(1,2,name);
    nCaractere =0;
    timeout=0;
    condition=0;
    etape=1;
    }

TCom1::~TCom1()
    {

    }

void TCom1::rxCar(unsigned char car)
    {
		if(etape ==1){
			if(car == 0x02){//vérification du premier caractère
		   		 tab[nCaractere]=car;
		   		 nCaractere++;
		   		 condition =3;
			}
			else {condition =1;}
		}
		else if(etape ==2){//Assembler les données
	   		 tab[nCaractere]=car;
	   		 nCaractere++;
	   		 condition = 2; //reste étape 2
	   		 if(nCaractere == 18)
	   		 {
	   			 nCaractere =0;
	   			 condition =3; //étape ++
	   		 }
		}
		else if(etape ==3){ //checksom
	     	char somme =0;

	     	for(unsigned int i =0; i<18;i++)
	     	{
	     		somme += tab[i];
	     	}
	     	somme = somme & 0b01111111; //sur 7 bit
	     	if(somme != 0){
	     		partage.crementControleBad();
	     	}
	     	condition =3;

	     	partage.setSomme(somme);
		}
		else if(etape ==4){// s'il y a un mouvement ou une surcapacité
			if(tab[2] ==0x68 ||tab[2] ==0x78 || tab[2]==0x7C || tab[2]==0x6C){
				condition =1;
			}
			else{condition =3;}
		}
		else if(etape ==5){//traitement des données

     		//Poids
     		strPoid.assign(tab,4,6);
     		strTare.assign(tab,10,6);
     		if(tab[2]==0x60)
     		{
     			strPoid.insert(5,".");
     			strTare.insert(5,"."); // 1 après la virgule
     			partage.setUnite("Lbs");
     		}
     		else if(tab[2]==0x70){
     			strPoid.insert(5,".");
     			strTare.insert(5,".");// 2 après la virgule
     			partage.setUnite("Kg ");
     		}
     		partage.setPoid(strPoid);
     		partage.setTare(strTare);
     		condition = 1;
		}
		if(condition ==1){etape =1;}
		else if(condition==2){etape = 2;}
		else if(condition==3){etape++;}

    }

void TCom1::rxTimeout(void)
    {
    if(++timeout > 40) // timeout de 1 s
	{
	screen->dispStr(10,11,"Timeout");
	timeout = 0;
	}
    }

