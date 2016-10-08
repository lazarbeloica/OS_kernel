/*
 * IVTEn.h
 *
 *  Created on: Aug 11, 2015
 *      Author: OS1
 */

#ifndef IVTEN_H_
#define IVTEN_H_

#include <DOS.H>
#include "def.h"
#include "event.h"
#include "KernelEv.h"

class IVTEntry
{
private:
	IVTNo myIVTNo;
	KernelEv *myKernelEv;
	Routine oldRoutine, newRoutine;

public:
	IVTEntry(IVTNo, Routine);
	~IVTEntry();

	static IVTEntry *IVT[256];

	void setKernelEvent(KernelEv*);
	void removeKernelEvent();
	void callOldRoutine();
	void signalEvent();
};


#endif /* IVTEN_H_ */
