/*
 * def.h
 *
 *  Created on: Jul 14, 2015
 *      Author: OS1
 */

#ifndef DEF_H_
#define DEF_H_

//64KB
#define MAX_STEK 65536

///////////////////////////////////////////////////////////////

#define TH_CONST_ID 1
#define TH_DEST_ID 2
#define TH_START_ID 3
#define TH_SLEEP_ID 4
#define TH_WAIT_ID 5
#define TH_DIPCH_ID 6
#define TH_KILL_ID 7

///////////////////////////////////////////////////////////////

#define Sem_Crea_ID 8
#define Sem_Dest_ID 9
#define Sem_Wait_ID 10
#define Sem_Signal_ID 11
#define Sem_Val_ID 12

///////////////////////////////////////////////////////////////

#define Ev_Crea_ID 13
#define Ev_Dest_ID 14
#define Ev_Signal_ID 15
#define Ev_Wait_ID 16

//////////////////////////////////////////////////////////////////////////////

#define IVTSYS_CALL 0x60
#define IVTTIMER 0x8
#define oldIVTTIMER 0x61

//////////////////////////////////////////////////////////////////////////////

void interrupt timer(...);

//////////////////////////////////////////////////////////////////////////////


#define lock \
		asm{\
	pushf;\
	cli;\
}

#define unlock \
	asm popf


typedef unsigned stek;
typedef int State;

typedef unsigned long StackSize;


typedef unsigned int Time;						//time,x 55ms
				//default = 2*55ms
typedef int ID;

typedef void interrupt (*Routine)(...);
int userMain(int argc, char *argv[]);



#endif /* DEF_H_ */
