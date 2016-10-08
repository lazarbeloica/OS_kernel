/*
 * semaphor.h
 *
 *  Created on: Jun 28, 2015
 *      Author: OS1
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_

#include <DOS.H>
#include "def.h"
#include "KerSem.h"


class Semaphore
{
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();

	virtual void wait();
	virtual void signal();
	int val() const;		//Returns the current value of this semaphore

private:
	ID myImpl;
	friend class KernelSem;
};





#endif /* SEMAPHOR_H_ */
