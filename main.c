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
  int nready, nio, ncpu;
  int procsLoaded;
  char *logMessage = (char *)calloc(sizeof(char), 128);
  int moveCPU;
  FILE *procsData;
  process *p;
  os sys; 
  sys.quantum = QUANTUM;
  sys.wait = WAIT;

  // Initialize the Data Structures
  process procs[P_COUNT];
  ui readyQ[P_COUNT];
  ui IO[P_COUNT];
  ui CPU[1];
  ui *cpu;
  ui *rq;
  ui *io;
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
    p[readyQ[i]].curPrior = p[readyQ[i]].priority;
    logProcess(p[i], i);
  }
  nready = P_COUNT;
  nio = 0;
  ncpu = 0;
  
  // Begin the Main Loop to Similuate the Scheduler
  appendToLogfile("Main Loop will begin now.");
  if(ret == 0){
    while(nready+nio+ncpu >0){

    // Sort the Priority Queue
    // appendToLogfile("About to sort the Ready Q.");
    // mapping each array to an ui* variable makes passing them simpler
    rq = readyQ;
    io = IO;
    cpu = CPU;

    sortReadyQueue(rq, p,nready);
      // printRQ(rq, p, nready); //This is used for testing.

    // If there is nothing in the cpu, then take the
    // the first item from the Priority Queue, put it in the CPU,
    // and Shift all other items up in the queue
    if(ncpu == 0){
      cpu[0] = pop(rq, nready);
      nready--;
      ncpu++;
    }
    // Update the stats of the process in the CPU.
    moveCPU = updateCPU(cpu, p);
    
  


    // Update all values and statistics for processes in the Ready Queue. 
    // If any have waited more than "wait", then increment their priority.
    updateRQ(p, rq, nready);

    // Update all values and statistics for processes waiting for IO
    // If any are finished, move them to the ready queue.
    // printf("Updating IO. nio = %d, nready = %d\n",nio, nready);
    updateIO(p, io, &nio, rq, &nready);
    // printf("Updated IO. nio = %d, nready = %d\n",nio, nready);

    // Move CPU if Necessary
    switch(moveCPU){
      case -1: //readyQueue
        if((p[cpu[0]].cpuTotal+p[cpu[0]].ioTotal) % 1000 == 0){
          logProcess(p[cpu[0]], cpu[0]);
        }
        readyQ[nready++] = cpu[0];
        if(p[rq[0]].waitMax < p[rq[0]].wait || p[rq[0]].waitMax == 0){
          p[rq[0]].waitMax = p[rq[0]].wait;
        }
        if(p[rq[0]].waitMin > p[rq[0]].wait || p[rq[0]].waitMin == 0){
          p[rq[0]].waitMin = p[rq[0]].wait;
        }
        cpu[0] = pop(rq, nready--);
        p[cpu[0]].wait = 0;
        p[cpu[0]].curPrior = p[cpu[0]].priority;
      break;

      case 0:
      break;

      case 1: //IO
        if((p[cpu[0]].cpuTotal+p[cpu[0]].ioTotal) % 1000 == 0){  
          logProcess(p[cpu[0]], cpu[0]);
        }
        io[nio++] = cpu[0];
        cpu[0] = pop(rq, nready--);
        p[cpu[0]].wait = 0;
        p[cpu[0]].curPrior = p[cpu[0]].priority;
      break;
      case 2: //Done
        if((p[cpu[0]].cpuTotal+p[cpu[0]].ioTotal) % 1000 == 0){  
          logProcess(p[cpu[0]], cpu[0]);
        }
        if(nready > 0){
          cpu[0] = pop(rq, nready--);
          p[cpu[0]].wait = 0;
          p[cpu[0]].curPrior = p[cpu[0]].priority;
        }else{
          ncpu = 0;
        }
      break;
      }
      // printRQ(rq, p, nready);
      // printRQ(cpu, p, 1);
    }
  }
  printStats(p, sys);
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

