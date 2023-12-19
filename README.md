# Thread-Race

## Overview
This project will simulate **Thread Race**. It is all about communicate between threads and locks. We simulate a race between a collection of threads. 

## Thread Race

The program will take one command line input. It must be a integer and strictly larger than 0. The input is the number of threads that will compete in the race. Having 0 threads race would be pointless. The program should produce an error if no value is given or an invalid input is given. 

For example, to race three threads we we run `./bin/race 3`

For the remainder of this section, let us assume $r$ is the number of racers. One thread will be the **game master** who runs the race. The other $r$ threads will be the racers.

The **Game Master** thread generates random dice rolls. The **racers** will then use these dice rolls to move across the race track. A **racer** wins when it has move 20 or more spaces. Once the **racer** is at the end of the race track it has crossed the finish line. Once all **racers** have crossed the finish line, the program is over. The **main** program should print a list of the threads ranked by when they crossed the finish line.

All threads will sleep during the race to simulate the program doing a complex calculation.

The basic logic of the **Game Master** thread is as follows:

```text
While all threads have not crossed the finish line
	Select a random number k between 0 and 5 (inclusive)
	Roll k 6-sided dice (random number 1-6 inclusive)
	Add the dice rolls to a queue
	Sleep for a random length between 0-2 seconds 
```

The basic logic of the **racer** thread is as follows:

```text
While thread has not moved at least 20 spaces
	Read a dice roll from the queue
	Move forward that many spaces
	Sleep for a random length between 0-2 seconds 
```

This program makes sure the following features are thread safe: 

- I/O two threads should never write to cout at the same time
- Dice Queue must be safe for the **game master** and **racer**

## Makefile
The program has the following make targets:

- `make`  - Builds the Program
- `make run` - Runs three races with 2, 4, and 10 racers.
- `make clean` - Remove compiled code
- `make doc` - generate doxygen documentation
