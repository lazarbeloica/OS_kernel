/*
 * Thread.cpp
 *
 *  Created on: May 25, 2015
 *      Author: OS1
 */

#include "thread.h"
#include <IOSTREAM.H>

void Thread::start()
{
unsigned segg, off;
#ifndef BCC_BLOCK_IGNORE
lock
	segg = FP_SEG(this);
	off = FP_OFF(this);

	asm {
			push ax
			push bx
			push cx
			push dx
			mov ax, TH_START_ID     	///myPCB = new PCB(this,timeSlice, stackSize);
			mov bx, segg
			mov cx, off
			int IVTSYS_CALL
			pop ax
			pop bx
			pop cx
			pop dx
		}
unlock
#endif
}


Thread::Thread (StackSize stackSize, Time timeSlice)
{
	TranspKonst *transporter = new TranspKonst(this, stackSize, timeSlice);
	unsigned segg, off;
#ifndef BCC_BLOCK_IGNORE
lock
	segg = FP_SEG(transporter);
	off = FP_OFF(transporter);


	asm {
			push ax
			push bx
			push cx
			push dx
			mov ax, TH_CONST_ID     	///myPCB = new PCB(this,timeSlice, stackSize);
			mov bx, segg
			mov cx, off
			int IVTSYS_CALL
			pop ax
			pop bx
			pop cx
			pop dx
		}
unlock
#endif
	delete transporter;
}

Thread::~Thread()
{
	waitToComplete();
unsigned segg, off;
#ifndef BCC_BLOCK_IGNORE
lock
	segg = FP_SEG(this);
	off = FP_OFF(this);

	asm {
		push ax
		push bx
		push cx
		push dx
		mov ax, TH_DEST_ID   //delete stack
		mov bx, segg
		mov cx, off
		int IVTSYS_CALL
		pop ax
		pop bx
		pop cx
		pop dx
	}
unlock
#endif
}


void Thread::wrapper(Thread *running)
{
	running->run();
#ifndef BCC_BLOCK_IGNORE
	lock
asm{
		push ax
		push bx
		push cx
		push dx

		mov ax, TH_KILL_ID
		int IVTSYS_CALL

		pop dx
		pop cx
		pop bx
		pop ax
	}
	unlock
#endif
}


void Thread::sleep(Time timeToSleep)
{
	unsigned t = timeToSleep;
#ifndef BCC_BLOCK_IGNORE
lock
	asm {
			push ax
			push bx
			push cx
			push dx
			mov ax, TH_SLEEP_ID
			mov bx, t
			int IVTSYS_CALL
			pop ax
			pop bx
			pop cx
			pop dx
		}
unlock
#endif
}


void Thread::waitToComplete()
{
	unsigned t = myPCB;
#ifndef BCC_BLOCK_IGNORE
lock
	asm {
			push ax
			push bx
			push cx
			push dx
			mov ax, TH_WAIT_ID
			mov bx, t
			int IVTSYS_CALL
			pop ax
			pop bx
			pop cx
			pop dx
		}
unlock
#endif
}




void dispatch()
{

#ifndef BCC_BLOCK_IGNORE
	lock
 	 asm{
	 push ax
	 push bx
	 push cx
	 push dx

	 mov ax, TH_DIPCH_ID
	 int IVTSYS_CALL

	 pop dx
	 pop cx
	 pop bx
	 pop ax
 }
	unlock
#endif
}

