#include "Scheduler.h"
using namespace std;
ostream& operator << (ostream& output, process* pros)            // operator overloading of process
{
	output << pros->getPID() << ", ";
	return output;
}
int main() {
	Scheduler s;
	s.simple_simulator();
	return 0;
}























