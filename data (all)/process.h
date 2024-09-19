#pragma once
#include <fstream>
#include <iostream>
#include "LinkedQueue.h"
enum processState {                                             // declaring the possible states for any process
	NEW,
	RDY,
	RUN,
	BLK,
	TRM,
	ORPH
};
struct request {                                                // struct for data of requests (IOR/IOD)
	int IOR;
	int IOD;
};

class process {
private:
	int PID, AT, RT, CT, TT, TRT, WT, N, IOR, IOD, numR, blktime;     // id of process, Arrival time, response time, cpu time, termination time, turn around time, waiting time , no. of requests
	float Deadline;                                                  
	int TotalIOD;                                                     // sum of IOD of requests that has been occured 
	float remaining_CT;
	int  forkedBefore;                                               // determine whether the process has been forked before once or twice or not
	processState state;
	char ch;
	process* child1;                                                 // pointer points to its first child
	process* child2;                                                 // pointer points to its second child
	process* parent;
	request* r;
	LinkedQueue <request*> Req;                                   // Make a queue of pointers to structs to carry requests data
public:
	process(int at = 0, int pid = 0, int ct = 0, int n = 0) {
		AT = at; PID = pid; CT = ct;
		remaining_CT = CT;
		TotalIOD = 0;
		Deadline = 0;
		forkedBefore = 0;
		state = NEW;
		numR = n;
		child1 = nullptr;
		child2 = nullptr;
		parent = nullptr;
		IOR = 0;
		IOD = 0;
	}
	//operator overloading friend function
	friend ostream& operator <<(ostream& output, process* pros);
	void Loaddata(ifstream& fin);
	int getN();
	int getnumR();
	int getPID();
	int getAT();
	int getRT();
	int getCT();
	int getTT();
	int getTRT();
	int getWT();
	void set_remianingCT(int x);
	float get_remainingCT();
	void setRT(int time);
	void setTT(int TClock);
	void setTRT(int trt);
	void setWT(int wt);
	void setState(processState s);
	void setforkedBefore();
	int getforkedBefore();
	void setChild1(process* ptr);
	process* getchild1();
	void setChild2(process* ptr);
	process* getchild2();
	void setParent(process* ptr);
	process* getparent();
	processState getstate();
	int getIOR();
	int getIOD();
	bool setIOD_IOR();
	void setTotalIOD(int total);
	int getTotalIOD();
	void setBLKt();
	void incrementBLKt();
	int getBLKt();
	float getDeadline();
};