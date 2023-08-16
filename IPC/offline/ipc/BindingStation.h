#ifndef BIND
#define BIND

#include "CommonFunctions.h"

extern sem_t binding ;


void* enteredBindStation(void* arg);

pthread_t bind(StationInfo *info);

void bindingStation();




#endif