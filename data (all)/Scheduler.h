#pragma once
#include <fstream>
#include <iostream>
#include <Windows.h>
#include "LinkedList.h"
#include "LinkedQueue.h"
#include "PriorityQueue.h"
#include "process.h"
#include "processor.h"
#include "FCFSProcessor.h"
#include "SJFProcessor.h"
#include "EDFProcessor.h"
#include "RRProcessor.h"
#include "UI.cpp"
using namespace std;
class Scheduler {
private:
	int NFCFS, NSJF, NRR, NEDF, STL, Nprocesses;
	int NTotal;                                                  //Total no. of processors
	processor** arr;                                            // array of pointers to processors
	process* ptr;
	process* c;
	LinkedQueue <process*> newlist;                             // Make a queue for New list
	int time_step = 1;
	int k ;
	LinkedQueue <process*> Blklist;
	LinkedQueue <process*> Trmlist;
	UI user;
	bool Nosteal;
	int SumWT=0;
	int SumRT=0;
	int SumTRT=0;
	int steallimit;
	int avgWORKSTEAL;
	int avgUTIL;
	float StealedProcesses = 0;
	float BeforeDeadline=0;
	int NStop;
public:
	void FileLoadingFunc();                                  // load data from input file
	void simple_simulator();                                // for each time step, make the needed moves
	int  getLongestRDY();                                  // return an index to the processor having the highest expected finish time
	int   getShortestRDY();                                // return an index to the processor having the lowest expected finish time
	int   getShortestFCFS();                              // return an index to the FCFS processor having the lowest expected finish time
	int   getShortestSJF();                              // return an index to the SJF processor having the lowest expected finish time
	int   getShortestRR();                              // return an index to the RR processor having the lowest expected finish time
	bool  move_from_FCFS_to_RR(process* RunProcess);                     // function1 of migration
	bool  move_from_RR_to_SJF(process* RunProcess);                     // function2 of migration
	float   get_steal_limit(int m, int n);
	void  Stealing();
	void  Move_From_Run_To_TRM(process* &trmptr);                      // process completion
	void  writeOutputFile();                                          // print data and statistics on output file
	void  forkChild(process* parent);
	void  killOrphans(process* parent);
	void  MovefromRUNtoBLK(process*& ptr);
	void MovefromBLKtoRDY();
	float gettotalPUtil();
	int getNRR();
	int getNSJF();
	void ProcessorOverheat(int processorindex);
	~Scheduler();
};