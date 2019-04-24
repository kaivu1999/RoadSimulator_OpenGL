#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include "road.h"
#include "constants.h"

using namespace std;

void move(vector<vehicle*> vehicles,bool signal_status){
    for(int i = 0; i < vehicles.size(); i++)
    {
        (vehicles[i])->move(vehicles,signal_status);
    }
    
}

void print_road(vehicle* road_g[road_width][road_length],bool signal_s){
    for(int i = 0; i < road_width; i++)
    {
        for(int j = 0; j < road_length; j++)
        {
            if (road_g[i][j]==nullptr) {
                cout << " ";
            }
            else
            {
                cout << (road_g[i][j])->representation;
            }
            if (j==(road_signal-1)) {
                if (signal_s==true) {
                    cout << "|";
                }
                
            }
        }
        cout << ":" << endl;
    }
    
}

int main(){
    // const char *FileName = "./config.ini";
    // CSimpleIniA ini;
    // ini.SetUnicode();
    // ini.LoadFile(FileName);

    // const char *section1 = "Parameters";
    // const char *section2 = "Instructions";
    // const char *entry = "Road_Length";
    // const char *DefaultStr = "Falt_In_Config";
    // const std::multimap<CSimpleIniTempl<char, SI_GenericNoCase<char>, SI_ConvertA<char> >::Entry, const char *, CSimpleIniTempl<char, SI_GenericNoCase<char>, SI_ConvertA<char> >::Entry::KeyOrder, std::allocator<std::pair<const CSimpleIniTempl<char, SI_GenericNoCase<char>, SI_ConvertA<char> >::Entry, const char *> > > * sect = ini.GetSection(section1);
    // const std::multimap<CSimpleIniA::Entry, const char *, CSimpleIniA::Entry::KeyOrder> *sect1 = ini.GetSection(section1);
    // cout << sect  << endl;

    road road1;
    vector<vehicle *> vehicles;

    truck truck1(4,1,vehicles.size());
    vehicles.push_back((vehicle*)&truck1);
    
    road1.add_vehicle((vehicle*)&truck1);
    print_road(road1.road_grid, road1.signal_status);

    bike bike2(1, 1, vehicles.size());
    vehicles.push_back((vehicle *)&bike2);
    road1.add_vehicle((vehicle*)&bike2);

    print_road(road1.road_grid, road1.signal_status);

    int iteration_time = 10;

    road1.signal_red();
    for(int i = 0; i < iteration_time; i++)
    {
        cout << ".........................................................s" << endl;
        cout << "Time : " <<  i << endl;
        cout << ".........................................................|" << endl;
        print_road(road1.road_grid, road1.signal_status);
        move(vehicles,road1.signal_status);
        bool status = (road1.road_update(vehicles));
        if(status == false){
            cout << "STOP Simulation" << endl;
            break;
        }
        cout << ".........................................................e" << endl;
    }
    road1.signal_green();
    for (int i = 0; i < iteration_time; i++)
    {
        cout << ".........................................................s" << endl;
        cout << "Time : " << i + iteration_time << endl;
        cout << ".........................................................|" << endl;
        print_road(road1.road_grid, road1.signal_status);
        move(vehicles, road1.signal_status);
        bool status = (road1.road_update(vehicles));
        if (status == false)
        {
            cout << "STOP Simulation" << endl;
            break;
        }
        cout << ".........................................................e" << endl;
    }
    return 0;
}