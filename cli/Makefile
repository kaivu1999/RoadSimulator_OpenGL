output: main.o  road.o vehicle.o constants.h
	g++ -o output main.o road.o vehicle.o 

main.o: main.cpp road.h vehicle.h constants.h
	g++ -c main.cpp -std=c++11

road.o: road.cpp road.h vehicle.h constants.h
	g++ -c road.cpp -std=c++11

vehicle.o: vehicle.cpp vehicle.h constants.h
	g++ -c vehicle.cpp -std=c++11

clean: 
	rm *.o
