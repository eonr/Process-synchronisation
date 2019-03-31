#include <iostream>
#include <stdlib.h>
#include <pthread.h> 
using namespace std;

//Each fork is taken as a binary semaphore
int mutex[] = {1};
int wrt[] = {1};
int rct[] = {0};
//1 indicates available, 0 indicates taken;
int holder[] = {5};
int readers[] = { 0, 1, 2, 3, 4 }; 
string read_names[] = { "0", "1", "2", "3", "4" }; 

void sleep(double a){
  while(a--);
}
void wait(int* semaphore){
  while(*semaphore==0); //Wait till both resources are on table
  *semaphore -= 1;
}

void wait(int* semaphore1, int* semaphore2){
  while(*semaphore1==0 || *semaphore2==0); //Wait till both resources are on table
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

void* writer(void* num) 
{
  int *ID = (int*)num;
  int i = *ID;
  //i is writer ID
    while (1) {
      cout<<"Writer wants to write"<<endl;
      wait(wrt, "Writer is writing");
      
      sleep(100000000);
      //leave CS
      signal(wrt);
      cout<<"Writer has finished writing"<<endl;
      sleep(100000000);
    } 
} 

void* reader(void* num) 
{
  int *ID = (int*)num;
  int i = *ID;
  //i is reader ID
    while (1) {
      cout<<"Reader "+read_names[i]+" wants to read"<<endl;
      wait(mutex);
      rct[0]++;

      //Giving priority to readers
      if (rct[0]==1)
        wait(wrt);

      signal(mutex); //Let other readers enter
      //Read for some time
      cout<<"Reader "+read_names[i]+" is reading"<<endl;
      sleep(100000000);
      cout<<"Reader "+read_names[i]+" has finished reading"<<endl;
      wait(mutex); // Reader wants to leave
      rct[0]--;

      if(rct[0]==0)
        signal(wrt); //Signal that writer can use.

      signal(mutex); //Leave

      sleep(400000000);
    } 
} 

int main() {
    pthread_t thread_id[6];

    pthread_create(&thread_id[5], NULL, writer, &holder[0]); 

     for (int i = 0; i < 5; i++){ 
        // create philosopher processes 
        pthread_create(&thread_id[i], NULL, reader, &readers[i]); 
     }

        for (int i = 0; i < 6; i++)   
          pthread_join(thread_id[i], NULL);
}
