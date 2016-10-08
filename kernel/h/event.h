/*
 * event.h
 *
 *  Created on: Aug 11, 2015
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "def.h"
#include <DOS.H>

typedef unsigned char IVTNo;
class KernelEv;

class Event
{
public:
	Event(IVTNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	void signal();		//can call KernelEv

private:
	ID myImpl;
};

struct eventTransp
{
	IVTNo No;
	Event *ev;
	eventTransp(IVTNo &n, Event *p):No(n), ev(p){}
};

#endif /* EVENT_H_ */
