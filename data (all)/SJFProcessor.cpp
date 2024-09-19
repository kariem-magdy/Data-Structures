#pragma once
#include"SJFProcessor.h"
#include"Scheduler.h"
    SJFProcessor::SJFProcessor(){
        state = IDLE;
        name = SJF;
        RunProcess = nullptr;
    }
    processorState  SJFProcessor::getstate() {
        return state;
    }
    void  SJFProcessor::Move_To_Ready(process* ptr) {
        rdyList.enqueue(ptr, 1 / (ptr->get_remainingCT()));
        ptr->setState(RDY);
        ptr->setIOD_IOR();
        sumCT = sumCT + ptr->get_remainingCT();                                         // update sum of cpu time to calculate shotest rdy list
    }
    void  SJFProcessor:: Move_From_Ready_To_Run() {
        if (state == IDLE && rdyList.getsize()>0) {
            rdyList.dequeue(Rptr);
            RunProcess = Rptr;
            Rptr->setState(RUN);
            state = BUSY;
            sumCT = sumCT - Rptr->get_remainingCT();                                         // update sum of cpu time to calculate shotest rdy list
        }
    }
    void  SJFProcessor:: Move_From_Run_To_Ready() {
            rdyList.enqueue(RunProcess,  1/(RunProcess->get_remainingCT()));
            RunProcess->setState(RDY);
            sumCT = sumCT + RunProcess->get_remainingCT();                                         // update sum of cpu time to calculate shotest rdy list
            RunProcess = nullptr;
            state = IDLE;
    }
    bool SJFProcessor::Move_From_Run(process*& ptr) {
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
    void  SJFProcessor::Move_From_BLK_To_Ready(process* & ptr) {
        rdyList.enqueue(ptr,  1/(ptr->get_remainingCT()));
        ptr->setState(RDY);
        sumCT = sumCT + ptr->get_remainingCT();                                         // update sum of cpu time to calculate shotest rdy list
    }
    void  SJFProcessor:: print()
    {
        rdyList.print();
    }
    int SJFProcessor:: getlengthrdy()
    {
        return rdyList.getsize();
    }
    int  SJFProcessor:: getRun() {
        return RunProcess->getPID();
    }
    bool  SJFProcessor::Move_From_Ready_To_TRM(int i, process*& Rptr) {
        return false;
    }
    bool SJFProcessor::Findprocess(process* ptr)
    {
        return false;
    }
    int SJFProcessor::getsumct() {
        return sumCT;
    }
    bool SJFProcessor::tokill(int timestep, process*& ptr)
    {
        return false;
    }
    void  SJFProcessor::ScheduleAlgo(int timestep, Scheduler* sptr) {
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
            Move_From_Ready_To_Run();
            if (state == BUSY && RunProcess->get_remainingCT() != 0) {
                RunProcess->set_remianingCT(RunProcess->get_remainingCT() - 1);
            }
            if (state == BUSY && RunProcess->getCT() - RunProcess->get_remainingCT() == 1) {
                RunProcess->setRT(timestep - RunProcess->getAT());
            }
            if (state == BUSY)
                totalbusytime++;
            else
                totalidealTime++;
        }
    }
    bool SJFProcessor::Requests(process*& ptr)
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
    bool SJFProcessor::remove(process*& ptr)
    {
        if (!rdyList.isEmpty())
        {
            rdyList.dequeue(ptr);
            sumCT = sumCT - ptr->get_remainingCT();                          // update sum of cpu time to calculate shotest rdy list
            return true;
        }
        return false;
    }
    void SJFProcessor::overheating(int timestop) ///function that takes the stop time and processes all the needed things for overheating
    {
        state = STOP;
        stop_time = timestop;

    }
    void SJFProcessor::decrementStopTime()
    {
        if (state == STOP)
            stop_time--;
        if (stop_time == 0)
            state = IDLE;
    }
    float SJFProcessor::gettotalbusytime()
    {
        return totalbusytime;
    }
    float SJFProcessor::gettotalidetime()
    {
        return totalidealTime;
    }
    SJFProcessor:: ~SJFProcessor() {}
