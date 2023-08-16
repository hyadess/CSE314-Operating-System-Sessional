#ifndef PARAM_H
#define PARAM_H

#include <bits/stdc++.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctime>
#include <mutex>
using namespace std;

#define PRINT_STATIONS 4
#define BIND_STATIONS 2
#define STAFF_NUMBER 2
extern int PRINTING_TIME;
extern int BINDING_TIME;
extern int READING_WRITING_TIME;
extern int NO_OF_STUDENTS;
extern int NO_OF_GROUPS;
extern clock_t start;
extern std::mutex printMutex;
struct Group
{
    int startStudent;
    int lastStudent;
    int groupNo;
};

struct StationInfo
{
    int studentID;
 
};

struct StaffInfo
{
    int staffNo;
 
};

struct GroupInfo
{
    int groupNo;
};


int findGroup(int studentID);
int findPrintStation(int studentID);
void takeInput();

void startClock();
double getCurrentTime();

#endif