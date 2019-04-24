#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include "constants.h"
// #include "road.h"

using namespace std;

class vehicle
{
    // suited for car
    public:
        int id;
        int velocity;
        int position_x;
        int position_y;
        int max_speed;
        int min_speed;
        int acceleration;
        int width;
        int length;
        char representation;
        int default_acceleration;
        int critical_speed = 3;
        int  k1 = 100 ;
        int  k2 = 3 ;
        int  alpha = 3 ;
        int  beta = 1 ;

        int get_width();
        int get_length();
        int get_max_acc();
        int get_position_x();
        int get_position_y();
        void move(vector<vehicle*> vehicles , bool sig_stat); // need to pass all vehicles here to decide about what to do !!    
        bool is_present(int i , int j);

        vehicle()
        {
            velocity = 0;
            max_speed = 10;
            min_speed = 0;
            acceleration = 1;
            width = 2;
            length = 2;
            representation = 'c';
            default_acceleration = 1;
        };

        vehicle(int i, int j,int Id)
        {
            velocity = 0;
            max_speed = 5;
            min_speed = 0;
            acceleration = 1;
            width = 2;
            length = 2;
            position_x = j;
            position_y = i;
            representation = 'c';
            id = Id;
            default_acceleration = 1;
        };

};

class bus: public vehicle
{
  public:
    bus()
    {
        velocity = 0;
        max_speed = 5;
        min_speed = 0;
        acceleration = 1;
        width = 2;
        length = 3;
        representation = 'B';
        default_acceleration = 1;
    };

    bus(int i, int j,int Id)
    {
        velocity = 0;
        max_speed = 5;
        min_speed = 0;
        acceleration = 1;
        width = 2;
        length = 3;
        position_x = j;
        position_y = i;
        representation = 'B';
        id = Id;
        default_acceleration = 1;
    };
};

class truck: public vehicle
{
  public:
    truck()
    {
        velocity = 0;
        max_speed = 5;
        min_speed = 0;
        acceleration = 1;
        width = 2;
        length = 3;
        representation = 'T';
        default_acceleration = 1;
    };

    truck(int i, int j, int Id)
    {
        velocity = 0;
        max_speed = 5;
        min_speed = 0;
        acceleration = 1;
        width = 2;
        length = 3;
        position_x = j;
        position_y = i;
        representation = 'T';
        id = Id;
        default_acceleration = 1;
    };
};

class bike: public vehicle
{
  public:
    bike()
    {
        velocity = 0;
        max_speed = 5;
        min_speed = 0;
        acceleration = 1;
        width = 1;
        length = 2;
        representation = 'b';
        default_acceleration = 1;
    };
    bike(int i, int j, int Id)
    {
        velocity = 0;
        max_speed = 5;
        min_speed = 0;
        acceleration = 1;
        width = 1;
        length = 2;
        position_x = j;
        position_y = i;
        representation = 'b';
        id = Id;
        default_acceleration = 1;
    };
};

class car: public vehicle
{
  public:
    car()
    {
        max_speed = 20;
        min_speed = 0;
        velocity = 0;
        acceleration = 1;
        width = 2;
        length = 2;
        representation = 'c';
        default_acceleration = 1;
    }
    car(int i, int j, int Id)
    {
        max_speed = 2;
        min_speed = 0;
        velocity = 0;
        acceleration = 1;
        width = 2;
        length = 2;
        position_x = j;
        position_y = i;
        representation = 'c';
        id = Id;
        default_acceleration = 1;
    }
};