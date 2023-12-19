
//A semaphore class using locks and condition variables

#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <mutex>
#include <condition_variable>


class semaphore{
	private:
		//The counter for increment and decrement
		unsigned int counter;
		//Mutex to lock access to counter
		//this should be mutable you will read why
		mutable std::mutex myLock;
		//Condition Variable for waits
		std::condition_variable myCond;
	public:
		//Default to 1 if no value given
		semaphore();
		//User gives the count
		semaphore(unsigned int size);
		//signal other threads
		void signal();
		//Wait for signal
		void wait();
};

#endif
