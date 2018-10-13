#include <stdio.h>
#include "a2.h"
// CS 4540 Fall '18 Assignment # 2
// Author: Bob Hardin

// a[] must have P_COUNT number of valid structs normally 48 or 0-47 indexes
void myPrintStats(process a[], os system) {
	int i, j;
	struct averages { double average; ui priority; } avg, avgs[P_COUNT];
	printf("\nTime quantum %u,  Maximum Wait %u\n", system.quantum, system.wait);
	printf("\nProcess#  Average Wait   Priority\n");
	for (i = 0; i < P_COUNT; i++) {
		avg.priority = a[i].priority;
		avg.average = (double)a[i].waitSum / (double)a[i].waitCount;
		// sorted insert
		j = i - 1;
		while (j >= 0 && avgs[j].average > avg.average) {
			avgs[j + 1] = avgs[j];
			j--;
		}
		avgs[j + 1] = avg;
	} // end for each process
	for (i = 0; i < P_COUNT; i++) {
		printf("%d      %12.2lf   %8u\n", i, avgs[i].average, avgs[i].priority);
	}
}
