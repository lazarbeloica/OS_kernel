/*
 * semaphor.cpp
 *
 *  Created on: Jun 30, 2015
 *      Author: OS1
 */
#include "semaphor.h"


Semaphore::Semaphore(int init)
{
	unsigned ts, to;
	SemCraTransp *tra = new SemCraTransp(this, init);
#ifndef BCC_BLOCK_IGNORE
	lock
	ts = FP_SEG(tra);
	to = FP_OFF(tra);

	asm{
		push ax
		push bx
		push cx
		push dx

		mov ax, Sem_Crea_ID
		mov bx, ts	//prosledjuje segment
		mov cx, to	//prosledjuje offset SemCraTransp-a
		int IVTSYS_CALL

		pop dx
		pop	cx
		pop bx
		pop ax
	}
	unlock
#endif
	delete tra;
}

Semaphore::~Semaphore()
{
	unsigned t = myImpl;
#ifndef BCC_BLOCK_IGNORE
	lock
	asm{
		push ax
		push bx
		push cx
		push dx

		mov ax, Sem_Dest_ID
		mov bx, t
		int IVTSYS_CALL

		pop dx
		pop	cx
		pop bx
		pop ax
	}
	unlock
#endif
}


void Semaphore::wait()
{
	unsigned t = myImpl;
#ifndef BCC_BLOCK_IGNORE
	lock
	asm{
		push ax
		push bx
		push cx
		push dx

		mov ax, Sem_Wait_ID
		mov bx, t		//prosledjuje id semafora
		int IVTSYS_CALL

		pop dx
		pop	cx
		pop bx
		pop ax
	}
	unlock
#endif
}


void Semaphore::signal()
{
	unsigned t = myImpl;
#ifndef BCC_BLOCK_IGNORE
	lock
	asm{
		push ax
		push bx
		push cx
		push dx

		mov ax, Sem_Signal_ID
		mov bx, t		//prosledjuje id semafora
		int IVTSYS_CALL

		pop dx
		pop	cx
		pop bx
		pop ax
	}
	unlock
#endif
}



int Semaphore::val() const		
{
	unsigned segg, off;
	SemVal *sv = new SemVal(myImpl);
#ifndef BCC_BLOCK_IGNORE
	lock
	segg = FP_SEG(sv);
	off = FP_OFF(sv);
	asm{
		push ax
		push bx
		push cx
		push dx

		mov ax, Sem_Val_ID
		mov bx, segg			//prosledjuje segment
		mov cx, off			//prosedjuje offset pokazivaca na transportnu strukturu
		int IVTSYS_CALL

		pop dx
		pop	cx
		pop bx
		pop ax
	}
	unlock
#endif
	int rez = sv->val;
	delete sv;
	return rez;
}










