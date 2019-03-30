#include <iostream>
#include <stdlib.h>
#include <pthread.h> 
using namespace std;

// In order: TobaccoAndPaper, PaperAndMatches, MatchesAndTobacco.
int semaphores[] = {0,0,0}; // 0 indicates not on table. 1 indicates on table
int notSmoking[] = {1}; //Semaphore
int holder[] = {3};
int smokers[] = {0, 1, 2}; 
string sem_name[] = {"Tobacco and Paper", "Paper and Matches", "Matches and Tobacco"};
string res_name[] = {"Matches","Tobacco","Paper"};

void sleep(double a){
  while(a--);
}

void smoke(){
  sleep(200000000);
}

void wait(int* semaphore){
  while(*semaphore==0); //Wait till both resources are on table
  *semaphore -= 1;
}

void wait(int* semaphore, string printer){
  while(*semaphore==0); //Wait till both resources are on table
  cout<<printer<<endl;
  *semaphore -= 1;
}

void signal(int* semaphore){
  *semaphore += 1;
}

void* agent(void* num) 
{
    while (1) {
      //Wait till done smoking
      wait(notSmoking);
      
      int random = rand() % 3;
      cout<<"Agent puts "<<sem_name[random]<<" on table"<<endl;
      signal(semaphores+random);
    } 
} 

void* smoker(void* num) 
{
  int *ID = (int*)num;
  int i = *ID;
  //i is smoker ID
    while (1) {
        cout<<"Smoker with "+res_name[i]+" wants to smoke"<<endl;
        wait(semaphores + i,"Smoker with "+res_name[i]+" is smoking");

        //critical section
        smoke(); 

        cout<<"Smoker with "<<res_name[i]<<" is done smoking"<<endl;
        signal(notSmoking);
        //Done smoking, signal the agent that he can put down next resource;
        
    } 
} 

int main() {
    pthread_t thread_id[4];

     pthread_create(&thread_id[3], NULL, agent, &holder[0]); //agent
     sleep(500000000);

     for (int i = 0; i < 3; i++){ 
        // create philosopher processes 
        pthread_create(&thread_id[i], NULL, smoker, &smokers[i]); 
     } 
        // terminate only after all threads are done terminating  
        for (int i = 0; i < 3; i++)   
         pthread_join(thread_id[i], NULL);
}