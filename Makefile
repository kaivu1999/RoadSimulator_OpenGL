sim: sim.cpp
	g++ -o sim sim.cpp -lglut -lGL -lGLEW -std=c++11 -pthread

clean: 
	rm *.o sim 
