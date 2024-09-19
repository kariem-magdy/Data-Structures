#pragma once
#include "RRProcessor.h"
#include "Scheduler.h"
RRProcessor::RRProcessor() {
            state = IDLE;
            name = RR;
            RunProcess = nullptr;
        }
void RRProcessor::loadTS(ifstream& fin){
    fin >> TimeSlice;
}
 void RRProcessor:: loadRTF(ifstream& fin) {
    fin >> RTF;
}
 int RRProcessor::TimeSlice = 0;
 int RRProcessor::RTF = 0;
 float RRProcessor::MigratedRTF = 0;
processorState RRProcessor::getstate() {
    return state;
}
void RRProcessor::Move_To_Ready(process* ptr) {
    rdyList.enqueue(ptr);
    ptr->setState(RDY);
    ptr->setIOD_IOR();
    sumCT = sumCT + ptr->get_remainingCT();                                         // update sum of cpu time to calculate shotest rdy list
}
void RRProcessor::Move_From_Ready_To_Run() {
   // process* Rptr;
    if (state == IDLE && rdyList.getsize()>0) {
        rdyList.dequeue(Rptr);
        RunProcess = Rptr;
        Rptr->setState(RUN);
        state = BUSY;
        k = (RunProcess->get_remainingCT()) - TimeSlice;
        sumCT = sumCT - Rptr->get_remainingCT();                                         // update sum of cpu time to calculate shotest rdy list
    }
}
void RRProcessor:: Move_From_Run_To_Ready() {
    rdyList.enqueue(RunProcess);
    RunProcess->setState(RDY);
    sumCT = sumCT + RunProcess->get_remainingCT();                                         // update sum of cpu time to calculate shotest rdy list
    RunProcess = nullptr;
    state = IDLE;
}
bool RRProcessor::Move_From_Run(process*& ptr) {
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
void RRProcessor:: Move_From_BLK_To_Ready(process*& ptr) {
    rdyList.enqueue(ptr);
    ptr->setState(RDY);
    sumCT = sumCT + ptr->get_remainingCT();                                         // update sum of cpu time to calculate shotest rdy list
}
void RRProcessor::print()
{
    rdyList.print();
}
int RRProcessor::getlengthrdy()
{
    return rdyList.getsize();
}
int RRProcessor::getRun() {
    return RunProcess->getPID();
}
bool RRProcessor:: Move_From_Ready_To_TRM(int i, process*& Rptr) {
    return false;
}
int RRProcessor::getsumct() {
    return sumCT;
}
float RRProcessor:: getMigratedProcesses() {
    return MigratedRTF;
}
bool RRProcessor::Findprocess(process* ptr)
{
    return false;
}
bool RRProcessor::tokill(int timestep, process*& ptr)
{
    return false;
}
void  RRProcessor::ScheduleAlgo(int timestep, Scheduler* sptr) {
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
        if (state == BUSY && RunProcess->get_remainingCT() == k) {
            Move_From_Run_To_Ready();
        }
        if (state == IDLE && sptr->getNSJF() > 0) {
            bool z = true;
            while (z)
            {
                Move_From_Ready_To_Run();
                if (state == BUSY && RunProcess->get_remainingCT() < RTF)     // Condition of migration
                {
                    MigratedRTF++;
                    z = sptr->move_from_RR_to_SJF(RunProcess);
                    RunProcess = nullptr;
                    state = IDLE;
                }
                else
                {
                    z = false;
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
        if (state == BUSY)
            totalbusytime++;
        else
            totalidealTime++;
    }
}
bool RRProcessor::Requests(process*& ptr)
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
bool RRProcessor::remove(process*& ptr)
{
    if (!rdyList.isEmpty())
    {
        rdyList.dequeue(ptr);
        sumCT = sumCT - ptr->get_remainingCT();                          // update sum of cpu time to calculate shotest rdy list
        return true;
    }
    return false;
}
void RRProcessor::overheating(int timestop) ///function that takes the stop time and processes all the needed things for overheating
{
    state = STOP;
    stop_time = timestop;

}
void RRProcessor::decrementStopTime()
{
    if (state == STOP)
        stop_time--;
    if (stop_time == 0)
        state = IDLE;
}
float RRProcessor::gettotalbusytime()
{
    return totalbusytime;
}
float RRProcessor::gettotalidetime()
{
    return totalidealTime;
}
RRProcessor::~RRProcessor() {}