/*
 * KernelEv.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: OS1
 */

#include "KernelEv.h"

Niz<KernelEv> *KernelEv::NizEv = new Niz<KernelEv>;

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


KernelEv::KernelEv(IVTNo No):val(0), ivtNo(No)
{
	father = (PCB*)PCB::running;
}

//////////////////////////////////////////////////////////////////////////////////////////////

KernelEv::~KernelEv()
{
	father->state = PCB::Ready;  		//za slucaj da je bila blokirana
	IVTEntry::IVT[ivtNo]->removeKernelEvent();	//bolje je pozvati odavde jer je ivtNo dostupan
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////STATICKE METODE KOJE KORISTE SISTEMSKI POZIVI///////////////////////////////

void KernelEv::CreateEvent(eventTransp *transp)
{
	KernelEv *tmp = new KernelEv(transp->No);
	transp->ev->myImpl = NizEv->add(tmp);	//ubacivanje novog KernelEv u niz svih postojecih KernelEv

	IVTEntry::IVT[transp->No]->setKernelEvent(tmp);		//povezivanje KernelEv sa IVTEntry objektom
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void KernelEv::deleteEvent(ID id)
{
	delete NizEv->niz[id];		//pozivanje destriktora KernelEv
	NizEv->remove(id);			//izbacivanje iz liste svih KernelEv
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void KernelEv::waitEvent(ID id)
{
	if(PCB::running == NizEv->niz[id]->father)
		if(--NizEv->niz[id]->val<0)
			NizEv->niz[id]->father->state = PCB::Blocked;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void KernelEv::signalEvent(ID id)
{
	signalEvent(NizEv->niz[id]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void KernelEv::signalEvent(KernelEv *ev)
{
	if (ev->val++ < 0)	//ovo je uslov da je nit blokirana na binarnom semaforu
	{
		ev->father->state = PCB::Ready;  //vracamo je u stanje spremnih
#ifndef BCC_BLOCK_IGNORE		//ovo moze da se desi i van sistemskog rezima pa treba da se obezbedi
		asm cli;
		Scheduler::put(ev->father);
		asm sti;
#endif
	}
	if(ev->val>0)ev->val = 0;   //mozda ovde treba i jedan da ide
}















