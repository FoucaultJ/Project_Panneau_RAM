#include "temps.hpp"

TTemps *temps;

TTemps::TTemps(void)
    {
    memset(&tDebut,0,sizeof(struct timeval));
    memset(&tFin,0,sizeof(struct timeval));
    memset(&temps,0,sizeof(time_t));
    }

char *TTemps::now(void)
    {
    time(&temps);
    localtime_r(&temps,&tmTemps);

    strftime(strTemps,sizeof strTemps,"%F %T",&tmTemps);

    return strTemps;
    }

char *TTemps::now_rt(char *bufferTemps)
    {
    lock.take();

    time(&temps);
    localtime_r(&temps,&tmTemps);

    strftime(bufferTemps,sizeof strTemps,"%F %T",&tmTemps);

    lock.release();

    return bufferTemps;
    }

void TTemps::startMesure(void)
    {
    gettimeofday(&tDebut,NULL);
    }

void TTemps::stopMesure(void)
    {
    gettimeofday(&tFin,NULL);
    }

double TTemps::mesure_us(void)
    {
    return (((tFin.tv_sec - tDebut.tv_sec) * 1000000) + (tFin.tv_usec - tDebut.tv_usec));
    }

