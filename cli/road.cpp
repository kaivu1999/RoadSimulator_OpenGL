#include "road.h"
#include "constants.h"
#include <typeinfo>
using namespace std;

void road::add_vehicle(vehicle* gadi){
    for(int i = 0; i < gadi->get_width(); i++)
    {
        for(int  j = 0; j < gadi->get_length(); j++)
        {
            road_grid[gadi->get_position_y() - i][gadi->get_position_x() - j] = gadi;
        }
        
    }
}
void road::signal_red()
{
    signal_status = true;
}

void road::signal_green()
{
    signal_status = false;
}

bool road::road_update(vector<vehicle *> vehicles)
{
    bool status = true; 
    // status = false in case of emergency !! the simulation will stop.

    int presence=0;
    for(int i = 0; i < road_width; i++)
    {
        for(int j = 0; j < road_length; j++)    
        {
            presence = 0;
            for (int k = 0; k < vehicles.size(); k++)
            {
                if ((*(vehicles[k])).is_present(i, j))
                {
                    presence += 1;
                    road_grid[i][j] = vehicles[k];
                }
            }
            if (presence == 0) {
                road_grid[i][j] = nullptr;
            }
            else if (presence > 1) {
                status = false;
                cout << "**** Accident Call an ambulance ***** " << endl;
                cout << "Dialing : 102 ..." << endl;
                break;
            }
        }
    }
    return status;
}