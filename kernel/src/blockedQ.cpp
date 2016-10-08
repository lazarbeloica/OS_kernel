/*
 * blockedQ.cpp
 *
 *  Created on: Jul 22, 2015
 *      Author: OS1
 */

#include "blockedQ.h"



void BlockedQueue::put(PCB *p)
{
	if (!head) head = tail = new Queue(p);
	else { tail->next = new Queue(p); tail = tail->next; }
}

PCB* BlockedQueue::get()
{
	if (head == 0) return 0;
	Queue *tmp = head;
	PCB *ret = tmp->pcb;
	if (head == tail) head = tail = 0;
	else head = head->next;
	delete tmp;
	return ret;
}

void BlockedQueue::expellAll()
{
	Queue *temp;
	while(head != 0)
	{
		temp = head;
		Scheduler::put(temp->pcb);
		head = head->next;
		delete temp;
	}
}
