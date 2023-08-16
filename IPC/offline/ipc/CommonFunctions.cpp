#include "CommonFunctions.h"

int PRINTING_TIME;
int BINDING_TIME;
int READING_WRITING_TIME;
int NO_OF_STUDENTS;
int NO_OF_GROUPS;
clock_t start;
std::mutex printMutex;

void takeInput()
{
    int n, m, w, x, y;
    cin >> n >> m >> w >> x >> y;

    NO_OF_STUDENTS = n;
    NO_OF_GROUPS = n / m;
    PRINTING_TIME = w;
    BINDING_TIME = x;
    READING_WRITING_TIME = y;
}

int findGroup(int studentID)
{
    int group_size = NO_OF_STUDENTS / NO_OF_GROUPS;
    return (studentID - 1) / group_size + 1;
}

int findPrintStation(int studentID)
{
    return ((studentID-1) % PRINT_STATIONS) + 1;
}

void startClock()
{
    start = clock();
}

double getCurrentTime()
{
    // Get the end time
    clock_t end = clock();

    // Calculate the processor time used in seconds
    double time_used = static_cast<double>(end - start)*1000.0 / CLOCKS_PER_SEC;
    return time_used;
}