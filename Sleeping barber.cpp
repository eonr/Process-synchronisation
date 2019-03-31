#include <iostream>
#include <stdlib.h>
#include <pthread.h> 
#include <string>
#include <queue>
using namespace std;
//Semaphores
int barberReady[] = {0}; // 0 indicates barber is ready to work
int seatAccess[] = {1}; // Seats are not full initially
int customers[] = {0}; // Number of waiting customers
int freeSeats[] = {5}; //Taking 5 free seats
queue<int> q;
string printers[] = {"0","1","2","3","4","5","6","7","8","9"};
int holder[] = {10};
int smokers[] = {0,1,2,3,4,5,6,7,8,9}; 
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

void wait(int* semaphore, string printer, int a){
  while(*semaphore==0 || q.front()!=a); //Wait till both resources are on table
  cout<<printer<<endl;
  *semaphore -= 1;
}

void signal(int* semaphore){
  *semaphore += 1;
}

void* customer(void* num) 
{
  // Wait till getting access to change free seats
  
  int id = *(int*)(num);
  cout<<"Customer "+printers[id]+" has arrived"<<endl;
  wait(seatAccess);

  if(freeSeats[0]>0){
    freeSeats[0] -= 1; //sit down
    cout<<"Customer "+printers[id]+" takes a seat"<<endl;
    q.push(id);
    signal(customers); 
    signal(seatAccess); // Release access.
    wait(barberReady, "Customer "+printers[id]+" is getting a haircut", id); //Wait till barber is ready
    // Haircut
    sleep(400000000);
    cout<<"Customer "+printers[id]+" left"<<endl;
  }
  else{
    cout<<"Customer "+printers[id]+" left without a haircut"<<endl;
    signal(seatAccess);
  }
  // This thread's work is done  
  while(1);
}

void* barber(void* num) 
{
  // Sleep till there are customers
  // wait(customers);{
  while(1){
    cout<<"Barber is sleeping"<<endl;
    wait(customers, seatAccess);
    freeSeats[0] += 1;
    signal(barberReady);
    signal(seatAccess);

    sleep(400000000); //Haircut
    q.pop();
  }
}


using namespace std;
int main() {
  pthread_t thread_id[11];

  pthread_create(&thread_id[10], NULL, barber, &holder[0]); //barber
     sleep(100000000);

     for (int i = 0; i < 10; i++){ 
        // create customer processes periodically 
        pthread_create(&thread_id[i], NULL, customer, &smokers[i]); 
        sleep(100000000);
     } 

     // terminate only after all threads are done terminating  
        for (int i = 0; i < 10; i++)   
         pthread_join(thread_id[i], NULL);
}
