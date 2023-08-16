#include "Library.h"

int readerCount = 0;
int submissionCount = 0;
sem_t readerCountSemaphore;
sem_t bookUpdateSemaphore;

void *staff(void *arg)
{
    StaffInfo *staff = static_cast<StaffInfo *>(arg);
    int staffno = staff->staffNo;
    while (1)
    {
        sleep(20);
        int tot_sub;

        // increase the reader count..............

        sem_wait(&readerCountSemaphore);
        readerCount += 1;
        if (readerCount == 1) // new reading phase just started..block the writer..
            sem_wait(&bookUpdateSemaphore);
        sem_post(&readerCountSemaphore);

        // read the data..................
        sleep(READING_WRITING_TIME);
        tot_sub = submissionCount;

        std::unique_lock<std::mutex> lock(printMutex);
        cout << "staff " << staffno << " is reading the book. total submission: " << tot_sub << " at time " << getCurrentTime() << endl;
        lock.unlock();

        // decrase the reader count...........

        sem_wait(&readerCountSemaphore);
        readerCount -= 1;
        if (readerCount == 0) // no problem for the writer..
            sem_post(&bookUpdateSemaphore);
        sem_post(&readerCountSemaphore);

        // check if the whole work is done....
        if (tot_sub == NO_OF_GROUPS)
        {
            std::unique_lock<std::mutex> lock(printMutex);
            cout << "staff " << staffno << " saw that all groups submitted. Ending his operation at time " << getCurrentTime() << endl;
            lock.unlock();
            break;
        }
    }
    pthread_exit(NULL);
}

void *createStaff(void *arg)
{
    pthread_t staffThreads[STAFF_NUMBER];
    for (int i = 0; i < STAFF_NUMBER; i++)
    {
        StaffInfo *staffInfo = new StaffInfo;
        staffInfo->staffNo = i + 1;
        pthread_create(&staffThreads[i], NULL, staff, staffInfo);
    }

    for (int i = 0; i < STAFF_NUMBER; i++)
    {
        pthread_join(staffThreads[i], NULL);
    }

    pthread_exit(NULL);
}

pthread_t LibraryInit()
{
    sem_init(&readerCountSemaphore, 0, 1);
    sem_init(&bookUpdateSemaphore, 0, 1);

    pthread_t staffCreate;
    pthread_create(&staffCreate, NULL, createStaff, NULL);
    return staffCreate;
}

void *submitProc(void *arg)
{
    GroupInfo *group = static_cast<GroupInfo *>(arg);
    sem_wait(&bookUpdateSemaphore);
    submissionCount += 1;
    sleep(READING_WRITING_TIME);
    sem_post(&bookUpdateSemaphore);

    std::unique_lock<std::mutex> lock(printMutex);
    cout << "group " << group->groupNo << " submitted at time " << getCurrentTime() << endl;
    lock.unlock();

    pthread_exit(NULL);
}


pthread_t submit(int groupNo)
{
    GroupInfo* group=new GroupInfo;
    group->groupNo=groupNo;
    pthread_t submitThread;
    pthread_create(&submitThread, NULL, submitProc, group);
    return submitThread;
}