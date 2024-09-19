#pragma once
#include "FCFSProcessor.h"
#include"Scheduler.h"
int FCFSProcessor::MaxW = 0;
int FCFSProcessor::ForkingProbability = 0;
float FCFSProcessor::killedprocesses = 0;
float FCFSProcessor::MigratedMaxW = 0;
float FCFSProcessor::ForkedProcesses = 0;
sigkill* FCFSProcessor::s=nullptr;
LinkedQueue <sigkill*>  FCFSProcessor::killing;
    FCFSProcessor::FCFSProcessor() {
        state = IDLE;
        name = FCFS;
        RunProcess = nullptr;
        sumCT = 0;
    }
    void  FCFSProcessor:: LoadMaxW(ifstream& fin) {
        fin >> MaxW;
    }
    void  FCFSProcessor:: LoadFP(ifstream& fin) {
        fin >> ForkingProbability;
    }
    void  FCFSProcessor::LoadSigKills(ifstream& fin) {      // load lines of sigkill in a static function to be constant for all FCFS processors
        while (!fin.eof()) {
            s = new sigkill;                                         // make a new sigkill
            fin >> s->killtime >> s->PID ;
            killing.enqueue(s);                                
        }
    }
    processorState FCFSProcessor::getstate() {
        return state;
    }
    void  FCFSProcessor::Move_To_Ready(process* ptr) {
        rdyList.insertend(ptr);
        ptr->setState(RDY);
        ptr->setIOD_IOR();
        sumCT = sumCT + ptr->get_remainingCT();                                         // update sum of cpu time to calculate shotest rdy list
    }
    void FCFSProcessor::Move_From_Ready_To_Run() {
       // process* Rptr;
        if (state == IDLE && rdyList.getsize()>0) {
                rdyList.deletefirst(Rptr);
                RunProcess = Rptr;
                Rptr->setState(RUN);
                state = BUSY;
                sumCT = sumCT - Rptr->get_remainingCT();                              // update sum of cpu time to calculate shotest rdy list
        }
    }
    void  FCFSProcessor::Move_From_Run_To_Ready() {
            rdyList.insertend(RunProcess);
            RunProcess->setState(RDY);
            sumCT = sumCT + RunProcess->get_remainingCT();                           // update sum of cpu time to calculate shotest rdy list
            RunProcess = nullptr;
            state = IDLE;
    }
  
    bool FCFSProcessor::Move_From_Run(process*& ptr) {
        if (RunProcess) {
            ptr = RunProcess;
            RunProcess = nullptr;
            if (state != STOP) {
                state = IDLE;
            }
            return true;
        }
        else
            return false;
    }
    void FCFSProcessor::Move_From_BLK_To_Ready(process* & ptr) {
        rdyList.insertend(ptr);
        ptr->setState(RDY);
        sumCT = sumCT + ptr->get_remainingCT();                                    // update sum of cpu time to calculate shotest rdy list
    }
    bool FCFSProcessor:: Move_From_Ready_To_TRM(int r,process* & Rpt) {
        for (int i = 1; i <= rdyList.getsize(); i++) {
            Rpt = rdyList.getpointer(i);
            if (Rpt != nullptr && Rpt->getPID() == r) {
                rdyList.remove(Rpt);
                sumCT = sumCT - Rpt->get_remainingCT();                          // update sum of cpu time to calculate shotest rdy list
                    return true;
            }
        }
        return false;
    }
    void FCFSProcessor::print()
    {
        rdyList.print();
    }
    int FCFSProcessor:: getlengthrdy()
    {
       return  rdyList.getsize();
    }
    int FCFSProcessor:: getRun() {
        return RunProcess->getPID();
    }
    int FCFSProcessor::getsumct() {
        return sumCT;
    }
    float FCFSProcessor::getMigratedProcesses() {
        return MigratedMaxW;
    }
    float FCFSProcessor::getkilledprocesses() {
        return killedprocesses;
    }
    float FCFSProcessor::getForkedProcesses() {
        return ForkedProcesses;
    }
    bool FCFSProcessor::timetosigkill(int timestep) {

        killing.peek(s);
        if (s->killtime == timestep)
        {
            return true;
        }
        return false;
    }
    bool FCFSProcessor::Findprocess(process* ptr)
    {
        if (ptr == RunProcess)
        {
            RunProcess = nullptr;
            state = IDLE;
            return true;
        }
        else
        {
            return  Move_From_Ready_To_TRM(ptr->getPID(), ptr);
        }
        return false;
    }
    void FCFSProcessor::ignoresigkill() {
        killing.dequeue(s);
    }
    bool FCFSProcessor::tokill(int timestep, process*& ptr)
    {
        killing.peek(s);
        bool x;
            x = Move_From_Ready_To_TRM(s->PID, ptr);
            if (x) {
                killedprocesses++;
                return true;
            }

            if (state==BUSY && RunProcess->getPID() == s->PID)
            {
                Move_From_Run(ptr);
                killedprocesses++;
                return true;
            }
            return false; //ignore sigkill at this processor as it is its timestep and is not found in RUN or RDY state;
        }
    void  FCFSProcessor::ScheduleAlgo(int timestep, Scheduler* sptr){
        if (state != STOP)
        {
            if (state == BUSY && RunProcess->get_remainingCT() == 0) {
                state = IDLE;
                sptr->Move_From_Run_To_TRM(RunProcess);
                RunProcess = nullptr;
            }
            if (Requests(Rptr)) {
                sptr->MovefromRUNtoBLK(Rptr);
            }
            if (state == IDLE && sptr->getNRR() > 0) {
                bool B = true;
                while (B)
                {
                    Move_From_Ready_To_Run();
                    if (RunProcess) {
                        RunProcess->setWT(timestep - RunProcess->getAT() - RunProcess->getCT() + RunProcess->get_remainingCT());
                    }
                    if (state == BUSY && RunProcess->getparent() == nullptr && RunProcess->getWT() > MaxW)     // Condition of migration
                    {
                        MigratedMaxW++;
                        B = sptr->move_from_FCFS_to_RR(RunProcess);
                        RunProcess = nullptr;
                        state = IDLE;
                    }
                    else
                    {
                        B = false;
                    }
                }
            }
            else
                Move_From_Ready_To_Run();
            if (state == BUSY && RunProcess->get_remainingCT() != 0) {
                RunProcess->set_remianingCT(RunProcess->get_remainingCT() - 1);
            }
            if (state == BUSY && RunProcess->getCT() - RunProcess->get_remainingCT() == 1) {
                RunProcess->setRT(timestep - RunProcess->getAT());
            }
            int random1 = (rand() % 100) + 1;
            if (state == BUSY && RunProcess->getforkedBefore() < 2 && random1 > (100 - ForkingProbability)) {
                ForkedProcesses++;
                sptr->forkChild(RunProcess);
            }
            if (state == BUSY)
                totalbusytime++;
            else
                totalidealTime++;
        }
    }
    bool FCFSProcessor::Requests(process*& ptr)
    {
        if (RunProcess)
        {
            if (RunProcess->getIOR() == RunProcess->getCT() - RunProcess->get_remainingCT())
            {
                ptr = RunProcess;
                RunProcess = nullptr;
                state = IDLE;
                ptr->setBLKt();
                return true;
            }
        }
        return false;
    }

    bool FCFSProcessor::remove(process*& ptr)
    {
        if (!rdyList.isEmpty())
        {
            rdyList.deletefirst(ptr);
            sumCT = sumCT - ptr->get_remainingCT();                          // update sum of cpu time to calculate shotest rdy list
            return true;
        }
        return false;
    }
    void FCFSProcessor::overheating(int timestop) ///function that takes the stop time and processes all the needed things for overheating
    {
        state = STOP;
        stop_time = timestop;

    }
    void FCFSProcessor::decrementStopTime()
    {
        if (state == STOP)
            stop_time--;
        if (stop_time == 0)
            state = IDLE;
    }
    float FCFSProcessor::gettotalbusytime()
    {
        return totalbusytime;
    }
     float FCFSProcessor::gettotalidetime()
    {
        return totalidealTime;
    }
    FCFSProcessor::~FCFSProcessor() {}