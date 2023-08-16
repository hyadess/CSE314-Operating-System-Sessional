#include <bits/stdc++.h>
#include <semaphore.h>
#include <pthread.h>
using namespace std;

sem_t mut;
sem_t t1Sem;
sem_t t2Sem;
int cont;

void *P(void *arg)
{
    int x=100;
    while (x--)
    {
        sem_wait(&t1Sem);
        cout<<'s';
        sem_wait(&mut);
        cont++;
        if(cont==2)
        {
            sem_post(&t2Sem);
            sem_post(&mut);
        }
        else{
            sem_post(&mut);
            sem_post(&t1Sem);
        }

       
    }
}

void *Q(void *arg)
{
    int y=100;
    while (y--)
    {
        sem_wait(&t2Sem);
        cout<<'t';
        sem_wait(&mut);
        cont--;
        if(cont==-2)
        {
            sem_post(&t1Sem);
            sem_post(&mut);
            
        }
        else{
            sem_post(&mut);
            sem_post(&t2Sem);
        }

       
    }
}

int main()
{
    freopen("output.txt","w",stdout);
    sem_init(&mut,0,1);
    sem_init(&t1Sem,0,1);
    sem_init(&t2Sem,0,0);
    pthread_t thread1,thread2;
    pthread_create(&thread1,NULL,P,NULL);
    pthread_create(&thread2,NULL,Q,NULL);

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    

}