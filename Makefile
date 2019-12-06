output: main.o scanner.o parser.o staticSem.o
	g++ main.o scanner.o parser.o staticSem.o -o statSem
main.o: main.cpp
	g++ -c main.cpp
scanner.o: scanner.cpp scanner.h
	g++ -c scanner.cpp
parser.o: parser.cpp parser.h
	g++ -c parser.cpp
staticSem.o: staticSem.cpp staticSem.h
	g++ -c staticSem.cpp
clean:
	rm *.o statSem
