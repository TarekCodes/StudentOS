#include <filesystem>
#include "PCB.h"
#include "Memory.h"
#include <iostream>


std::vector<PCB> vec;
Memory memory;
void siodisk(int jobnum);
void siodrum(int jobnum, int jobsize, int coreaddress, int direction);
void ontrace(); // called without arguments
void offtrace(); // called without arguments 

void startup()
{
	// Allows initialization of static system variables declared above.
	// Called once at start of the simulation.
}

// INTERRUPT HANDLERS
// The following 5 functions are the interrupt handlers. The arguments 
// passed from the environment are detailed with each function below.
// See RUNNING A JOB, below, for additional information
void Crint(int &a, int p[])
{
	// Indicates the arrival of a new job on the drum.
	// At call: p [1] = job number
	// p [2] = priority
	// p [3] = job size, K bytes
	// p [4] = max CPU time allowed for job
	// p [5] = current time
	PCB temp(p[1], p[2], p[3], p[4], -1);
	if (memory.insertNewJob(temp)) {
		vec.push_back(temp);
		siodrum(temp.getJobNumber(), temp.getJobSize(), temp.getMemoryPos(), 0);
	}
}
void Dskint(int &a, int p[])
{
	// Disk interrupt.
	// At call: p [5] = current time
}
void Drmint(int &a, int p[])
{
	p[2] = 0;
	p[3] = vec[0].getJobSize();
	p[4] = vec[0].getMaxCPUTime();
	a = 2;

	// Drum interrupt.
	// At call: p [5] = current time
}
void Tro(int &a, int p[])
{
	// Timer-Run-Out.
	// At call: p [5] = current time
}
void Svc(int &a, int p[])
{
	// Supervisor call from user program.
	// At call: p [5] = current time
	// a = 5 => job has terminated
	// a = 6 => job requests disk i/o
	// a = 7 => job wants to be blocked until all its pending
	// I/O requests are completed
}


FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}