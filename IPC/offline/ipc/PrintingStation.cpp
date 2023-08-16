#include "PrintingStation.h"

vector<pair<int, sem_t*> *> waiting_list[PRINT_STATIONS + 1]; /// students and their corresponding semaphores.....
sem_t stations[PRINT_STATIONS + 1];

/// semaphore for the waiting list so that the running thread and a new incoming thread do not access the
/// queue at a time............
sem_t list_sem[PRINT_STATIONS + 1];

void usingPrinter(int studentID)
{
    std::unique_lock<std::mutex> lock(printMutex);
    cout << "Student " << studentID << " started using printer at time " << getCurrentTime() << endl;
    lock.unlock();
    sleep(PRINTING_TIME);
    std::unique_lock<std::mutex> lock2(printMutex);
    cout << "Student " << studentID << " finished using printer at time " << getCurrentTime() << endl;
    lock2.unlock();
}

void signalOthers(int studentID)
{
    int stationNo = findPrintStation(studentID);
    if (waiting_list[stationNo].size() == 0)
        return;
    bool done = false;
    for (int i = 0; i < waiting_list[stationNo].size(); i++)
    {
        if (findGroup(waiting_list[stationNo][i]->first) == findGroup(studentID))
        {
            //cout<<"here"<<endl;
            sem_post(waiting_list[stationNo][i]->second);
            done = true;
            std::unique_lock<std::mutex> lock(printMutex);
            cout << "Student " << studentID << " signaled student " << waiting_list[stationNo][i]->first << " for print station " << stationNo << endl;
            lock.unlock();
            waiting_list[stationNo].erase(waiting_list[stationNo].begin() + i);

            break;
        }
    }
    if (done)
        return;
    for (int i = 0; i < waiting_list[stationNo].size(); i++)
    {
        //cout<<"here 2"<<endl;
        sem_post(waiting_list[stationNo][i]->second);
        done = true;
        std::unique_lock<std::mutex> lock(printMutex);
        cout << "Student " << studentID << " signaled student " << waiting_list[stationNo][i]->first << " for print station " << stationNo << endl;
        lock.unlock();
        waiting_list[stationNo].erase(waiting_list[stationNo].begin() + i);
        break;
    }
}

void *enteredStationFromWaitingRoom(void *arg)
{
    pair<int, sem_t*> *args = static_cast<pair<int, sem_t*> *>(arg);
    int studentID = args->first;

    // wait in the waiting room...........
    sem_wait(args->second);

    int stationNo = findPrintStation(studentID);

    /// acquire the list semaphore before acquiring the station semaphore.
    /// because lets say we don't get the station, so we need to wait in queue. but between those two checking, if the
    /// the running thread finds queue empty, then he would leave the station. but ALAS!!...in the next sec
    /// our chagol friend enters the queue.!!!!

    sem_wait(&stations[stationNo]);

    usingPrinter(studentID);

    // acquire queue semaphore again..................
    sem_wait(&list_sem[stationNo]);

    // first release station, then release queue semaphore. otherwise, one can enter the queue in between.
    sem_post(&stations[stationNo]);
    signalOthers(studentID);
    // delay to ensure next student occupied the station
    sleep(1);
    sem_post(&list_sem[stationNo]);

    //**************destroy sem_t for this thread
    //sem_destroy(args->second);

    pthread_exit(NULL);
}

void *enteredStation(void *arg)
{
    StationInfo *info = static_cast<StationInfo *>(arg);
    int studentID = info->studentID;
    // cout<<studentID<<endl;
    int stationNo = findPrintStation(studentID);

    /// acquire the list semaphore before acquiring the station semaphore.
    /// because lets say we don't get the station, so we need to wait in queue. but between those two checking, if the
    /// the running thread finds queue empty, then he would leave the station. but ALAS!!...in the next sec
    /// our chagol friend enters the queue.!!!!


    std::unique_lock<std::mutex> lock(printMutex);
    cout << "Student " << studentID << " entered the printing station at time " << getCurrentTime() << endl;
    lock.unlock();


    sem_wait(&list_sem[stationNo]);
    if (sem_trywait(&stations[stationNo]) == 0)
    {
        // got the semaphore, no need to hold the queue, release it...
        sem_post(&list_sem[stationNo]);

        usingPrinter(studentID);

        // acquire queue semaphore again..................
        sem_wait(&list_sem[stationNo]);

        // first release station, then release queue semaphore. otherwise, one can enter the queue in between.
        sem_post(&stations[stationNo]);
        signalOthers(studentID);
        // delay to ensure next student occupied the station
        sleep(1);
        sem_post(&list_sem[stationNo]);
    }
    else
    {
        /// create semaphore and thread to wait in waiting room.............
        std::unique_lock<std::mutex> lock(printMutex);
        cout << "Student " << studentID << " entered the waiting room for printing at time " << getCurrentTime() << endl;
        lock.unlock();

        sem_t cur;
        sem_init(&cur, 0, 0);
        pair<int, sem_t*> *arg = new pair<int, sem_t*>(make_pair(studentID, &cur));
        waiting_list[stationNo].push_back(arg);

        pthread_t thread;
        if (pthread_create(&thread, NULL, enteredStationFromWaitingRoom, arg) != 0)
        {
            cerr << "Error creating thread while printing for waiting room student " << studentID << endl;
        }
        sem_post(&list_sem[stationNo]); // release the queue semaphore............
        pthread_join(thread, NULL);
    }
    pthread_exit(NULL);
}

void PrintingStation()
{
    for (int i = 0; i < PRINT_STATIONS + 1; i++)
    {
        sem_init(&stations[i], 0, 1);
        sem_init(&list_sem[i], 0, 1);
    }
}

pthread_t enter(StationInfo *info)
{
    pthread_t thread;

    // cout<<*arg<<endl;
    if (pthread_create(&thread, NULL, enteredStation, info) != 0)
    {
        cerr << "Error creating thread while printing for student " << info->studentID << endl;
        delete info; // Cleanup in case of an error
        return NULL;
    }
    else
        return thread;
}