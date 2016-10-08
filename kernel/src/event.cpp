/*
 * event.cpp
 *
 *  Created on: Aug 11, 2015
 *      Author: OS1
 */

#include "event.h"

Event::Event(IVTNo No)
{
	eventTransp *transporter = new eventTransp(No, this);
	unsigned t0, t1;
#ifndef BCC_BLOCK_IGNORE
	lock
	t0 = FP_SEG(transporter);
	t1 = FP_OFF(transporter);
	asm{
		push ax
		push bx
		push cx
		push dx

		mov ax, Ev_Crea_ID
		mov bx, t0
		mov cx, t1

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


Event::~Event()
{
	unsigned t = myImpl;
#ifndef BCC_BLOCK_IGNORE
	lock
	asm{
		push ax
		push bx
		push cx
		push dx

		mov ax, Ev_Dest_ID
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

void Event::signal()
{
	unsigned t = myImpl;
#ifndef BCC_BLOCK_IGNORE
	lock
	asm{
		push ax
		push bx
		push cx
		push dx

		mov ax, Ev_Signal_ID
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


void Event::wait()
{
	unsigned t = myImpl;
#ifndef BCC_BLOCK_IGNORE
	lock
	asm{
		push ax
		push bx
		push cx
		push dx

		mov ax, Ev_Wait_ID
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


