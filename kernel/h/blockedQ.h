/*
 * blockedQ.h
 *
 *  Created on: Jul 22, 2015
 *      Author: OS1
 */

#ifndef BLOCKEDQ_H_
#define BLOCKEDQ_H_

#include "SCHEDULE.H"

class PCB;


class BlockedQueue
{
private:
	struct Queue
	{
		PCB *pcb;
		Queue *next;
		Queue(PCB *p) :pcb(p), next(0){}
	};

	Queue *head, *tail;

public:
	BlockedQueue() :head(0), tail(0){};

	void put(PCB *p);
	PCB* get();
	void expellAll();
};




#endif /* BLOCKEDQ_H_ */
