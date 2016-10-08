/*
 * Kernel.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: OS1
 */


#include "Kernel.h"

int Kernel::KernelMode = 0;
int Kernel::BOOTED = 0;
unsigned *Kernel::KernelStek =  new stek[defaultStackSize];
Routine Kernel::oldTimer = 0;


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

extern void tick();

void interrupt timer(...)
{
	tick();
	PCB::sleepList->check();
#ifndef BCC_BLOCK_IGNORE
	asm int oldIVTTIMER
#endif
	if(!Kernel::KernelMode && !PCB::running->infinity)
	{
		PCB::brojac--;
		if (PCB::brojac == 0)
		{
			dispatch();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////



void interrupt Kernel::SYS_CALL(...)
{

	unsigned t0, t1, t2;//za prihvatanje id poziva i argumenata

#ifndef BCC_BLOCK_IGNORE
	asm{
		mov t0, ax			//id poziva
		mov t1, bx			//argument poziva
		mov t2, cx			//argument poziva
	}
#endif

	KernelMode=1;

////////////////PRELAZAK NA KERNEL NIT////////////////////

#ifndef BCC_BLOCK_IGNORE
	PCB::running->sp = _SP;
	PCB::running->bp = _BP;
	PCB::running->ss = _SS;
#endif

	KernelStek[defaultStackSize-1]=t2;		//smestanje argumenata
	KernelStek[defaultStackSize-2]=t1;		//f-je koja se poziva
	KernelStek[defaultStackSize-3]=t0;

#ifndef BCC_BLOCK_IGNORE
	KernelStek[defaultStackSize-6]=0x200;			//PSW, setovan i fleg

	KernelStek[defaultStackSize-7]=FP_SEG(ActionChoise);		//smestanje adrese
	KernelStek[defaultStackSize-8]=FP_OFF(ActionChoise);		//f-je koja se poziva

	_SP = FP_OFF(&KernelStek[defaultStackSize-8]);			//inicijalizacija ss
	_BP = FP_OFF(&KernelStek[defaultStackSize-8]);			//sp i bp
	_SS = FP_SEG(&KernelStek[defaultStackSize-8]);

	asm iret;
#endif

}


///////////////////////////////////////////////////////////////////////////////////////


void Kernel::ActionChoise(unsigned t0, unsigned t1, unsigned t2)
{
	switch(t0)
	{

//-------------SISTEMSKI POZIVI THREAD-A----------------------------------------------1

			case TH_CONST_ID:

		#ifndef BCC_BLOCK_IGNORE
			asm {
				mov bx, t1
				mov cx, t2
			}
		#endif
			PCB::ThConstructor();
			break;

//--------------------------------------------------------------------------------------2

			case TH_DEST_ID:

		#ifndef BCC_BLOCK_IGNORE
			asm {
				mov bx, t1
				mov cx, t2
			}
		#endif
			PCB::ThDestructor();
			break;

//-------------------------------------------------------------------------------------3

			case TH_START_ID:

		#ifndef BCC_BLOCK_IGNORE
			asm {
				mov bx, t1
				mov cx, t2
			}
		#endif
			PCB::ThStart();
			break;
//--------------------------------------------------------------------------------------4

			case TH_SLEEP_ID:

			PCB::putToSeep(t1);
			break;


//--------------------------------------------------------------------------------------5
			case TH_WAIT_ID:

			PCB::putToWait(t1);
			break;



//--------------------------------------------------------------------------------------6
			case TH_DIPCH_ID:

			break;

//--------------------------------------------------------------------------------------7

			case TH_KILL_ID:

			PCB::killThread();
			break;

//----------------------SISTEMSKI POZIVI SEMAPHORE-A--------------------------------------8

			case Sem_Crea_ID:

			SemCraTransp *sem;
#ifndef BCC_BLOCK_IGNORE
		sem  = (SemCraTransp *)MK_FP(t1, t2);
#endif

			KernelSem::CreateSem(sem);
			break;

//---------------------------------------------------------------------------------------9

			case Sem_Dest_ID:

			KernelSem::DestroySem(t1);
			break;

//--------------------------------------------------------------------------------------10

			case Sem_Wait_ID:

			KernelSem::WaitSem(t1);
			break;

//---------------------------------------------------------------------------------------11

			case Sem_Signal_ID:

			KernelSem::SignalSem(t1);
			break;

//---------------------------------------------------------------------------------------12

			case Sem_Val_ID:

		SemVal *sv;
#ifndef BCC_BLOCK_IGNORE
		sv = (SemVal*)MK_FP(t1, t2);
#endif

			KernelSem::GetValue(sv);
			break;

//----------------------SISTEMSKI POZIVI EVENT-A--------------------------------------//13

			case Ev_Crea_ID:

			eventTransp *tran;
#ifndef BCC_BLOCK_IGNORE
			tran  = (eventTransp *)MK_FP(t1, t2);
#endif

			KernelEv::CreateEvent(tran);
			break;

//--------------------------------------------------------------------------------------\\14

			case Ev_Dest_ID:

			KernelEv::deleteEvent(t1);
			break;

//--------------------------------------------------------------------------------------\\15

			case Ev_Wait_ID:

			KernelEv::waitEvent(t1);
			break;

//--------------------------------------------------------------------------------------\\16

			case Ev_Signal_ID:

			KernelEv::signalEvent(t1);
			break;

			default: break;
		}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////POCETAK POVRATKA NA KORISNICKI REZIM///////////////////////////

#ifndef BCC_BLOCK_IGNORE
	asm cli;
#endif

	KernelMode = 0;

	if(PCB::running->state != PCB::Blocked && PCB::running->state !=PCB::Sleeping && PCB::running->state !=PCB::Idle && PCB::running->state != PCB::Done)
	{
		PCB::running->state = PCB::Ready;
		if(PCB::running->id != 1)
		Scheduler::put((PCB*)PCB::running);
	}
	PCB::running = Scheduler::get();
	if(PCB::running == NULL)
		PCB::running = PCB::NizPCB->niz[1];  //ilde nit
	else PCB::running ->state = PCB::Running;
	PCB::brojac = PCB::running->tSlice;

#ifndef BCC_BLOCK_IGNORE
	_SP=PCB::running->sp;
	_BP=PCB::running->bp;
	_SS=PCB::running->ss;

	asm{
		mov sp,bp
		pop bp
		pop di
		pop si
		pop ds
		pop es
		pop dx
		pop cx
		pop bx
		pop ax

		iret
	}
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


void Kernel::BOOT()
{
	if(!BOOTED)
	{
		PCB *MAIN = new PCB(0, 1, 40);
		PCB::brojac = MAIN->tSlice;
		PCB::running = MAIN;

#ifndef BCC_BLOCK_IGNORE
		setvect(IVTSYS_CALL, SYS_CALL);

		oldTimer = getvect(IVTTIMER);
		setvect(oldIVTTIMER,Kernel::oldTimer);
		setvect(IVTTIMER, timer);
#endif
		BOOTED = 1; 		//zastita od visestruke inicijalizacije sistema
	}
}

////////////////////////////////////////////////////////////////////////////////////////

void Kernel::SHUTDOWN()
{
	if(BOOTED)
	{
		delete PCB::NizPCB;
		delete KernelEv::NizEv;
		delete KernelSem::Semaphores;

#ifndef BCC_BLOCK_IGNORE
		setvect(IVTTIMER, Kernel::oldTimer);
#endif
		BOOTED = 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

int Kernel::getKernelMode() {return KernelMode;}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////END/////////////////////////////////////////
