#include <bits/stdc++.h>
#include <semaphore.h>
#include <pthread.h>
using namespace std;

sem_t rebound[3];
sem_t threads[3];


void *P(void *arg)
{
    while (true)
    {
        sem_wait(&threads[0]);
        
        cout << "p ";
        sem_post(&rebound[0]);
       
    }
}
void *Q(void *arg)
{
    while (true)
    {
        sem_wait(&threads[1]);
        
        cout << "q ";
        sem_post(&rebound[1]);
        
    }
}
void *R(void *arg)
{
    while (true)
    {
        sem_wait(&threads[2]);
        
        cout << "r ";
        sem_post(&rebound[2]);
        
    }
}

void *restart(void *arg)
{
    while (true)
    {
        sem_wait(&rebound[0]);
        sem_wait(&rebound[1]);
        sem_wait(&rebound[2]);
        cout<<endl;
        
        sem_post(&threads[1]);
        sem_post(&threads[0]);
        sem_post(&threads[2]);
    }
}

int main()
{
    sem_init(&rebound[0], 0, 0);
    sem_init(&rebound[1], 0, 0);
    sem_init(&rebound[2], 0, 0);
   
   
    sem_init(&threads[1], 0, 1);
    sem_init(&threads[0], 0, 1);
    sem_init(&threads[2], 0, 1);

  

    pthread_t pp, qq, rr, re;
    pthread_create(&pp, NULL, P, NULL);
    pthread_create(&qq, NULL, Q, NULL);
    pthread_create(&rr, NULL, R, NULL);
    pthread_create(&re, NULL, restart, NULL);
    pthread_join(pp,NULL);
    pthread_join(qq,NULL);
    pthread_join(rr,NULL);
    pthread_join(re,NULL);

}