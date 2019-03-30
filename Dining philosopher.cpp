#include <iostream>
#include <stdlib.h>
#include <pthread.h> 
using namespace std;

//Each fork is taken as a binary semaphore
int fork[] = {1,1,1,1,1};
//1 indicates available, 0 indicates taken;
int phil[] = { 0, 1, 2, 3, 4 }; 

void sleep(double a){
  while(a--);
}
void wait(int* fork1, int* fork2){

  while(*fork1 == 0 || *fork2 == 0); //Wait till both forks are not taken
  *fork1 -= 1;
  *fork2 -= 1;
}

void signal(int* fork1, int* fork2){
  *fork1 += 1;
  *fork2 += 1;
}
void* philospher(void* num) 
{
  int *ID = (int*)num;
  int i = *ID;
  //i is Philosopher ID
    while (1) {
        cout<<"Philosopher "<<i<<" is Hungry"<<endl;
        wait(fork+i, fork+((i-1)%5));
        cout<<"Philosopher "<<i<<" is Eating"<<endl;
        //critical section
        sleep(200000000);        

        signal(fork + i,fork+((i-1)%5));
        cout<<"Philosopher "<<i<<" has finished Eating"<<endl;
        cout<<"Forks "<<i<<" & "<<(i-1)%5<<" are free"<<endl;
        //Done eating, wait for random time before hungry again;
        cout<<"Philosopher "<<i<<" is Thinking"<<endl;
        double waiter = rand() % 200000000 + 200000000;
        sleep(waiter); 
        
    } 
} 

int main() {
    pthread_t thread_id[5];
     for (int i = 0; i < 5; i++){ 
        // create philosopher processes 
        pthread_create(&thread_id[i], NULL, philospher, &phil[i]); 
     }
        // starting processes 
        for (int i = 0; i < 5; i++)   
        pthread_join(thread_id[i], NULL);
}