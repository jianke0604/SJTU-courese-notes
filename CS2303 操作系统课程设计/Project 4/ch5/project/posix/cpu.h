// length of a time quantum
#define QUANTUM 10
#include "task.h"
extern int tid_value;
// run the specified task for the following time slice
void run(Task *task, int slice);