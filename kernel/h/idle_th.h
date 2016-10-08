/*
 * idle_th.h
 *
 *  Created on: Jul 22, 2015
 *      Author: OS1
 */

#ifndef IDLE_TH_H_
#define IDLE_TH_H_

#include "thread.h"

class Idle: public Thread
{
public:
	int ON;
	Idle():Thread(50, 1){ ON = 1; }
	void run();
	~Idle(){ ON = 0; }
};

void Idle::run()
{
	while(ON);
}

#endif /* IDLE_TH_H_ */
