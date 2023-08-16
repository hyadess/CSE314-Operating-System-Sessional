#ifndef LIB
#define LIB

#include "CommonFunctions.h"

extern int readerCount;
extern int submissionCount;
extern sem_t readerCountSemaphore;
extern sem_t bookUpdateSemaphore;




void *staff(void *arg);

void* createStaff(void* arg);




pthread_t LibraryInit();


void *submitProc(void *arg);



pthread_t submit(int groupNo);



#endif