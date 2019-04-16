/*
 * PID.cpp
 *
 *  Created on: 2017-12-12
 *      Author: jfoucault
 */

#include "PID.hpp"

PID::PID() {
	// TODO Auto-generated constructor stub
 kp=0;
 ki=0;
 sommation=0;
}

PID::~PID() {
	// TODO Auto-generated destructor stub

}

double PID::calculePid(double valmesure, double consigne){//calcule du p
	double val;

     val= consigne-valmesure;


     if(val<15 && val>-15){
    	 sommation +=val;
    	 val= (val*kp)+(sommation*ki);
      }
     else{
    	 sommation=0;
    	 val = val *kp;
     }

     if(val >100)
    	 val=100;
     else if(val<0)
    	 val=0;
	return val;
}
void PID::resetSommation()
{
	sommation =0;
}
double PID::getSommation(){
	return sommation;
}
double PID::calculePidPetitB(double valmesure, double consigne){
	double val;

     val= consigne-valmesure;


     if(val<65 && val>-15){
    	 sommation +=val;
    	 val= (val*kp)+(sommation*ki);
     }
     else{
    	 sommation=0;
    	 val = val *kp;
     }

     if(val >100)
    	 val=100;
     else if(val<0)
    	 val=0;
	return val;
}
double PID::calculePidTemp(double valmesure, double consigne){
	double val;

	     val= consigne-valmesure;


	    if(val<5 && val>-5){
	    	 sommation +=val;
	    	 val= (val*kp)+(sommation*ki);
	     }
	     else{
	    	 sommation=0;
	    	 val = val *kp;
	     }

		return val;
}
double PID::getKp() const{
	return kp;
}
void PID::setKp(double val){
	kp =val;
}
double PID::getKi() const{
	return ki;
}
void PID::setKi(double val){
	ki = val;
}
