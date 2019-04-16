#ifndef CONTROL_HPP
#define CONTROL_HPP

#include "thread.hpp"
#include "ram.hpp"
#include "PID.hpp"

class TControl : public TThread
    {
    private:
	TRam::partageRam_t *partageRam;
	PID pidGB, pidPB, pidTPB;
   public:
	TControl(const char *name,sharedData_t *shared,int policy,int priority,destruction_t destruction,int noCpu = -1);
	~TControl();
	double getKpGB() const;
	void setKpGB(double val);
	double getKiGB() const;
	void setKiGB(double val);
	double getKpPB() const;
	void setKpPB(double val);
	double getKiPB() const;
	void setKiPB(double val);
	double getKpTPB() const;
	void setKpTPB(double val);
	double getKiTPB() const;
	void setKiTPB(double val);


	// tâche control
	void task(void);
    };

extern TControl *control;

#endif //CONTROL_HPP
