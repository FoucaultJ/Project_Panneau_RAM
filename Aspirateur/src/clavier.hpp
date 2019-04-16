#ifndef CLAVIER_HPP
#define CLAVIER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <termio.h>
#include <stdint.h>

class TClavier
    {
    private:
	struct termios oldTermios;
	const int timeoutMs;		// temps d'attende d'un caractère au clavier

	int16_t reconfigureTerminal(void);
	int16_t restaureTerminal(void);
    public:
	TClavier(void);
	~TClavier();

	int16_t kbhit(void);
	int16_t getch(void);
    };

extern TClavier *clavier;

#endif // CLAVIER_HPP
