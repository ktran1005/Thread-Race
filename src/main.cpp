/**
 * 
 * 
 * Make commands:
 * 
 *  make
 * 
 * will build the binary.
 * 
 *  make run
 * 
 * will run three races with 2, 4 and 10 racers.
 * 
 *  make clean
 * 
 * will clear out compiled code.
 * 
 */


#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include "semaphore.h"
#include <queue>
#include <cstdlib>
#include <ctime>
#include <vector>

//Mutex Lock for printing
std::mutex ioLock;
// Mutex Lock for buffer
std::mutex lockBuffer;

// Random Time to Wait between 0-2 seconds
std::chrono::seconds waitTime();

/** Generates radom dice rolls, adds to the queue, 
    and sleep between 0-2 seconds.
*/
void gameMaster(int &numberOfRacer, std::queue<int> &roll, std::vector<std::thread::id> &finish , semaphore *semB);

/** Use dice rolls from GameMaster to move across the trace track. 
    A racer wins when it has moved 20 or more spaces.
*/
void racers(int &numberOfRacer, std::queue<int> &roll, std::vector<std::thread::id> &finish, semaphore *semB);


// Counter semaphore to signal racers
semaphore *items = new semaphore(0);
// Binary semaphore for putting threadId in the finish array
semaphore *semF = new semaphore(1);


/**

 The main program will take one command line input. It must be a integer and larger than 0.
 The input is the nunber of threads that will compete in the race.
 If the user provide 0 as the second paramater. Error message will be printed out.
 
 * @param argc number of command line arguments, expected value is 2
 * @param argv has value argv[1] number of threads that will compete in the race
 * @return 1,2,3 on failure and 0 on success
 */

int main(int argc, char *argv[])
{
    // Error checking. Make sure the program has 2 inputs
    if (argc != 2) {
        std::cout << "Invalid input. This program requires two arguments to run" << std::endl;
        return 1;
    }

    std::string input(argv[1]);
    int numberOfRacer;

    try {
        numberOfRacer = stoi(input);
    }catch (std::invalid_argument e) {
        std::cout << "Number of Racers is not a number." << std::endl;
        return 2;
    }

    // if number of racer is less than 0, throw an error 
    if (numberOfRacer <= 0) {
        std::cout << "Number of Racers must be greater than 0" << std::endl;
        return 3;
    }

    std::vector<std::thread::id> finishId;
    //Initiate a random number generator
    std::srand(std::time(0));
    // Initiate a queue
    std::queue<int> diceRoll;

    std::thread *my_Thread = new std::thread[numberOfRacer + 1];
    my_Thread[0] = std::thread(gameMaster, std::ref(numberOfRacer), std::ref(diceRoll), std::ref(finishId), items);

    // A thread for Game Master
    for (int i=1; i < numberOfRacer+1; i++)
        my_Thread[i] = std::thread(racers, std::ref(numberOfRacer), std::ref(diceRoll), std::ref(finishId), items);

    // Threads for racers
    for (int i=0; i < numberOfRacer+1; i++)
       my_Thread[i].join();

    // Ranking among racers at the end
    for (int i=0; i < finishId.size(); i++) {
        std::cout << i + 1 << ": " << finishId[i] << "\n";
    }

    return 0;
}

//Wait a random time to simulate hard work
std::chrono::seconds waitTime(){
	int time2Wait = std::rand()%3;
	std::chrono::seconds t
			= std::chrono::seconds(time2Wait);
	return t;
}

// while all threads have not crossed the finish line
// select a random number k between 0 and 5 (inclusive)
// Add the dice rolls to a queue
// sleep for a random length between 0-2 seconds
void gameMaster(int &numberOfRacer, std::queue<int> &roll, std::vector<std::thread::id> &finish, semaphore *semB) {
    
    while (numberOfRacer != 0) {
        int k = std::rand() % 5;                         //    Generate k value
        lockBuffer.lock();                               //    Protect the buffer
        for (int i=0; i < k; i++) {                      //    generate random number k times
            int value = std::rand() % 6 + 1;             //    generate data
            roll.push(value);                            //    add data to buffer
            semB->signal();                              //    singal the racers  
        } 
        lockBuffer.unlock();                             //    unlock the buffer
     
        std::this_thread::sleep_for(waitTime());   
    }
}

// while thread has not moved at least 20 spaces
// move forward that many spaces
// sleep for random length between 0-2 seconds
void racers(int &numberOfRacer, std::queue<int> &roll, std::vector<std::thread::id> &finish, semaphore *semB){
    int space = 0;
    std::thread::id this_id = std::this_thread::get_id();
    if (space == 0) {
        ioLock.lock();
        std::cout << "Thread " << this_id << " left the gate." << "\n";
        ioLock.unlock();
    }

    while (space < 20) {        
        semB->wait();                                   //   Wait for data          
        lockBuffer.lock();                              //   Get lock    
        int roll_value = roll.front();                  //   Get data
        roll.pop();                                     //   Pop the value   
        lockBuffer.unlock();                            //   Release lock
        space += roll_value;
        ioLock.lock();                                  // use mutex lock to prevent multiple threads try to print at the same time
        std::cout << "Thread " << this_id << " moved forward " << roll_value << " " << "spaces." << "\n";
        ioLock.unlock();                                // release the lock
        std::this_thread::sleep_for(waitTime());        // sleep between 0-2 seconds
    }

    numberOfRacer--;
    ioLock.lock();
    std::cout << "Thread " << this_id << " has crossed the finished line." << "\n";
    ioLock.unlock();

    semF->wait();                                        // turn the value of semaphore to 0 (grab)
    finish.push_back(this_id);                          //  put the thread's ID to finish array
    semF->signal();                                     // turn the value of semaphore back to 1 (release)
    
}