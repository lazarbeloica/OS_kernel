/*
 * KernelEv.h
 *
 *  Created on: Aug 20, 2015
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "def.h"
#include "event.h"
#include "IVTEn.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "makro.h"
#include "niz.h"


class KernelEv
{
private:

	int val;
	PCB *father;
	IVTNo ivtNo;

	KernelEv(IVTNo);
	~KernelEv();

	//staticki niz svih postojecih KernelEv u sistemu
	static Niz<KernelEv> *NizEv;

	//staticke metode koje se koriste za sistemske pozive
	static void CreateEvent(eventTransp*);
	static void deleteEvent(ID);
	static void signalEvent(KernelEv*);
	static void signalEvent(ID);
	static void waitEvent(ID);

	//spisak prijateljskih klasa
	friend class IVTEntry;
	friend class Kernel;
	friend class Niz<KernelEv>;
};



#endif /* KERNELEV_H_ */
