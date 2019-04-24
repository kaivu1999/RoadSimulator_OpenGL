#include "vehicle.h"
#include <cmath>

using namespace std;

// all these parameteres will be inherent to vehicles ... to do it in general no need to pass all these parameters
int decelerate(int v, int critical_speed , int k1, int k2, int alpha, int beta)
{
    float acc;
    int ans;
    if (v<=critical_speed) {
        acc = (float) k1 * std::pow( 2.71828f , ((float)(-1 * k2 * v)));
    }
    else
    {
        acc = alpha + (beta * v); 
    }
    ans = (int)acc;
    cout << acc << " : "<<  ans << endl;
    return ans; 
}

int vehicle::get_width()
{
    return width;
}

int vehicle::get_length()
{
    return length;
}

int vehicle::get_max_acc()
{
    return acceleration;
}

int vehicle::get_position_x()
{
    return position_x;
}

int vehicle::get_position_y()
{
    return position_y;
}

void vehicle::move(vector<vehicle*> vehicles,bool sig_stat)
{
    // now according to vehicles and sig_stat decide how to move 

    // also see how acceleration changes

    // cout << "acc" << ":" << acceleration  << endl;
    // cout << "velocity" << ":" << velocity  << endl;
    // cout << "aposition_x" << ":" <<position_x << endl;
    if ((sig_stat == true) and (position_x <= road_signal) and (position_x >= (road_signal - 5)))
    {
        acceleration = (-1) * decelerate(velocity,critical_speed,k1,k2,alpha,beta);
        cout << velocity << ":" <<acceleration << endl;
    }
    // else if  ((position_x >= road_signal) and (position_x <= (road_signal + 6))) {
        // acceleration = (1) * accelerate()
    // }
    

    else
    {
        cout << "n" << acceleration << endl;
        acceleration = default_acceleration;
    }
    
     
    velocity += acceleration;
    if (velocity<max_speed & acceleration> 0)
    {
        velocity += acceleration;
        if (velocity > max_speed)
        {
            velocity = max_speed;
        }
    }
    else if (velocity > min_speed & acceleration < 0)
    {
        velocity += acceleration;
        if (velocity < min_speed)
        {
            velocity = min_speed;
        }
    }

    // velocity sanity check
    if (velocity > max_speed) {
        velocity = max_speed;
    }
    else if(velocity<min_speed)
    {
        velocity=min_speed;
    }
    // cout << "position_x : " << position_x << " ; " << "position_y : " << position_y << " ; " << "velocity :" << velocity << endl;
    position_x = position_x + velocity;
    // cout << position_x << endl;
}

bool vehicle::is_present(int i, int j)
{
    if (( (i <= position_y) & i >= (position_y - width + 1) ) & ( (j <= position_x) & (j >= (position_x - length + 1)) ))
    {
        return true;
    }
    else
        false;
}