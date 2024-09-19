#pragma once
#include<iostream>
#include <stdlib.h>
#include "processor.h"
using namespace std;
class UI
{
private:
	int Runcount;
public:
	void UpdateInterface(processor** arr, int NoP, LinkedQueue<process*>* BLK, LinkedQueue<process*>*TRM, int timestep)
	{
		Runcount = 0;
		cout << "Current Timestep: " << timestep << endl;
		cout << "----------------------------- RDY Processes -------------------------------------" << endl;
		for (int i = 0; i < NoP; i++)
		{
			cout << "Processor " << i+1 << "[ " << *(arr[i]) << " ] :" << arr[i]->getlengthrdy() << " RDY  "; //to be used to print the num of ready processes
			arr[i]->print(); //printing readylist 
			if (arr[i]->getstate() == STOP)
				cout << "Processor Overheated";
			cout << endl;
		}
		cout << "----------------------------- BLK Processes -------------------------------------" << endl;
		cout << BLK->getsize() << " BLK: ";
		BLK->print();
		cout << endl;
		cout << "----------------------------- RUN Processes -------------------------------------" << endl;
		for (int i = 0; i < NoP; i++)
		{
			if (arr[i]->getstate() == BUSY)
				Runcount++;
		}
		
		cout << Runcount << " RUN: ";
		for (int i = 0; i < NoP; i++)
		{
			if (arr[i]->getstate() == BUSY)
			{
				cout << arr[i]->getRun() << "(P" << i+1 << ") ";
				if (i < NoP - 1)
					cout << ",";
			}
		}
		cout << endl;
		cout << "----------------------------- TRM Processes -------------------------------------" << endl;
		cout << TRM->getsize() << " TRM: ";
		TRM->print();
		cout << endl;
		//system("CLS");
	}
	
};