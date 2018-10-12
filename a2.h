#ifndef A1_h
#define A1_H
// CS 4540 Fall '18 Assignment # 2
// Author: Bob Hardin
#define P_COUNT 48	// If were're going to define things, lets do it in the Header File.
#define LOG_FILE_NAME "scheduler.log" // Yeah well, I need a global log file... So Shoot Me.
#define QUANTUM 50 // As long as these values are being Hard Coded, Best Practice is to define them Globally. 
#define WAIT 50	// Same as above.

typedef unsigned int ui;
struct process_struct {
	ui priority; // never changes
	ui cpu;  // time in cpu before I/O
	ui io; // time I/O takes
	ui runTime; // how long process runs

	ui curCpu; // count of current time in CPU
	ui curIo;  // count of time waiting for I/O
	ui wait; // current count of time in wait queue
	ui curPrior; // adjusted for starvation
	ui cpuTotal; // sum of time in cpu
	ui ioTotal; // sum of time doing io
				// statistics
	ui waitSum; // total time in wait queue
	ui waitCount; // how many times in wait queue (for average)
	ui waitMin; // smallest time in wait queue
	ui waitMax; // longet time in wait queu
};
typedef struct process_struct process;

struct os_struct { ui quantum; ui wait; };
typedef struct os_struct os;

// a[] must have count of 48 valid structs 0-47 indexes
void printStats(process a[], os system);
void appendToLogfile(char *message);
FILE *openDataFile(char *fileName);
int parseDataFile(FILE *dataFile, process *processArray);
void closeDataFile(FILE *fp);
void sortReadyQueue(ui *rq, process *p, int count);
void swapItems(ui *first, ui *second);
void printRQ(ui *rq, process *p, int count);
ui pop(ui *rq, int count);
int updateCPU(ui *cpu, process *p);
void updateRQ(process *p, ui *rq, int nready);
void updateIO(process *p, ui *io, int *nio, ui *rq, int *nready);
void swapToRQ(ui *io, int pos, int *nio, ui *rq, int *nready);
#endif

