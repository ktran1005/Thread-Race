STD=--std=c++17 -pthread
GCC=g++
OBJ=obj
BIN=bin

bin/race: src/main.cpp obj/semaphore.o
	[ -d $(BIN) ] || mkdir -p $(BIN)
	${GCC} ${STD} -o bin/race src/main.cpp obj/semaphore.o

obj/semaphore.o: src/semaphore.h src/semaphore.cpp
	[ -d ${OBJ} ] || mkdir -p ${OBJ}
	${CC} ${CFLAGS} -c -o obj/semaphore.o src/semaphore.cpp

.PHONY: doc run clean

run: bin/race
	./bin/race 2
	./bin/race 4
	./bin/race 10

clean:
	rm -f obj/*.o
	rm -f bin/race
	rm -r -f bin
	rm -r -f obj