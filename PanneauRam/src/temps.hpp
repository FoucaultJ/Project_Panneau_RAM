#ifndef TEMPS_HPP
#define TEMPS_HPP

#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "mutex.hpp"

class TTemps
    {
    private:
	char strTemps[50];
	time_t temps;
	struct tm tmTemps;

	struct timeval tDebut,tFin;

	TMutex lock;
    public:
	TTemps(void);

	char *now(void);
	char *now_rt(char *bufferTemps);

	void startMesure(void);
	void stopMesure(void);
	double mesure_us(void);
    };

extern TTemps *temps;

#endif // TEMPS_HPP
