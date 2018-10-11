// CS 4540 Fall '18 Assignment # 2
// Author: Chris Carlson
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "a2.h"



int main(int argc, char *argv[]){
// Initialize all the State Variables and Utility Variables.
  int ret = 0, i;
  int runningProcesses = P_COUNT;
  int nready, nio, ncpu;
  int wait = 50;
  int procsLoaded;
  char *logMessage = (char *)calloc(sizeof(char), 128);
  FILE *procsData;
  process *p;

  // Initialize the Data Structures
  process procs[P_COUNT];
  ui readyQ[P_COUNT];
  ui io[P_COUNT];
  ui CPU[1];
  ui *cpu;
  ui *rq;
// Read the File Data in to Initialize the Process Array
if(argc > 1){
	sprintf(logMessage, "Opening File %s to populate the Process Array \'procs\'.", argv[1]);
	appendToLogfile(logMessage);
	procsData = openDataFile(argv[1]);
  p = procs;
	procsLoaded = parseDataFile(procsData, p);
  sprintf(logMessage, "Loaded %d processes successfully into the processes array.", procsLoaded);
  appendToLogfile(logMessage);
	sprintf(logMessage, "Closing file %s.", argv[1]);
	appendToLogfile(logMessage);
	closeDataFile(procsData);

}else{
	ret = 1;
	printf("No FileName Given to populate the Processes Array. Exiting Now.\n");
}
// Initialize the Ready Queue by putting all items in.
appendToLogfile("Populating the ReadyQ");
for(i=0; i<P_COUNT ; i++){
  readyQ[i] = i;
}
nready = P_COUNT;
nio = 0;
ncpu = 0;
// Begin the Main Loop to Similuate the Scheduler
appendToLogfile("Main Loop will begin now.");
if(ret == 0){
  while(nready+nio+ncpu >0){

  // Sort the Priority Queue
  appendToLogfile("About to sort the Ready Q.");
  rq = readyQ;
  sortReadyQueue(rq, p,nready);
    // printRQ(rq, p, nready); //This is used for testing.

  // Update the stats of the process in the CPU. If finished, move to next
  // destination.
  if(ncpu == 0){
    cpu[0] = pop(rq, nready);
    nready--;
    ncpu++;
  }
  cpu = CPU;
  updateCPU(cpu, rq, p, &nready);
  
  // If there is nothing in the cpu, then take the
  // the first item from the Priority Queue, put it in the CPU,
  // and Shift all other items up in the queue


  // Update all values and statistics for processes in the Ready Queue. 
  // If any have waited more than "wait", then increment their priority.


  // Update all values and statistics for processes waiting for IO
  // If any are finished, move them to the ready queue.
  }
}
free(logMessage);
return(ret);
}


// Open the Data File. Returns a file pointer.
FILE *openDataFile(char *fileName){
  FILE *fp;
  char *logString = (char *)malloc(75*sizeof(char));
  sprintf(logString, "Opening Data File with Name: %s.", fileName);
  appendToLogfile(logString);
  fp = fopen(fileName, "r");
  free(logString);
  return(fp);
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


void closeDataFile(FILE *fp){
  fclose(fp);
}

//Returns the number of processes created.
int parseDataFile(FILE *dataFile, process *processArray){
  char buffer[4096];
  char *token;
  char *tokenCounter;
  int numberProcessesParsed = 0;
  int bytesRead;
  char *logMessage = (char *)calloc(sizeof(char), 128);
  unsigned int priority, cpu, io, runTime;

  bytesRead = fread(buffer, sizeof(char), sizeof(buffer), dataFile);
  //Continue to Read until all bytes read.
  while(bytesRead > 0){
    sprintf(logMessage, "Just read %d bytes from the dataFile.", bytesRead);
    appendToLogfile(logMessage);
    //Grab tokens delineated by newlines.
    token = strtok_r(buffer, "\n", &tokenCounter);
    while(token != NULL){
      sprintf(logMessage, "Processing next line of data:\n\r%s", token);
      appendToLogfile(logMessage);
    
      sscanf(token, "%u %u %u %u", &priority, &cpu, &io, &runTime);
      processArray[numberProcessesParsed].priority = priority;
      processArray[numberProcessesParsed].cpu = cpu;
      processArray[numberProcessesParsed].io = io;
      processArray[numberProcessesParsed].runTime = runTime;

      numberProcessesParsed++;
      token = strtok_r(NULL, "\n", &tokenCounter);
    }

    bytesRead = fread(buffer, sizeof(char), sizeof(buffer), dataFile);
  }

  sprintf(logMessage, "Finished Processing Data File. Populated %d processes in the process array.", numberProcessesParsed);
  appendToLogfile(logMessage);
  free(logMessage);
  return(numberProcessesParsed);
}

// Bubble sort based on the priority of the process. 
// Sorts the ready queue.
void sortReadyQueue(ui *rq, process *p, int count){
  char *logMessage = (char *)calloc(sizeof(char), 128);
  sprintf(logMessage, "Sorting Ready Queue with %d items.", count);
  int i, j;
  for(i=0; i<count-1; i++){
    for(j=0; j<count-i-1; j++){
      if(p[rq[j]].priority < p[rq[j+1]].priority){
        swapItems(&rq[j],&rq[j+1]);
      }
    }
  }
}

// Swaps the places of two items in the ready queue array. 
// Makes the sort algorithm more readable.
void swapItems(ui *first, ui *second){
	int temp = *first;
	*first = *second;
	*second = temp;
}

// Prints the Ready Queue to Standard out
void printRQ(ui *rq, process *p, int count){
  int i;
  printf("The Ready Queue is:\n**POINTER#**\t**PRIORITY**\n");

  for(i=0; i<count; i++){
    printf("   %u  \t\t\t  %u  \n", rq[i], p[rq[i]].priority);
  }
}

// Pops the first item off the array, shifts all other items down.
ui pop(ui *rq, int count){
  ui ret = rq[0];
  int i;
  for(i=0; i<count; i++){
    rq[i] = rq[i+1];
  }
  return(ret);
}

// Updates statistics for the proceess currently in the CPU.
// Moves that process to the IO Queue or if complete removes it.
// returns 0 or -1 depending on if the ready queue has been changed.
void updateCPU(ui *cpu, ui *rq, process *p, int *rqCount){
  int ret = 0;
  p[cpu[0]].curCpu++;
  
  if(p[cpu[0]].curCpu == p[cpu[0]].cpu){
    p[cpu[0]].cpuTotal += p[cpu[0]].cpu;
    p[cpu[0]].curCpu = 0;

  }
  return(ret);
}