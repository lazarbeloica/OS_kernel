/*
 * makro.h
 *
 *  Created on: Jul 26, 2015
 *      Author: OS1
 */

#ifndef MAKRO_H_
#define MAKRO_H_

#include "Kernel.h"
#include "KernelEv.h"

#define PREPAREENTRY(No, OldR)\
void interrupt eventInterrupt##No(...);\
IVTEntry IVT##No(No, &eventInterrupt##No);\
void interrupt eventInterrupt##No(...)\
{\
	IVT##No.signalEvent();\
	if (OldR) IVT##No.callOldRoutine();\
	if (!Kernel::getKernelMode())\
		dispatch();\
}

#endif /* MAKRO_H_ */
