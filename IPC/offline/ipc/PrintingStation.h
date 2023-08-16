#ifndef PRINT
#define PRINT

#include "CommonFunctions.h"

extern vector<pair<int, sem_t*> *> waiting_list[PRINT_STATIONS + 1]; /// students and their corresponding semaphores.....
extern sem_t stations[PRINT_STATIONS + 1];

/// semaphore for the waiting list so that the running thread and a new incoming thread do not access the
/// queue at a time............
extern sem_t list_sem[PRINT_STATIONS + 1];

void usingPrinter(int studentID);

void signalOthers(int studentID);

void *enteredStationFromWaitingRoom(void *arg);

void *enteredStation(void *arg);

void PrintingStation();

pthread_t enter(StationInfo* info);

#endif