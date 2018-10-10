// CS 4540 Fall '18 Assignment # 2
// Author: Chris Carlson
#include <stdio.h>
#include <time.h>
#include "a2.h"



int main(int argc, char *argv[]){
// Initialize all the State Variables
int ret = 0;
int runningProcesses = P_COUNT;
int quantum = 50;
int wait = 50;

// Initialize the Data Structures
process procs[P_COUNT];
ui readyQ[P_COUNT];
ui io[P_COUNT];
ui cpu[1];

// Read the File Data in to Initialize the Process Array

// Initialize the Ready Queue by putting all items in.


// Begin the Main Loop to Similuate the Scheduler


// Sort the Priority Queue

// Update the stats of the process in the CPU. If finished, move to next
// destination. If there is nothing in the cpu, then take the
// the first item from the Priority Queue, put it in the CPU,
// and Shift all other items up in the queue


// Update all values and statistics for processes in the Ready Queue. 
// If any have waited more than "wait", then increment their priority.


// Update all values and statistics for processes waiting for IO
// If any are finished, move them to the ready queue.



return(ret);
}



// Takes a Message, Writes it to the LogFile.
// This method of timestamping the Log File was inspired by the code located at
// stack overflow here: https://stackoverflow.com/questions/7411301/how-to-introduce-date-and-time-in-log-file
void appendToLogfile(char *message){
  FILE *logFile = fopen(LOG_FILE_NAME, "a");
  char timeStamp[20];
  struct tm *timeStruct;
  time_t now = time(0);
  timeStruct = gmtime(&now);
  strftime(timeStamp, sizeof(timeStamp), "%Y-%m-%d %H:%M:%S", timeStruct);

  fprintf(logFile, "%s: %s\n", timeStamp, message);
  fclose(logFile);
}



