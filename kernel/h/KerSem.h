/*
 * KernelSem.h
 *
 *  Created on: Jun 28, 2015
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_


#include "PCB.h"
#include "Kernel.h"
#include "SCHEDULE.H"
#include "semaphor.h"
#include "blockedQ.h"
#include "niz.h"

class Semaphore;

/////////////////////////////////////////////////////////////////////////

struct SemVal
{
	int id;
	int val;
	SemVal(int i): id(i){}
};

/////////////////////////////////////////////////////////////////////////

struct SemCraTransp
{
	Semaphore *sem;
	int init;
	SemCraTransp(Semaphore *s, int i):sem(s), init(i){}
};

/////////////////////////////////////////////////////////////////////////

class KernelSem
{
private:
	int val;
	BlockedQueue *SemBlocked;

	KernelSem(int init);
	~KernelSem();
	void erase();  //metoda za oslobadjanje blokiranih pri brisanju semafora

	//staticki niz svih semafora ciji se id-evi koriste
	static Niz<KernelSem> *Semaphores;

	//staticke metode za sistemske pozive
	static void CreateSem(SemCraTransp *tra);
	static void WaitSem(int myID);
	static void SignalSem(int myID);
	static void GetValue(SemVal *s);
	static void DestroySem(int myID);

	//spisak prijateljskih klasa
	friend class Kernel;
	friend class Niz<KernelSem>;
};


#endif /* KERSEM_H_ */
