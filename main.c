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
  int ret = 0;
  int runningProcesses = P_COUNT;
  int quantum = 50;
  int wait = 50;
  int procsLoaded;
  char *logMessage = (char *)calloc(sizeof(char), 128);
  FILE *procsData;
  process *p;

  // Initialize the Data Structures
  process procs[P_COUNT];
  ui readyQ[P_COUNT];
  ui io[P_COUNT];
  ui cpu[1];

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


// Begin the Main Loop to Similuate the Scheduler
if(ret == 0){

// Sort the Priority Queue

// Update the stats of the process in the CPU. If finished, move to next
// destination. If there is nothing in the cpu, then take the
// the first item from the Priority Queue, put it in the CPU,
// and Shift all other items up in the queue


// Update all values and statistics for processes in the Ready Queue. 
// If any have waited more than "wait", then increment their priority.


// Update all values and statistics for processes waiting for IO
// If any are finished, move them to the ready queue.

}
free(logMessage);
return(ret);
}



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




