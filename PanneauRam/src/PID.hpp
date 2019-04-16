/*
 * PID.hpp
 *
 *  Created on: 2017-12-12
 *      Author: jfoucault
 */

#ifndef PID_HPP_
#define PID_HPP_

class PID {
private:
	double kp;
	double ki;
	double sommation;
public:
	PID();
	virtual ~PID();
	double calculePid(double valmesure, double consigne);
	double calculePidPetitB(double valmesure, double consigne);
	double calculePidTemp(double valmesure, double consigne);
	double getKp() const;
	void setKp(double val);
	double getKi() const;
	void setKi(double val);
	void resetSommation();
	double getSommation();
};

#endif /* PID_HPP_ */
