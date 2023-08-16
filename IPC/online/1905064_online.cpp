#include <bits/stdc++.h>
#include <semaphore.h>
#include <pthread.h>
using namespace std;

#define maxm 20


int m;

struct Info
{
    int id;
 
};


sem_t semProc[maxm];
sem_t semCons;
sem_t vect;
vector<int> v;

void *producer(void *arg)
{
    Info *proid = static_cast<Info *>(arg);
    int id = proid->id;
    int ui=6;
    while (ui--)
    {
        sem_wait(&semProc[id-1]);
        sem_wait(&vect);
        v.push_back(id*10+rand()%10);
        if(v.size()==2*m)
        {
            sem_post(&semCons);
        }
        sem_post(&vect);
        
    }
}

void* consumer(void* arg)
{
    int u=3;
    while(true)
    {
        sem_wait(&semCons);
        sem_wait(&vect);
        cout<<"butter: ";
        for(int i=0;i<v.size();i++)
            cout<<v[i]<<" ";
        while(!v.empty())
        {
            v.pop_back();
        }
        cout<<endl;
        u--;
        if(u>0)
        {
            sem_post(&vect);
            for(int i=0;i<m;i++)
            {
                sem_post(&semProc[i]);
                
            }
            for(int i=0;i<m;i++)
            {
                sem_post(&semProc[i]);
                
            }
            
        }
        else{
            sem_post(&vect);
        break;
        }
    }
}






int main()
{
    cin>>m;
    for(int i=0;i<m;i++)
    {
        sem_init(&semProc[i],0,2);
    }
    sem_init(&semCons,0,0);
    sem_init(&vect,0,1);
    pthread_t procThreads[m],consThread;
    for(int i=0;i<m;i++)
    {
        Info* info=new Info;
        info->id=i+1;
        pthread_create(&procThreads[i],NULL,producer,info);
    }
    pthread_create(&consThread,NULL,consumer,NULL);

    for(int i=0;i<m;i++)
    {
        
        pthread_join(procThreads[i],NULL);
    }
    pthread_join(consThread,NULL);

}