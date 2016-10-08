/*
 * thread.h
 *
 *  Created on: May 20, 2015
 *      Author: OS1
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "PCB.h"
#include "Kernel.h"
#include "def.h"


const StackSize defaultStackSize = 4096;
const Time defaultTimeSlice = 2;			//default = 2*55ms

class Thread
{
private:
	ID myPCB;

	static void wrapper(Thread *running);
protected:
	friend class PCB;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run(){}
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	static void sleep (Time timeToSleep);
};

void dispatch();



#endif /* THREAD_H_ */
