/*
 * com_melangeur.hpp
 *
 *  Created on: 2017-11-30
 *      Author: jfoucault
 */

#ifndef COM_MELANGEUR_HPP_
#define COM_MELANGEUR_HPP_
#include "com.hpp"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "TPartage.hpp"
using namespace std;
class TCom2 : public TCom
    {
    private:
	uint16_t timeout;
	char commande,moteur;
	//char tab[18];
	enum etat_t{
			STX,//<
			CMD1, //M
			CMD2, //M|A
			MOT, //A|B|C
			ETX //>

		}etat;
    public:
	TCom2(const char *name,sharedData_t *shared,int32_t priority,baudrate_t baudRate = b115200,
                                   parity_t parite = pNONE,dimData_t dimData = dS8,int32_t timeoutRxMs = 25);
	~TCom2();

	virtual void rxCar(unsigned char car);
	virtual void rxTimeout(void);

    };
extern TCom *com2;
#endif /* COM_MELANGEUR_HPP_ */
