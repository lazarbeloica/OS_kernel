/*
 * Kernel.h
 *
 *  Created on: Jun 4, 2015
 *      Author: OS1
 */

#ifndef KERNEL_H_
#define KERNEL_H_
#include <dos.h>
#include <IOSTREAM.H>
#include "SCHEDULE.H"
#include "def.h"
#include "thread.h"
#include "PCB.h"
#include "semaphor.h"
#include "KerSem.h"
#include "event.h"
#include "KernelEv.h"
#include "IVTEn.h"
#include "makro.h"
#include "niz.h"


//svi clanovi klase Kernel su staticki sto znaci da je
//moguce kreirati samo jedan objekat ove klase

class Kernel
{
private :
	static void interrupt SYS_CALL(...);		//zajednicka prekidna rutina za sve sistemske pozive
	static void ActionChoise(unsigned t0, unsigned t1, unsigned t2);

	static unsigned *KernelStek;
	static int KernelMode;
	static int BOOTED;

	//spisak prijateljskih klasa i funkcija
	friend class PCB;
	friend class KernelEv;
	friend class KernelSem;
	friend void interrupt timer(...);


	static Routine oldTimer;

public:
	static void BOOT();
	static void SHUTDOWN();
	static int getKernelMode();
};

#endif /* KERNEL_H_ */
