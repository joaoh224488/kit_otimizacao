out: Data.o ILS.o main.o
	g++ -o main Data.o ILS.o main.o

ILS.o:	ILS.cpp ILS.h 
	g++ -c ILS.cpp

Data.o: Data.cpp Data.h
	g++ -c Data.cpp

clean:
	rm *.o 

show:
	./main

