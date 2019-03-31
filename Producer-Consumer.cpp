#include <iostream>
#include <stdlib.h>
#include <pthread.h> 
using namespace std;

//Each fork is taken as a binary semaphore
int mutex[] = {1};
int full[] = {0};
int empty[] = {10};

int ids[] = { 0, 1}; 
string read_names[] = { "0", "1", "2", "3", "4" }; 

void sleep(double a){
  while(a--);
}
void wait(int* semaphore){
  while(*semaphore==0); //Wait till both resources are on table
  *semaphore -= 1;
}

void wait(int* semaphore1, int* semaphore2, string printer){
  while(*semaphore1==0 || *semaphore2==0); //Wait till both resources are on table
  cout<<printer<<endl;
  *semaphore1 -= 1;
  *semaphore2 -=1;
}

void wait(int* semaphore, string printer){
  while(*semaphore==0); //Wait till both resources are on table
  cout<<printer<<endl;
  *semaphore -= 1;
}



void signal(int* fork1){
  *fork1 += 1;
}

void* producer(void* num) 
{
  
    while (1) {
      cout<<"Producer wants to produce"<<endl;
      wait(empty,mutex,"Producer is producing");
      
      sleep(1000000);
      //leave CS
      signal(mutex);
      signal(full);
      cout<<"Producer has finished producing"<<endl;
      int waiter = rand()%100000000 + 100000000;
      sleep(waiter); //Before restarting
    } 
} 

void* consumer(void* num) 
{

    while (1) {

      cout<<"Consumer wants to consume"<<endl;
      wait(mutex,full,"Consumer is taking item from buffer");

      sleep(1000000);      

      signal(mutex); 
      signal(empty);

      //Consuming
      int waiter = rand()%100000000 + 100000000;
      sleep(waiter); //Before restarting
    } 
} 

int main() {
    pthread_t thread_id[2];

    pthread_create(&thread_id[0], NULL, producer, &ids[0]); 
    pthread_create(&thread_id[1], NULL, consumer, &ids[1]); 

        for (int i = 0; i < 2; i++)   
          pthread_join(thread_id[i], NULL);
}
