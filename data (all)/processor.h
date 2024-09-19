#pragma once
#include"linkedList.h"
#include"PriorityQueue.h"
#include"process.h"
#include"LinkedQueue.h"

class Scheduler;                                          // forward declaration of class scheduler to avoid infinite loops of include

enum processorState {
    BUSY,
    IDLE,
    STOP   ///PROCESSOR OVERHEATING STATE 
};
enum processorName {  
    FCFS,
    SJF,
    RR,
    EDF
};
class processor {
protected:
    Scheduler* sptr;                                                // pointer on scheduler to call functions in it
    processorState state;
    processorName name;
    int totalbusytime;
    int totalidealTime;
    int sumCT;                                                     // sum of CT of all processes = expected finish time
    int stop_time;                                                  // time the processor is in STOP state
    process* Rptr;
public:
    processor() {
    }
    friend ostream& operator <<(ostream& output, const processor& pros)          // operator overloading to print processor's names
    {
        if (pros.name == FCFS)
            output << "FCFS";
        else if (pros.name == SJF)
            output << "SJF";
        else if(pros.name==RR)
            output << "RR";
        else
            output << "EDF";
        return output;
    }
    virtual void  ScheduleAlgo(int timestep, Scheduler* sptr) = 0;
    virtual processorState getstate() = 0;                                   // return state of processor
    virtual void Move_To_Ready(process* ptr) = 0;                           // add process to ready queue
    virtual void Move_From_Ready_To_Run() = 0;
    virtual void Move_From_Run_To_Ready() = 0;
    virtual bool Move_From_Run(process* & ptr) = 0;          // remove the process in the run state
    virtual void Move_From_BLK_To_Ready(process* & ptr) = 0;
    virtual  bool Move_From_Ready_To_TRM(int i, process*& Rptr) = 0;
    virtual void print() = 0;                                            //print the rdy list
    virtual int getlengthrdy() = 0;                                     //return size of rdy list 
    virtual int getRun() = 0;
    virtual int getsumct()=0;                                          //return expected finish time
    virtual bool  tokill(int timestep, process*& ptr) = 0;            // search for the process in rdy or run to kill it
    virtual bool Requests(process*& ptr) = 0;                         // check the condition to move from run to blk
    virtual bool Findprocess(process* ptr) = 0;                       // search for a certain process in rdy list of FCFS processors
    virtual bool remove(process*& ptr) = 0;                          // remove a process from  rdy list
    virtual float gettotalbusytime()=0 ;
    virtual float gettotalidetime()=0;
    virtual void overheating(int timestop) = 0; ///function that takes the stop time and processes all the needed things for overheating
    virtual void decrementStopTime() = 0;

};
