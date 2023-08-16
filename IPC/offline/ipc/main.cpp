#include "CommonFunctions.h"
#include "PrintingStation.h"
#include "BindingStation.h"
#include "Library.h"



pthread_t enteringStation(int studentID, int delayTime)
{
    sleep(delayTime);
    StationInfo* info=new StationInfo;
    info->studentID=studentID;
    return enter(info);
}

void *groupPrintingPhase(void *arg)
{

    Group *group = static_cast<Group *>(arg);
    int startStudent = group->startStudent;
    int lastStudent = group->lastStudent;
    int groupNo = group->groupNo;

    int lambda = 3; // Poisson distribution parameter (mean)
    // Create a random number engine and a Poisson distribution
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::poisson_distribution<int> poissonDist(lambda);

    
    pthread_t printingThreads[lastStudent - startStudent + 1];

    for (int i = startStudent; i <= lastStudent; i++)
    {
        int randomValue = poissonDist(gen);
        printingThreads[i - startStudent] = enteringStation(i, randomValue);
    }
    for (int i = startStudent; i <= lastStudent; i++)
    {
        pthread_join(printingThreads[i - startStudent], NULL);
    }
    std::unique_lock<std::mutex> lock(printMutex);
    cout << "Group " << groupNo << " finished printing at time " << getCurrentTime() << endl;
    lock.unlock();
    pthread_exit(NULL);
}


//workflow of a group................
void* groupWorkFlow(void* arg)
{
    /// fetch group info....................
    Group *group = static_cast<Group *>(arg);
    int startStudent = group->startStudent;
    int lastStudent = group->lastStudent;
    int groupNo = group->groupNo;
    int groupLeader = lastStudent;

    /// printing phase........................
    pthread_t printThread;
    pthread_create(&printThread, NULL, groupPrintingPhase, group);
    pthread_join(printThread,NULL);

    std::unique_lock<std::mutex> lock(printMutex);
    cout<<"Group "<<group->groupNo<<" starting binding phase"<<endl;
    lock.unlock();


    /// binding phase...........................
    StationInfo* info =new StationInfo;
    info->studentID=lastStudent;
    pthread_t bindThread=bind(info);
    pthread_join(bindThread,NULL);


    ///submission phase..................

    pthread_t submitThread=submit(groupNo);
    pthread_join(submitThread,NULL);



    std::unique_lock<std::mutex> loc(printMutex);
    cout<<"Group "<<group->groupNo<<" finished all phase"<<endl;
    loc.unlock();


    pthread_exit(NULL);

    
}

//creates individual group threads.............
pthread_t groupStart(int startStudent, int lastStudent,int delayTime)
{
    // create the group.................
    Group *group = new Group;
    group->startStudent = startStudent;
    group->lastStudent = lastStudent;
    group->groupNo = findGroup(startStudent);

    //start group work after a random time...........
    sleep(delayTime);
    std::unique_lock<std::mutex> lock(printMutex);
    cout<<"Group "<<group->groupNo<<" starting their work at time "<<getCurrentTime()<<endl;
    lock.unlock();
    pthread_t groupThread;
    pthread_create(&groupThread, NULL, groupWorkFlow, group);

    return groupThread;
        
    
}


/// initializes the groups.......................
void startSimulation()
{
    int group_size=NO_OF_STUDENTS/NO_OF_GROUPS;
    int cur_start=1,cur_end=group_size;

    // random group start............

    int lambda = 4; // Poisson distribution parameter (mean)
    // Create a random number engine and a Poisson distribution
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::poisson_distribution<int> poissonDist(lambda);
    vector<pthread_t>groupThreads;
    while(1)
    {
        int randomValue = poissonDist(gen);
        pthread_t groupThread=groupStart(cur_start,cur_end,randomValue);
        groupThreads.push_back(groupThread);
        cur_start=cur_end+1;
        cur_end=cur_start+group_size-1;
        if(cur_end>NO_OF_STUDENTS)
            break;

    }

    for(int i=0;i<groupThreads.size();i++)
    {
        pthread_join(groupThreads[i],NULL);
    }


}

int main()
{

    takeInput();
    pthread_t Library=LibraryInit();

    PrintingStation();
    bindingStation();
    startSimulation();

    pthread_join(Library,NULL);
}