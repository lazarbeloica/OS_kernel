/*
 * PCB.cpp
 *
 *  Created on: May 21, 2015
 *      Author: OS1
 */
#include "PCB.h"

State PCB::New = 1;
State PCB::Running = 2;
State PCB::Ready = 3;
State PCB::Blocked = 4;
State PCB::Sleeping = 5;
State PCB::Done = 6;
State PCB::Idle = 7;

/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

int PCB::brojac = 20;
volatile PCB* PCB::running = 0;
Niz<PCB> *PCB::NizPCB = new Niz<PCB>();
SleepList *PCB::sleepList = new SleepList();


//////////////////////////////////////////////////////////////////////////////////////////

PCB::~PCB()
{
	delete[] stack;
	NizPCB->remove(id);	//izbacivanje iz spiska svih PCB-ova u sistemu
}

///////////////////////////////////////////////////////////////////////////////////////////

PCB::PCB(Thread *myth, StackSize Size, Time Slice)
{

	myThread=myth;
	sSize= Size>MAX_STEK ? MAX_STEK : Size;
	tSlice= Slice;
	infinity = tSlice==0 ? 1 : 0;
	id = NizPCB->add(this); //dodela id-a
	stack=0;
	state=New;
	waitingPCB = new BlockedQueue();
}

/////////////////////////////////////////////////////////////////////////////////////////

void PCB::stackCreate()
{

		stack=new unsigned[sSize];
	#ifndef BCC_BLOCK_IGNORE
		stack[sSize-1]=FP_SEG(myThread);		//wrapperovi argumenti
		stack[sSize-2]=FP_OFF(myThread);		//pokazivac na nit
	#endif

		stack[sSize-5] =0x200;					//PSW  //setovan i bit

#ifndef BCC_BLOCK_IGNORE
		stack[sSize-6] = FP_SEG(myThread->Thread::wrapper);		//adresa f-je
		stack[sSize-7] = FP_OFF(myThread->Thread::wrapper);


		bp = sp = FP_OFF(&stack[sSize - 16]);			//cuvanje svih programski
		ss = FP_SEG(&stack[sSize - 16]);				//dostupnih registara
#endif
		state = PCB::Ready;								//promena stanja niti
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


void  PCB::ThConstructor(...)
{
	TranspKonst *transp;
	unsigned off, segg;
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov segg, bx
		mov off, cx
	}

	transp = (TranspKonst*)MK_FP(segg, off);
#endif
	PCB* novi = new PCB(transp->nit,transp->tSlice, transp->sSize);
	transp->nit->myPCB = novi->id;			//inicijalizacija myPCB pola u objektu Thread-a
}


////////////////////////////////////////////////////////////////////////////////////////////


void  PCB::ThDestructor(...)
{
	Thread *nit;
	unsigned segg, off;
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov segg, bx
		mov off, cx
	}

	nit = (Thread*)MK_FP(segg, off);
#endif

	delete PCB::NizPCB->get(nit->myPCB);
}

//////////////////////////////////////////////////////////////////////////////////////////


void  PCB::ThStart(...)
{
	Thread *nit;
	unsigned segg, off;
#ifndef BCC_BLOCK_IGNORE
	asm {
		mov segg, bx
		mov off, cx
	}
	nit = (Thread*)MK_FP(segg, off);
#endif
	PCB *pcb = PCB::NizPCB->get(nit->myPCB);
	pcb->stackCreate();
	if(pcb->id != 1)		//Idle nit se ne stavlja u Scheduler
	{
		pcb->state=PCB::Ready;
		Scheduler::put(pcb);
	}
	else if(pcb->id != 0) pcb->state = PCB::Idle;
}

//////////////////////////////////////////////////////////////////////////////////////////


void PCB::putToSeep(unsigned s)
{
	if (!(s<=0))sleepList->add((PCB*)PCB::running, s);	//vreme sna ne sme da bude negativno ili 0
//	Kernel::ConextSwitch();

}

//////////////////////////////////////////////////////////////////////////////////////////


void PCB::putToWait(unsigned id)
{
	if(!(NizPCB->niz[id]==0 || NizPCB->niz[id]==running || NizPCB->niz[id]->state == Done || NizPCB->niz[id]->state == New ))
	{
		NizPCB->niz[id]->waitingPCB->put((PCB*)running);
		running->state = Blocked;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

void PCB::killThread()
{
	running->waitingPCB->expellAll();		//oslobadjanje svih niti koje cekaju na ovu nit
	running->state = Done;
}


///////////////////////END///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////









