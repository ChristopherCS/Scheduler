#ifndef A1_h
#define A1_H
// CS 4540 Fall '18 Assignment # 2
// Author: Bob Hardin
#define P_COUNT 48	// If were're going to define things, lets do it in the Header File.
#define LOG_FILE_NAME "Scheduler.log" // Yeah well, I need a global log file... So Shoot Me.

typedef unsigned short ui;
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
int parseDataFile(FILE *dataFile, process *processArray[48]);
void closeDataFile(FILE *fp);


#endif