void logProcess(process p, int position){
  char *logMessage = (char *)calloc(sizeof(char), 256);
  sprintf(logMessage, "Position: %d\npriority: %u\ncpu: %u\nio: %u\nruntime:%u\ncurPrior: %u\ncpuTotal: %u\nioTotal: %u\nwaitSum: %u\nwaitCount:%u\nwaitMin: %u\nwaitMax:%u\n", position, p.priority, p.cpu, p.io, p.runTime, p.curPrior, p.cpuTotal, p.ioTotal, p.waitSum, p.waitCount, p.waitMin, p.waitMax);
  appendToLogfile(logMessage);
  free(logMessage);

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
      // After getting a bunch of very ugly results, and spending much time scratching my head
      // I realized all the values need to be initialized before running the simulation. 
      // Since I didn't calloc the arrays, things aren't automatically zero.
      processArray[numberProcessesParsed].priority = priority;
      processArray[numberProcessesParsed].cpu = cpu;
      processArray[numberProcessesParsed].io = io;
      processArray[numberProcessesParsed].runTime = runTime;
      processArray[numberProcessesParsed].curCpu = 0;
      processArray[numberProcessesParsed].ioTotal = 0;
      processArray[numberProcessesParsed].cpuTotal = 0;
      processArray[numberProcessesParsed].wait = 0;
      processArray[numberProcessesParsed].waitCount = 0;
      processArray[numberProcessesParsed].waitMax = 0;
      processArray[numberProcessesParsed].waitMin = 0;
      processArray[numberProcessesParsed].waitSum = 0;
      processArray[numberProcessesParsed].curPrior = priority;
      processArray[numberProcessesParsed].curIo = 0;

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
      if(p[rq[j]].curPrior < p[rq[j+1]].curPrior){
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
  printf("The Ready Queue is:\n**POINTER#**\t**CURPRIORITY**\t**WAIT**\t**CURCPU**\t**\n");

  for(i=0; i<count; i++){
    printf("   %u  \t\t   %u  \t\t  %u  \t\t  %u \n", rq[i], p[rq[i]].curPrior, p[rq[i]].wait, p[rq[i]].curCpu);
  }
}

// Pops the first item off the array, shifts all other items down.
ui pop(ui *rq, int count){
  ui ret = rq[0];
  int i;
  for(i=0; i<count-1; i++){
    rq[i] = rq[i+1];
  }
  return(ret);
}

// Updates statistics for the process currently in the CPU.
// Moves that process to the IO Queue or if complete removes it.
// return indicates where process in CPU should go:
// -1->readyQueue, 0->stays in cpu, 1->IO, 2 complete
int updateCPU(ui *cpu, process *p){
  int ret = 0;
  p[cpu[0]].curCpu++;
  
  if(p[cpu[0]].curCpu >= p[cpu[0]].cpu){
    p[cpu[0]].cpuTotal += p[cpu[0]].cpu;
    p[cpu[0]].curCpu = 0;
    if(p[cpu[0]].cpuTotal + p[cpu[0]].ioTotal >= p[cpu[0]].runTime){
      ret = 2;
    }else{
      ret = 1;
    }
  }else if(p[cpu[0]].curCpu >= QUANTUM){
    ret = -1;
  }
  return(ret);
}

// Updates statistics for the processes in the ready queue
void updateRQ(process *p, ui *rq, int nready){
  int i;

  for(i=0; i<nready; i++){
    p[rq[i]].waitSum++;
    if(p[rq[i]].wait++ == 0){
      p[rq[i]].waitCount++;
    }
    if(p[rq[i]].wait >= WAIT){
      p[rq[i]].curPrior++;
    }
  }

}

// Updates statistics for processes awaiting IO
void updateIO(process *p, ui *io, int *nio, ui *rq, int *nready){
  int i;

  for(i=0 ; i<*nio ; i++){
    p[io[i]].curIo++;
    p[io[i]].ioTotal++;
    if(p[io[i]].curIo >= p[io[i]].io){
      p[io[i]].curIo = 0;
      // printf("Ready Queue Count Was: %d\n", *nready);
      swapToRQ(io, i, nio, rq, nready);
      // printf("Ready Queue is now: %d\n", *nready);
      *nio = *nio-1;
    }
  }
}

// Swaps a process from ReadyQueue to IOQueue
void swapToRQ(ui *io, int pos, int *nio, ui *rq, int *nready){
  int i;
  rq[*nready] = io[pos];
  *nready = *nready +1;
  for(i=pos; i<(*nio-1); i++){
    io[i] = io[i+1];
  }
}