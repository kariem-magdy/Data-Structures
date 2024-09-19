#pragma once
#include"EDFProcessor.h"
#include"Scheduler.h"
EDFProcessor::EDFProcessor() {
    state = IDLE;
    name = EDF;
    RunProcess = nullptr;
}

processorState  EDFProcessor::getstate() {
    return state;
}
void  EDFProcessor::Move_To_Ready(process* ptr) {
    rdyList.enqueue(ptr, 1 / (ptr->getDeadline()));
    ptr->setState(RDY);
    ptr->setIOD_IOR();
    sumCT = sumCT + ptr->get_remainingCT();                                         // update sum of cpu time to calculate shotest rdy list
}
void  EDFProcessor::Move_From_Ready_To_Run() {
    //  process* Rptr;
    if (state == IDLE && rdyList.getsize() > 0) {
        rdyList.dequeue(Rptr);
        RunProcess = Rptr;
        Rptr->setState(RUN);
        state = BUSY;
        sumCT = sumCT - Rptr->get_remainingCT();                                         // update sum of cpu time to calculate shotest rdy list
    }
}
void  EDFProcessor::Move_From_Run_To_Ready() {
    rdyList.enqueue(RunProcess, 1 / (RunProcess->getDeadline()));
    RunProcess->setState(RDY);
    sumCT = sumCT + RunProcess->get_remainingCT();                                         // update sum of cpu time to calculate shotest rdy list
    RunProcess = nullptr;
    state = IDLE;
}
bool EDFProcessor::Move_From_Run(process*& ptr) {
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
void  EDFProcessor::Move_From_BLK_To_Ready(process*& ptr) {
    rdyList.enqueue(ptr, 1 / (ptr->getDeadline()));
    ptr->setState(RDY);
    sumCT = sumCT + ptr->get_remainingCT();                                         // update sum of cpu time to calculate shotest rdy list
}
void  EDFProcessor::print()
{
    rdyList.print();
}
int EDFProcessor::getlengthrdy()
{
    return rdyList.getsize();
}
int  EDFProcessor::getRun() {
    return RunProcess->getPID();
}
bool  EDFProcessor::Move_From_Ready_To_TRM(int i, process*& Rptr) {
    return false;
}
bool EDFProcessor::Findprocess(process* ptr)
{
    return false;
}
int EDFProcessor::getsumct() {
    return sumCT;
}
bool EDFProcessor::tokill(int timestep, process*& ptr)
{
    return false;
}
void  EDFProcessor::ScheduleAlgo(int timestep, Scheduler* sptr) {
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
        if (rdyList.peek(Rptr)) {
            if (state == BUSY && Rptr->getDeadline() < RunProcess->getDeadline())
            {
                Move_From_Run_To_Ready();
            }
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
bool EDFProcessor::Requests(process*& ptr)
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
bool EDFProcessor::remove(process*& ptr)
{
    if (!rdyList.isEmpty())
    {
        rdyList.dequeue(ptr);
        sumCT = sumCT - ptr->get_remainingCT();                          // update sum of cpu time to calculate shotest rdy list
        return true;
    }
    return false;
}
void EDFProcessor::overheating(int timestop) ///function that takes the stop time and processes all the needed things for overheating
{
    state = STOP;
    stop_time = timestop;

}
void EDFProcessor::decrementStopTime()
{
    if (stop_time == 0)
        state = IDLE;
    if (state == STOP)
        stop_time--;
}
float EDFProcessor::gettotalbusytime()
{
    return totalbusytime;
}
float EDFProcessor::gettotalidetime()
{
    return totalidealTime;
}
EDFProcessor:: ~EDFProcessor() {}
