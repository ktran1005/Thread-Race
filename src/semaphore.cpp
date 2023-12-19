//Implementation of Semaphore Using Locks
#include "semaphore.h"

//Constructor defaults to binary semaphore
semaphore::semaphore(){
	std::lock_guard<std::mutex> lock(myLock);
	counter = 1;
}

//General Constructor for any size
semaphore::semaphore(unsigned int x){
	std::lock_guard<std::mutex> lk(myLock);
	counter = x;
}

//Signal Other Waiting Threads
void semaphore::signal(){
	std::lock_guard<std::mutex> lk(myLock);
	counter=counter+1;
	myCond.notify_all();
}

//Wait if decrement makes negative
void semaphore::wait(){
	//unique because condition var needs
	//to lock/unlock sometimes
	std::unique_lock<std::mutex> lk(myLock);
	//No need to wait if decrement does not make
	//negative
	myCond.wait(lk,[this]{return counter>0;});
	counter = counter - 1;
	lk.unlock();
	return;
}
