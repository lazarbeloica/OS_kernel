/*
 * sleeplist.cpp
 *
 *  Created on: Jul 18, 2015
 *      Author: OS1
 */

#include "sleepl.h"

#include "SCHEDULE.H"

void SleepList::add(PCB *p, unsigned &s)
{
	p->state = PCB::Sleeping;
	SleepNode *novi = new SleepNode(p, s);
	if (head == 0) head = novi;
	else
	{
		if (novi->TTS < head->TTS)
		{
			head->TTS -= novi->TTS;
			novi->next = head;
			head = novi;
		}
		else
		{
			SleepNode *tek = head, *pred;
			while (tek != 0 && tek->TTS <= novi->TTS)
			{
				novi->TTS -= tek->TTS;
				pred = tek;
				tek = tek->next;
			}
			if (tek == 0){ pred->next = novi; }
			else
			{
				novi->next = tek->next;
				tek->next = novi;
			}
		}
	}
}



void SleepList::check()
{
	if (head != 0)
	{
		--head->TTS;
		while (head != 0 && head->TTS == 0)
		{
			head->pcb->state = PCB::Ready;
			Scheduler::put(head->pcb);
			SleepNode *stari = head;
			head = head->next;
			delete stari;

		}
	}
}


SleepList::~SleepList()
{
	SleepNode *stari;
	while (head != 0)
	{
		head->pcb->state = PCB::Ready;
		Scheduler::put(head->pcb);
		stari = head;
		head = head->next;
		delete stari;
	}
}
