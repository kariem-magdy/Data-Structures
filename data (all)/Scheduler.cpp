#pragma once
#include "Scheduler.h"
	void Scheduler:: FileLoadingFunc() {
		ifstream fin("input6.txt");
		if (fin.is_open()) {
			fin >> NFCFS >> NSJF >> NRR >> NEDF;
			NTotal = NFCFS + NSJF + NRR + NEDF;
			arr = new processor * [NTotal];
			for (int i = 0; i < NFCFS; i++) {                                  // Make (NFCFS) number of FCFS processors
				arr[i] = new FCFSProcessor;
			}
			for (int i = NFCFS; i < (NFCFS + NSJF); i++) {                    // Make (NSJF) number of SJF processors
				arr[i] = new SJFProcessor;
			}
			for (int i = NFCFS + NSJF; i < NFCFS+NSJF+NRR; i++) {                    // Make (NRR) number of RR processors
				arr[i] = new RRProcessor;
			}
			for (int i = NFCFS + NSJF + NRR; i < NTotal; i++) {                    // Make (NEDF) number of EDF processors
				arr[i] = new EDFProcessor;
			}
			RRProcessor::loadTS(fin);                                       // Load timeslice
			RRProcessor::loadRTF(fin);                                      //load RTF
			FCFSProcessor::LoadMaxW(fin);                                   //load MaxW
			fin >> STL;                                                  // load steal_limit
			FCFSProcessor::LoadFP(fin);                                  // load forking probability
			fin >> NStop >> Nprocesses;                                  // load NO. of time steps to stop in processor overheating and number of processes
			for (int i = 0; i < Nprocesses; i++) {
				c = new process;                                       // Make a new process and load its data members
				c->Loaddata(fin);
				newlist.enqueue(c);                                    // New list is a queue of pointers to processes 
			}
			FCFSProcessor::LoadSigKills(fin);
			fin.close();
		}
	}
	void Scheduler:: simple_simulator() {
		srand((unsigned)time(0));                          // to pick a different number every time we use rand()

		////////////////////////////////Interactive Modes + File loading Function////////////////////////////////////////////////
		int x;
		cout << "Enter (1) for Interactive mode or (2) for Step-By-Step mode or (3) for Silent mode : " << endl;
		cin >> x;
		FileLoadingFunc();

	  	////////////////////////////////Move From New To Ready////////////////////////////////////////////////
	    // check the arrived processes at this timestep
		while  (Trmlist.getsize() != Nprocesses) {
			while (newlist.peek(ptr) == true && ptr->getAT() == time_step) {                       // moving processes from new to rdy
				newlist.dequeue(ptr);
				k= getShortestRDY();
				arr[k]->Move_To_Ready(ptr);
			}

			////////////////////////////////Sig Kill////////////////////////////////////////////////
			// check if i need to make a sigkill at this timestep
			if (FCFSProcessor::timetosigkill(time_step)) {               
				for (int j = 0; j < NFCFS; j++) {
					bool x = false;
					x = arr[j]->tokill(time_step, ptr);                  // function of sigkill
					if (x == true)
					{
						Move_From_Run_To_TRM(ptr);
						break;
					}
				}
				FCFSProcessor::ignoresigkill();
			}

			////////////////////////////////processor overheat////////////////////////////////////////////////
			//generate random number to check if it lies inside 5% overheat probability
			if (NTotal > 1 && NFCFS > 1) {
				int random = 0;
				random = (rand() % 100) + 1;
				if (random <= 5)
				{
					int rand1 = (rand() % (NTotal-1)) +1;
					if (rand1 < NTotal) {
						arr[rand1]->overheating(NStop);
						ProcessorOverheat(rand1);
					}

				}
				for (int i = 0; i < NTotal; i++)
				{
					if (arr[i]->getstate() == STOP)
						arr[i]->decrementStopTime();
				}
			}

			////////////////////////////////Stealing////////////////////////////////////////////////
	        // stealing each stl timestep
			if (time_step % STL ==0) {
				Stealing();
			}

			////////////////////////////////Blk To Rdy + Schedule Algo////////////////////////////////////////////////
			 MovefromBLKtoRDY();
			for (int j = 0; j < NTotal; j++) {
				arr[j]->ScheduleAlgo(time_step,this);
			}
			
			////////////////////////////////Interactive Modes////////////////////////////////////////////////
			if (x == 1) {
				user.UpdateInterface(arr, NTotal, &Blklist, &Trmlist, time_step);
				system("pause");                                    // to wait for a key after each time step
			}
			else if (x == 2) {
				user.UpdateInterface(arr, NTotal, &Blklist, &Trmlist, time_step);
				Sleep(1000);                                       // to wait one second after each time step
			}
			time_step++;
		}
		if (x == 3)
		{
			cout << "Silent Mode..................       Simulation Starts..." << endl;
			cout << "Simulation ends, Output file created" << endl;
			system("pause");
		}

		////////////////////////////////Generate Output File////////////////////////////////////////////////
		writeOutputFile();                                         // create output file
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
    // get the index of shortest or longest indices

	int Scheduler::getLongestRDY() {                               // needed for stealing
		int max = -1000000; int index = 0;
		for (int i = 0; i <NTotal; i++)
		{
			if (arr[i]->getsumct() == max) {
				Nosteal = true;
			}

			if (arr[i]->getsumct() > max)
			{
				Nosteal = false;
				max = arr[i]->getsumct();
				index = i;
			}
		}
		return index;
	}

	int Scheduler::getShortestRDY()
	{
		int min = 1000000; int index = 0;
		for (int i = 0; i <NTotal; i++)
		{
			if (arr[i]->getsumct() == min) {
				Nosteal = true;
			}
			if (arr[i]->getsumct() < min && arr[i]->getstate()!=STOP) {
				Nosteal = false;
				min = arr[i]->getsumct();
				index = i;
			}
		}
		return index;
	}

	int Scheduler::getShortestFCFS()
	{
		int min = 10000000;
		int index = 0;
		for (int i = 0; i < NFCFS; i++)
		{
			if (arr[i]->getsumct() < min && arr[i]->getstate() != STOP)
			{
				min = arr[i]->getsumct();
				index = i;
			}
		}
		return index;
	}

	int Scheduler::getShortestSJF()
	{
		int min = 10000000;
		int index = 0;
		for (int i = NFCFS; i < NFCFS+NSJF; i++)
		{
			if (arr[i]->getsumct() < min && arr[i]->getstate() != STOP)
			{
				min = arr[i]->getsumct();
				index = i;
			}
		}
		return index;
	}

	int Scheduler::getShortestRR()
	{
		int min = 10000000;
		int index = 0;
		for (int i = NFCFS+NSJF; i < NTotal; i++)
		{
			if (arr[i]->getsumct() < min && arr[i]->getstate() != STOP)
			{
				min = arr[i]->getsumct();
				index = i;
			}
		}
		return index;
	}

	////////////////////////////////Migration/////////////////////////////////////////////////
	bool Scheduler::move_from_FCFS_to_RR(process* RunProcess)        // migration from FCFS to RR
	{
		k = getShortestRR();
		arr[k]->Move_To_Ready(RunProcess);
		return true;
	}

	bool Scheduler::move_from_RR_to_SJF(process* RunProcess)       // migration from RR to SJF
	{
		k = getShortestSJF();
		arr[k]->Move_To_Ready(RunProcess);
		return true;
	}

	////////////////////////////////Stealing///////////////////////////////////////////////
	//get steal limit to use it in stealing function
	float Scheduler::get_steal_limit(int m,int n) {
		float steal_limit;
		float max = arr[m]->getsumct();
		float min = arr[n]->getsumct();
		steal_limit = ((max - min) / max);
		return steal_limit;
	}


	void Scheduler::Stealing()               // stealing function
	{
		int min = getShortestRDY();
		if (Nosteal == false) {
			int max = getLongestRDY();
			if (Nosteal == false) {
				float t = get_steal_limit(max,min);
					while (t > 40 / 100)
					{
					   arr[max]->remove(ptr);
					   if (ptr->getparent() == nullptr) {
							if (ptr) {
								arr[min]->Move_To_Ready(ptr);
								StealedProcesses++;
							}
							else {
								return;
							}
							t = get_steal_limit(max, min);
					   }
					   else { 
						   arr[max]->Move_To_Ready(ptr);
						   return; }
					}
			}
		}
	}

	////////////////////////////////Move to TRM////////////////////////////////////////////////
	// used in process completion and in sigkill 
	void  Scheduler::Move_From_Run_To_TRM(process* &trmptr) {
		trmptr->setTT(time_step);
		Trmlist.enqueue(trmptr);
		trmptr->setState(TRM);
		trmptr->setTRT(time_step - trmptr->getAT());
		trmptr->setWT(trmptr->getTRT() - trmptr->getCT() + trmptr->get_remainingCT());
		SumWT += trmptr->getWT();
		SumRT += trmptr->getRT();
		SumTRT += trmptr->getTRT();
		if (time_step<=trmptr->getDeadline()) {
			BeforeDeadline++;
		}
		killOrphans(trmptr);
	}

	////////////////////////////////OutputFile////////////////////////////////////////////////
	// generate a file called "outputfile" and save data in it

	void Scheduler::writeOutputFile()
	{
		ofstream output_file("OutputFile.txt");
		if (output_file.is_open())
		{
			output_file << "TT" << "\t" << "PID" << "\t" << "AT" << "\t" << "CT" <<"\t"<< "IO_D" << "\t" << "WT" << "\t" << "RT" <<"\t" << "TRT" << endl;
			while (!Trmlist.isEmpty())
			{
				Trmlist.dequeue(ptr);
				output_file << ptr->getTT() << "\t" << ptr->getPID() << "\t" << ptr->getAT() << "\t" << ptr->getCT()-ptr->get_remainingCT() << "\t " << ptr->getTotalIOD() << "\t" << ptr->getWT() << "\t" << ptr->getRT() << "\t" << ptr->getTRT() << endl;
			}
			output_file << "Processes: " << Nprocesses << endl;
			output_file << "Avg WT =  " << SumWT/Nprocesses<< " " << ", Avg RT =  " << SumRT / Nprocesses<<" " << ", Avg TRT =  " << SumTRT / Nprocesses << endl;
			output_file << "Migration :" << "RTF =" << RRProcessor::getMigratedProcesses() / Nprocesses * 100 << " %\t" << ", MaxW= " << FCFSProcessor::getMigratedProcesses() / Nprocesses * 100 << " %" << endl;
			output_file << "Work steal%: " << StealedProcesses/Nprocesses*100 << "\t" << ", Forked Process: " << FCFSProcessor::getForkedProcesses() / Nprocesses * 100 << endl;
			output_file << "Killed Process: " << FCFSProcessor::getkilledprocesses() / Nprocesses * 100 << " %" << endl;
			output_file << "Processes before deadline: " << BeforeDeadline / Nprocesses * 100 << " %" << endl;
			output_file << "Processors: " << NTotal << "[" << NFCFS <<" " << "FCFS, " <<" " << NSJF <<" "<<"SJF, "<<" " << NRR<<" " << "RR]" << endl;
			output_file << "Processors Load" << endl;
			for (int i = 0; i < NTotal; i++)
			{
				output_file << "P" << (i + 1) << "=" << (arr[i]->gettotalbusytime() / SumTRT) * 100 << "\t";  // getProcessorLoad to be integrated (karim)
			}
			output_file << endl;
			output_file << "Processors Utiliz" << endl;
			for (int i = 0; i < NTotal; i++)
			{
				output_file << "P" << (i + 1) << "=" << (arr[i]->gettotalbusytime() / (arr[i]->gettotalbusytime() + arr[i]->gettotalidetime())) * 100 << "\t";  // getProcessorUtil to be integrated (karim)
			}
			output_file << endl;
			output_file << "Avg utilization =";
			output_file << gettotalPUtil() / NTotal << endl;
			output_file.close();
		}
		else
		{
			cout << "Error: Unable to open output file." << endl;
		}
	}

	////////////////////////////////Forking////////////////////////////////////////////////
	void Scheduler::forkChild(process* parent)
	{
		parent->setforkedBefore();
		Nprocesses++;
		process* child = new process(time_step, Nprocesses ,parent->get_remainingCT());
		if (parent->getforkedBefore() == 1) {
			parent->setChild1(child);
		}
		else if (parent->getforkedBefore() == 2) {
			parent->setChild2(child);
		}
		child->setParent(parent);
		k = getShortestFCFS();
		arr[k]->Move_To_Ready(child);
	}

	////////////////////////////////Killing Orphans////////////////////////////////////////////////
	// the function callad in termination to kill all the children
	void Scheduler::killOrphans(process* parent)
	{
		process* child1 = parent->getchild1();
		process* child2 = parent->getchild2();
		if (parent->getparent() && parent->getparent()->getchild1()==child1) {
			parent->getparent()->setChild1(nullptr);
		}
		else if (parent->getparent() && parent->getparent()->getchild2() == child2) {
			parent->getparent()->setChild2(nullptr);
		}
		if (child1 != nullptr) {
			for (int i = 0; i < NFCFS; i++)
			{
				if (arr[i]->Findprocess(child1) == true)
				{
					Move_From_Run_To_TRM(child1);
					break;
				}
			}
		}
		if (child2 != nullptr) {
			for (int i = 0; i < NFCFS; i++)
			{
				if (arr[i]->Findprocess(child2) == true)
				{
					Move_From_Run_To_TRM(child2);
					break;
				}
			}
		}
	}
	////////////////////////////////IO requests////////////////////////////////////////////////
	void Scheduler::MovefromRUNtoBLK(process* & ptr) {
			Blklist.enqueue(ptr);
			ptr->setState(BLK);
	}

	void Scheduler::MovefromBLKtoRDY()
	{
		if (Blklist.peek(ptr))
		{
			if (ptr->getBLKt() == ptr->getIOD())
			{////////////////move from blk to rdy///////////////////////////////////////////////////////////

				Blklist.dequeue(ptr);
				ptr->setIOD_IOR();
				int min = getShortestRDY();
				arr[min]->Move_From_BLK_To_Ready(ptr);
			}
			else
			{
				ptr->incrementBLKt();
			}
		}
	}
	//////////////////////////////////////////processor overheating/////////////////////////////////////////////////////////
	void Scheduler::ProcessorOverheat(int processorindex)
	{
		int min1 = getShortestRDY();
		int min2 = getShortestFCFS();
		if (arr[processorindex]->Move_From_Run(ptr)) {
			if (ptr->getparent() != nullptr) {
				arr[min2]->Move_To_Ready(ptr);
			}
			else {
				arr[min1]->Move_To_Ready(ptr);
			}
		}
		while (arr[processorindex]->remove(ptr))
		{
			if (ptr->getparent() != nullptr) {
				arr[min2]->Move_To_Ready(ptr);
			}
			else {
				arr[min1]->Move_To_Ready(ptr);
			}
		}
	}
	////////////////////////////////Some Getters////////////////////////////////////////////////
	float Scheduler::gettotalPUtil()
	{
		float sum = 0;
		for (int i = 0; i < NTotal; i++)
		{
			sum = sum + ((arr[i]->gettotalbusytime() / (arr[i]->gettotalbusytime() + arr[i]->gettotalidetime())) * 100);
		}
		return sum;
	}

	int Scheduler::getNRR() {
		return NRR;
	}

	int Scheduler::getNSJF() {
		return NSJF;
	}

	Scheduler :: ~Scheduler() {
		for (int i = 0; i < Trmlist.getsize(); i++) {
			Trmlist.dequeue(ptr);
			delete ptr;
		}
		for (int i = 0; i < NTotal; i++) {
			delete arr[i];
		}
	}