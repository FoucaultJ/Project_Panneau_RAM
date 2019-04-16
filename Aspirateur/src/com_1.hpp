#ifndef COM1_HPP
#define COM1_HPP

#include "com.hpp"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "TPartage.hpp"
using namespace std;
class TCom1 : public TCom
    {
    private:
	char tempon[20];
	uint16_t timeout;
	//char condition=3;
	//unsigned int etape;
	unsigned char tab[3];
	unsigned char demande;

	enum commande_t{
		STX,//<
		CMD, //M|A|P|D
		END //>

	}commande;

    public:
	TCom1(const char *name,sharedData_t *shared,int32_t priority,baudrate_t baudRate = b115200,
                                   parity_t parite = pNONE,dimData_t dimData = dS8,int32_t timeoutRxMs = 25);
	~TCom1();

	virtual void rxCar(unsigned char car);
	virtual void rxTimeout(void);


    };
extern TCom *com1;
#endif //COM1_HPP
