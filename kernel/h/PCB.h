/*
 * PCB.h
 *
 *  Created on: May 21, 2015
 *      Author: OS1
 */

#ifndef _PCB_H_
#define _PCB_H_


#include "def.h"
#include "thread.h"
#include "sleepl.h"
#include "blockedQ.h"
#include "niz.h"
#include "Kernel.h"

class Thread;
class SleepList;

//////////////////////////////////////////////////////

struct transporter
{
public:
	void (*b)(unsigned int);
	int k;
	int id;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TranspKonst
{
	Thread *nit;
	Time tSlice;
	StackSize sSize;
	TranspKonst(Thread *n, Time t, StackSize s):nit(n), tSlice(t), sSize(s){}
};

////////////////////////////////////////////////////////

class PCB
{
private:

	volatile static PCB *running;
	static int brojac;
	static Niz<PCB> *NizPCB;
	static State New, Ready, Running, Blocked, Sleeping, Done, Idle;	//moguca stanja PCB-a

	ID id;

	unsigned sp;
	unsigned ss;
	unsigned bp;

	stek *stack;
	State state;
	int infinity;

	Thread *myThread;
	Time tSlice;
	StackSize sSize;

	PCB(Thread *myTh, StackSize Size, Time Slice);
	~PCB();
	void stackCreate();

	//staticke metode koje se koriste za sistemske pozive
	static void ThConstructor(...);
	static void ThDestructor(...);
	static void ThStart(...);
	static void putToSeep(unsigned);
	static void putToWait(unsigned);
	static void killThread();

	//prioritetni red svih uspavani niti
	static SleepList *sleepList;

	//red niti koje cekaju na ovaj objekat PCB-a
	BlockedQueue *waitingPCB;

	//spisak prijateljskih klasa i funkcija
	friend class Kernel;
	friend class KernelEv;
	friend class KernelSem;
	friend class IVTEntry;
	friend class SleepList;
	friend class BlockedQueue;
	friend class Niz<PCB>;
	friend void interrupt timer(...);

};


#endif /* PCB_H_ */
