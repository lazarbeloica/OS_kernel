/*
 * Main.cpp
 *
 *  Created on: Jul 13, 2015
 *      Author: OS1
 */


#include <STDIO.H>
#include "Kernel.h"
#include "def.h"
#include "idle_th.h"


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[])
{
#ifndef BCC_BLOCK_IGNORE
	asm cli;
#endif

	Kernel::BOOT();

	Idle *IDLE = new Idle();
	IDLE->start();

/*	cout<<"\n********************************************************************************\n"
			"***********************************WELCOME**************************************\n"
			"********************************************************************************\n"<<endl;*/

#ifndef BCC_BLOCK_IGNORE
	asm sti;
#endif

	int usmn = userMain(argc, argv);

#ifndef BCC_BLOCK_IGNORE
	asm cli;
#endif

	IDLE->~Idle();
	Kernel::SHUTDOWN();

/*	cout<<"\n********************************************************************************\n"
			"***********************************GOOD BYE*************************************\n"
			"********************************************************************************\n"<<endl;*/

#ifndef BCC_BLOCK_IGNORE
	asm sti;
#endif

	return usmn;
}


