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
	uint16_t timeout;
	//char condition=3;
	//unsigned int etape;
	unsigned char tab[20];
	enum etape_t{
		STX,//0x02
		FXC, //0x03
		NBR, //0x04
		Data1, //somebody
		Data2, //somebody
		Data3, //somebody
		Data4, //somebody
		CRC1, //somebody
		CRC2//, //somebody

	}etape;

    public:
	TCom1(const char *name,sharedData_t *shared,int32_t priority,baudrate_t baudRate = b115200,
                                   parity_t parite = pNONE,dimData_t dimData = dS8,int32_t timeoutRxMs = 25);
	~TCom1();

	virtual void rxCar(unsigned char car);
	virtual void rxTimeout(void);
	unsigned int calculerCRC(unsigned char* input, unsigned int len);

    };
extern TCom *com1;
#endif //COM1_HPP

