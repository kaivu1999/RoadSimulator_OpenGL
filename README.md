# RoadSimulator

## Libraries used
- GL/freeglut.h 
- GL/gl.h
for openGL implementation

The sim.cpp file includes the code which simulates the scene according to CONFIG file on runtime.
Two threaded implementation allows Dyanamic input from keyboard as well, apart from the config file.

## Features : 
We have added the following features
- In the Default Config file: We have types  Car, auto, bike, Bus, Truck 
- Bikes go in front !! and somewhat ahead of all on the signal.  
- We have added Overtaking feature  ( from left or right ). 
- Used Multi threading for Dynamicity from config File ( 1 OpenGL thread 
and Dynamic Config Parser thread 
- We have also handled accident . In case of an accident, all vehicles stop to 
move and Accident alert text is shown. 
- Keyboard Input : We have also added feature to  change the current signal 
status and add Cars from Keyboard . GUI :)

![Sample image](https://user-images.githubusercontent.com/35027192/56661148-c2d3f180-66be-11e9-9f2e-6a4bff8ed70f.png)
