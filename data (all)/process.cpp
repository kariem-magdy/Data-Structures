#pragma once
#include "process.h"
using namespace std;

	//operator overloading friend function
	void process::Loaddata(ifstream& fin) {                                    // load data of each process
		fin >> AT >> PID >> CT >> Deadline >> N;                                 // Load arrival time, ID, CPU time,Deadline, No. of requests
		remaining_CT = CT;
		for (int i = 0; i < N;i++) {
			r = new request;
			fin >> ch>> r->IOR>>ch >> r->IOD>>ch;                   // Load IOR and Iod in a struct
			Req.enqueue(r);                                         // and then enqueue them in a queue of pointers to structs
		}
	}
	void process::setforkedBefore()
	{
		forkedBefore++;
	}
	int process::getforkedBefore()
	{
		return forkedBefore;
	}
	process* process::getparent()
	{
		return parent;
	}
	void process::setChild1(process* ptr)
	{
		child1 = ptr;
	}
	process* process::getchild1()
	{
		return child1;
	}
	void process::setChild2(process* ptr)
	{
		child2 = ptr;
	}
	process* process::getchild2()
	{
		return child2;
	}
	void process::setParent(process* ptr)
	{
		parent = ptr;
	}
	int process::getN() {
		return N;
	}
	int process:: getnumR() {
		return numR;
	}
	int process::getPID()
	{
		return PID;
	}
	int process:: getAT()
	{
		return AT;
	}
	int process:: getRT()
	{
		return RT;
	}
	int process::getCT()
	{
		return CT;
	}
	int process:: getTT()
	{
		return TT;
	}
	int process::getTRT()
	{
		return TRT;
	}
	int process:: getWT()
	{
		return WT;
	}
	void process:: set_remianingCT(int x) {
		remaining_CT = x;
	}
	float process:: get_remainingCT() {
		return remaining_CT;
	}
	void process:: setRT(int time)
	{
		RT = time;
	}
	void process::setTT(int TClock)
	{
		TT = TClock;
	}
	void process:: setTRT(int trt)
	{
		TRT = trt;
	}
	void process::setWT(int wt)
	{
		WT = wt;
	}

	void process:: setState(processState s)
	{
		state = s;
	}
	processState process::getstate()
	{
		return state;
	}
	int process::getIOR()
	{
		return IOR;
	}
	int process::getIOD()
	{
		return IOD;
	}
	void process::setTotalIOD(int total)
	{
		TotalIOD=total;
	}
	int process::getTotalIOD()
	{
		return TotalIOD;
	}
	bool process::setIOD_IOR()
	{
		if (Req.dequeue(r))
		{
			IOR = r->IOR;
			IOD = r->IOD;
			TotalIOD += IOD;
			return true;
		}
		return false;
	}
	void process::incrementBLKt()
	{
		blktime++;
	}
	int process::getBLKt()
	{
		return blktime;
	}
	void process::setBLKt()
	{
		blktime = 1;

	}
	float process:: getDeadline() {
		return Deadline;
	}