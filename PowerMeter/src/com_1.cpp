#include "com_1.hpp"
#include "screen.hpp"
//#include "Elkor.h"
//#include "Elkor.c"
TCom *com1;
TCom1::TCom1(const char *name,sharedData_t *shared,int32_t priority,baudrate_t baudRate,
                                        parity_t parite,dimData_t dimData,int32_t timeoutRxMs) :
                                                      TCom(name,shared,priority,baudRate,parite,dimData,timeoutRxMs)
    {
    screen->dispStr(1,2,name);

    timeout=0;

    etape=STX;
    memset(tab,0,sizeof(tab));
    }

TCom1::~TCom1()
    {

    }

void TCom1::rxCar(unsigned char car)
    {


		switch(etape){
		case STX:

			if(car == 0x02){
				tab[0]=car;
				etape= FXC;
			}
			else
				etape = STX;
			break;
		case FXC:
				tab[1]=car;
				etape= NBR;
			break;
		case NBR:
			tab[2]=car;
			etape =Data1;
			break;
		case Data1:
			tab[3]=car;
			etape =Data2;
			break;
		case Data2:
			tab[4]=car;
			etape =Data3;
			break;
		case Data3:
			tab[5]=car;
			etape =Data4;
			break;
		case Data4:
			tab[6]=car;
			etape =CRC1;
			break;
		case CRC1:
			tab[7]=car;
			etape =CRC2;
			break;
		case CRC2:
			tab[8]=car;

			unsigned short tmpCks =0;
			tmpCks= ((short)tab[7])<<8 |tab[8];

			if(calculerCRC(tab,7)== tmpCks){
				float val;
				char bufferVal[20];
				screen->dispStr(10,32,"etape4");
				unsigned char* pVal= (unsigned char*) &val;
				pVal[0]= tab[6];
				pVal[1]= tab[5];
				pVal[2]= tab[4];
				pVal[3]= tab[3];


				sprintf(bufferVal,"%.2f",val);

				 if(partage.getLecture() ==1){

				    partage.setVan(bufferVal);

				   }
				  else if(partage.getLecture()  == 2){

				     partage.setVbn(bufferVal);}
				  else if(partage.getLecture()  == 3){

				      partage.setVab(bufferVal);}
				  else if(partage.getLecture()  == 4){

				     partage.setIa(bufferVal);}
				  else if(partage.getLecture()  == 5){

				     partage.setIb(bufferVal);}
				  else if(partage.getLecture()  == 6){

				     partage.setPuissance(bufferVal);}
				  else if(partage.getLecture()  == 7){

				     partage.setFactPuissance(bufferVal);}
				  else if(partage.getLecture()  == 8){

				     partage.setEnergetique(bufferVal);}
				   etape = STX;
				}
				else{

					etape = STX;
				}
			screen->dispStr(1,32,"END");
			///if(etape != STX)
			   partage.releaseSem();
				break;
		}


    }

void TCom1::rxTimeout(void)
    {
    if(++timeout > 40) // timeout de 1 s
	{
	screen->dispStr(10,11,"Timeout");
	partage.releaseSem();
	timeout = 0;
	}
    }
unsigned int TCom1::calculerCRC(unsigned char* input, unsigned int len)
   {
	unsigned int CRC, Low, Hi;
	unsigned int i,j;

	CRC = 0XFFFF;

	for ( i = 0; i < len  ; i++)
		{
		CRC = input[i] ^ CRC;

		for (j = 0; j < 8; j++)
			{
			if (CRC & 0x0001)
				{				//LSB == 1
				CRC = CRC >> 1;
				CRC = CRC ^ 0xA001;
				}
			else				//LSB == 0
				CRC = CRC >> 1;
			}
		}

	// il faut inverser le low et le hi byte
	Low	= CRC & 0x00FF;
	Low = Low << 8;
	Hi =  CRC & 0xFF00;
	Hi =  Hi >> 8;
	CRC = Hi | Low;

	return CRC;
   }

