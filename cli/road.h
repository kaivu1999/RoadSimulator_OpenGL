#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include "constants.h"
#include "vehicle.h"
#include <typeinfo>
using namespace std;

class road
{
  private:
    int length = road_length;
    int width = road_width;
    int signal = road_signal;
    // true for red and     false for green
  public:
    bool signal_status = false;
    vehicle *road_grid[road_width][road_length];
    road()
    {
        for (int i = 0; i < road_width; i++)
        {
            for (int j = 0; j < road_length; j++)
            {
                road_grid[i][j] = nullptr;
            }
        }
    }
    void add_vehicle(vehicle *gaadi);
    void signal_red();
    void signal_green();
    bool road_update(vector<vehicle *> vehicles);
};
