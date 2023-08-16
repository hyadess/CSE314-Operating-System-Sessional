#include "BindingStation.h"

sem_t binding ;



void* enteredBindStation(void* arg)
{

    StationInfo *info = static_cast<StationInfo *>(arg);
    int studentID = info->studentID;
    int groupNo=findGroup(studentID);

    std::unique_lock<std::mutex> lock(printMutex);
    cout << "Student " << studentID << " entering binding station for group "<<groupNo<<" at time " << getCurrentTime() << endl;
    lock.unlock();

    sem_wait(&binding);

    std::unique_lock<std::mutex> lock2(printMutex);
    cout << "Student " << studentID << " starting using binding station for group "<<groupNo<<" at time " << getCurrentTime() << endl;
    lock2.unlock();

    sleep(BINDING_TIME);
    sem_post(&binding);

    std::unique_lock<std::mutex> lock3(printMutex);
    cout << "Student " << studentID << " finished using binding station for group "<<groupNo<<" at time " << getCurrentTime() << endl;
    lock3.unlock();



    pthread_exit(NULL);
}




pthread_t bind(StationInfo *info)
{
    pthread_t thread;

    // cout<<*arg<<endl;
    if (pthread_create(&thread, NULL, enteredBindStation, info) != 0)
    {
        cerr << "Error creating thread while printing for student " << info->studentID << endl;
        delete info; // Cleanup in case of an error
        return NULL;
    }
    else
        return thread;
}

void bindingStation()
{
    sem_init(&binding,0,BIND_STATIONS);

}