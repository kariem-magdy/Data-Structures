#pragma once
#include"processor.h"
class scheduler;
class EDFProcessor : public processor {
private:
    process* RunProcess;
    PriorityQueue <process*> rdyList;           // A priority queue to arrange processes according to deadline
public:
    EDFProcessor();
    processorState getstate();
    void Move_To_Ready(process* ptr);
    void Move_From_Ready_To_Run();
    void Move_From_Run_To_Ready();
    bool Move_From_Run(process*& ptr);
    void Move_From_BLK_To_Ready(process*& ptr);
    void print();
    int getlengthrdy();
    int getRun();
    bool Move_From_Ready_To_TRM(int i, process*& Rptr);
    int getsumct();
    bool  tokill(int timestep, process*& ptr);
    bool Findprocess(process* ptr);
    void  ScheduleAlgo(int timestep, Scheduler* sptr);
    bool Requests(process*& ptr);
    bool remove(process*& ptr);
    float gettotalbusytime();
    float gettotalidetime();
    void overheating(int timestop); ///function that takes the stop time and processes all the needed things for overheating
    void decrementStopTime();
    ~EDFProcessor();
};
