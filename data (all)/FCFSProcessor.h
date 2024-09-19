#pragma once
# include"processor.h"
struct sigkill {                           //struct for data of sigkill (PID of process to be killed / time to be killed at)
    int PID;
    int killtime;
};

class scheduler;

class FCFSProcessor : public processor {
    static int ForkingProbability;
    static int MaxW;
    static float MigratedMaxW;                                        // no. of migrated processes due to MaxW
    static float ForkedProcesses;
    static float killedprocesses;
    process* child;                                                  
    process* RunProcess;
    LinkedList <process*> rdyList;
    static sigkill* s;
    static LinkedQueue <sigkill*> killing;                            // queue of pointers to structs of sigkill
public:
    FCFSProcessor();
    static void LoadMaxW(ifstream& fin);                             // function to load MaxW
    static void LoadFP(ifstream& fin);                              // function to load forking probability
    static void LoadSigKills(ifstream& fin);                       // function to load sigkills
    processorState getstate();
    void Move_To_Ready(process* ptr);
    void Move_From_Ready_To_Run();
    void Move_From_Run_To_Ready();
    bool Move_From_Run(process*& ptr);
    void Move_From_BLK_To_Ready(process*& ptr);
    bool Move_From_Ready_To_TRM(int i,process* &Rptr);
    void print();
    int getlengthrdy();
    int getRun();
    int getsumct();
    static float getMigratedProcesses();
    static float getForkedProcesses();
    bool Findprocess(process* ptr);
    static float getkilledprocesses();
    static bool timetosigkill(int timestep);
    static void ignoresigkill();
    bool  tokill(int timestep, process*& ptr);
    void  ScheduleAlgo(int timestep, Scheduler* sptr);
    bool Requests(process*& ptr);
    bool remove(process*& ptr);
    float gettotalbusytime();
    float gettotalidetime();
    void overheating(int timestop); ///function that takes the stop time and processes all the needed things for overheating
    void decrementStopTime();
    ~FCFSProcessor();
};