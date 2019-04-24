#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <utility>
#include <string>
#include <cstring>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <random>
#include <thread>
// #include "vehicle.h"

using namespace std;
using namespace boost;

struct gluint 
{
    int* argc;
    char** argv;
    gluint(int* a , char** b)
    {
        argc = a ;
        argv = b;
    }
};


// int our_time = 0;
// int stop_time = 0;
int road_id;
float road_length;
float road_width;
float road_signal;
vector<float> vector_max_speed;
vector<float> vector_acceleration;
vector<float> vector_length;
vector<float> vector_width;
vector<string> vector_vehicle_types;

float rl ;
float rw ;

int signal_status = 0;
int signal_prev_status = 1;
bool accident_status = false;
bool khatam = false;

default_random_engine generator;
normal_distribution<double> distribution(0.0, 1500.0);

float get(string demand, string car_type)
{
    int num;
    int i = 0;
    // determining vehicle type
    for (i = 0; i < vector_vehicle_types.size(); i++)
    {
        if (vector_vehicle_types[i].compare(car_type) == 0)
        {
            num = i;
            break;
        }
    }
    if (i >= vector_vehicle_types.size())
    {
        num = 0;
    }
    // cout << "vehicle_type : " << car_type << "num" << num << endl;
    // We have vehicle type and if not found then we will assume dfault type ( i = 0 )

    if (demand.compare("max_speed") == 0)
    {
        return vector_max_speed[num];
    }
    else if (demand.compare("acceleration") == 0)
    {
        return vector_acceleration[num];
    }
    else if (demand.compare("width") == 0)
    {
        return vector_width[num];
    }
    else if (demand.compare("length") == 0)
    {
        return vector_length[num];
    }
}
vector<float> getColour(string colour_demand)
{
    // cout << colour_demand << "this is the collllll " << " ; " << colour_demand.compare("BLUE") << endl;
    vector<float> colors;
    if (colour_demand.compare("RED") == 0)
    {
        colors.push_back(1.0);
        colors.push_back(0.0);
        colors.push_back(0.0);
        return colors;
    }
    else if (colour_demand.compare("BLUE") == 0)
    {
        colors.push_back(0.0);
        colors.push_back(0.0);
        colors.push_back(1.0);
        return colors;
    }
    else if (colour_demand.compare("WHITE") == 0)
    {
        colors.push_back(1.0);
        colors.push_back(1.0);
        colors.push_back(1.0);
        return colors;
    }
    else if (colour_demand.compare("BLACK") == 0)
    {
        colors.push_back(0.0);
        colors.push_back(0.0);
        colors.push_back(0.0);
        return colors;
    }
    else if (colour_demand.compare("PINK") == 0)
    {
        colors.push_back(1.0);
        colors.push_back(0.756);
        colors.push_back(0.796);
        return colors;
    }
    else if (colour_demand.compare("YELLOW") == 0)
    {
        colors.push_back(1.0);
        colors.push_back(1.0);
        colors.push_back(0.0);
        return colors;
    }
}

class vehicle
{
    // suited for car
  public:
    string type;
    vector<float> colours;
    int id;
    float velocity;
    float velocity_y = 5.0f;

    float positions[2][2];
    float position_x;
    float position_y;

    float max_speed;
    float rel_max_speed;
    float min_speed;
    float acceleration;
    float max_acceleration;
    float width;
    float length;
    char representation;

    float critical_speed = 3;
    float k1 = 2.5;
    float k2 = 0.1;
    float alpha = 3;
    float beta = 1;

    float get_width()
    {
        return width;
    }
    float get_length()
    {
        return length;
    }
    float get_max_acc()
    {
        return acceleration;
    }
    float get_position_x()
    {
        return position_x;
    }
    float get_position_y()
    {
        return position_y;
    }
    int decelerate(int v, int critical_speed, int k1, int k2, int alpha, int beta)
    {
        float acc;
        int ans;
        if (v <= critical_speed)
        {
            acc = (float)k1 * std::pow(2.71828f, ((float)(-1 * k2 * v)));
        }
        else
        {
            acc = alpha + (beta * v);
        }
        ans = (int)acc;
        // cout << acc << " : " << ans << endl;
        return ans;
    }
    bool accident(vector<vehicle *> vehicles)
    {
        for(int i = 0; i < vehicles.size(); i++)
        {
            // check no collision with self ;)
            if(this->id != vehicles[i]->id)
            {
                if ((vehicles[i]->position_x <= this-> position_x && vehicles[i]->position_x >= (this->position_x - this->length) ) && (vehicles[i]->position_y <= this->position_y &&  vehicles[i]->position_y >= (position_y - width) ) ) {
                    return true;
                }
                else if ((vehicles[i]->position_x <= this->position_x && vehicles[i]->position_x >= (this->position_x - this->length)) && ((vehicles[i]->position_y - vehicles[i]->width) <= this->position_y && (vehicles[i]->position_y - vehicles[i]->width) >= (position_y - width)))
                {
                    return true;
                }
                else if (this-> position_y >= road_width || (this->position_y - this->width) <= (-1 * road_width))
                {
                    // hitting the footpaths
                    return true;
                }
                else false;
            }             
        }
        return false;
    }

    int direction( vector<vehicle*> gaadis)
    {
        int temp=0;
        float my_range_x = this-> position_x + ((this->length)*0.2);
        float my_range_y = this-> position_y + ((this->width)/16);
        float range_width = (this->width * 1.125);
        float range_length = (this->length * 0.2);

        bool someone_in_front = false;
        for (int i = 0; i < gaadis.size(); i++)
        {
            if (this->id != gaadis[i]->id)
            {
                if (((gaadis[i]->position_x - gaadis[i]->length) <= my_range_x && (gaadis[i]->position_x - gaadis[i]->length) >= (my_range_x - range_length)) && !((gaadis[i]->position_y > my_range_y && (gaadis[i]->position_y - gaadis[i]->width) > my_range_y) || (gaadis[i]->position_y < (my_range_y - range_width) && (gaadis[i]->position_y - gaadis[i]->width) < (my_range_y - range_width) )))
                {
                    this->rel_max_speed = (gaadis[i]->velocity - 1);
                    temp = -1;
                    someone_in_front = true;
                }    
            }
        }
        if (someone_in_front == false) {
            this->rel_max_speed = this->max_speed;
        }
        

        float side_my_range_x = this->position_x ;
        float side_my_range_y = this->position_y + this->width;
        float side_range_width = this->width;
        float side_range_length = this->length;

        float right_side_my_range_x = this->position_x;
        float right_side_my_range_y = this->position_y - this->width;
        float right_side_range_width = this->width;
        float right_side_range_length = this->length;

        bool someone_in_left = false;

        bool someone_in_right = false;
        bool temp_left = false ;
        bool temp_right = false ;

        for (int i = 0; i < gaadis.size(); i++)
        {
            temp_left = (((gaadis[i]->position_y - gaadis[i]->width) <= side_my_range_y && (gaadis[i]->position_y - gaadis[i]->width) >= (side_my_range_y - side_range_width)) && !((gaadis[i]->position_x > side_my_range_x && (gaadis[i]->position_x - gaadis[i]->length) > side_my_range_x) || (gaadis[i]->position_x < (side_my_range_x - side_range_length) && (gaadis[i]->position_x - gaadis[i]->length) < (side_my_range_x - side_range_length) )) );
            temp_right = (((gaadis[i]->position_y ) <= right_side_my_range_y && (gaadis[i]->position_y ) >= (right_side_my_range_y - right_side_range_width)) && !((gaadis[i]->position_x > right_side_my_range_x && (gaadis[i]->position_x - gaadis[i]->length) > right_side_my_range_x) || (gaadis[i]->position_x < (right_side_my_range_x - right_side_range_length) && (gaadis[i]->position_x - gaadis[i]->length) < (right_side_my_range_x - right_side_range_length) )) );
            if (temp_left == true) {
                someone_in_left = true;
            }
            if (temp_right == true) {
                someone_in_right = true;
            }
        }
        
        if ((someone_in_front == true)  && (someone_in_right == false)) {
            temp = -1;
        }
        else if ((someone_in_front == true) && (someone_in_right == true) && (someone_in_left == false))
        {
            temp = 1;
        }
        else
        {
            temp = 0;
        }
        
        if (this->position_y + ((this->width) / 8) >= road_width)
        {
            if(temp = -1)
                temp = 0;
        }
        else if ((this->position_y - this->width) - ((this->width) / 8) <= (-1 * road_width))
        {
            if(temp = 1)
                temp = 0; // hitting the footpaths
        }
        cout << this->id << " : " << someone_in_front << " , " << someone_in_left << " , " << someone_in_right << endl; 

        return temp;
    }

    void move(vector<vehicle *> vehicles)
    {
        if (this->type.compare("bike") == 0)
        {
            if ((signal_status == 0) and (position_x <= road_signal + 150) and (position_x >= (road_signal - 200)))
            {
                acceleration = (-1) * decelerate(velocity, critical_speed, k1, k2, alpha, beta);
            }
            else if ((signal_status == 1 and signal_prev_status == 2) and (position_x <= road_signal + 100) and (position_x >= (road_signal - 200)))
            {
                acceleration = (this->max_acceleration)*2;
            }
            else if (signal_status == 1 and signal_prev_status == 0)
            {
                acceleration = this->acceleration;
            }
            else
            {
                acceleration = this->max_acceleration;
            }
        }
        else
        {
            if ((signal_status == 0) and (position_x <= road_signal) and (position_x >= (road_signal - 400)))
            {
                acceleration = (-1) * decelerate(velocity, critical_speed, k1, k2, alpha, beta);
            }
            else if ((signal_status == 1 and signal_prev_status == 2) and (position_x <= road_signal) and (position_x >= (road_signal - 400)))
            {
                acceleration = (this->max_acceleration) * 2;
            }
            else if (signal_status == 1 and signal_prev_status == 0)
            {
                acceleration = this->acceleration;
            }
            else
            {
                acceleration = this->max_acceleration;
            }
        }
            
        if (accident(vehicles)) {
            cout << "Accident !!" << endl;
            accident_status = true;
        }

        velocity += acceleration;

        // if (velocity<rel_max_speed && acceleration> 0)
        // {
        //     velocity += acceleration;
        // }
        // else if (velocity > rel_max_speed)
        // {
        //     velocity = rel_max_speed;
        // }

        // if (velocity < min_speed)
        // {
        //     velocity = min_speed;
        // }

        int super = direction(vehicles);
        
        if (super == 0) {
            if (this->type.compare("bike") == 0)
            {
                if ((this->position_x < road_signal + 100) && velocity <= 0)
                {
                    velocity = 4;
                } 
            }
            else
            {
                if ((this->position_x < road_signal - 50) && velocity <= 0)
                {
                    velocity = 4;
                }
            }
            
        }
        if ((velocity >= rel_max_speed))
        {
            velocity = rel_max_speed;
        }
        else if (velocity <= min_speed)
        {
            velocity = min_speed;
        }

        position_x = position_x + velocity;
        position_y = position_y + (velocity_y * (float)super);
        
    }

    vehicle(float i, float j, int idd, string colour_type, string vehicle_type)
    {
        type = vehicle_type;
        colours = getColour(colour_type);
        velocity = 0;
        max_speed = get("max_speed", vehicle_type);
        rel_max_speed = max_speed;
        min_speed = 0;
        acceleration = get("acceleration", vehicle_type);
        max_acceleration = acceleration;
        length = get("length", vehicle_type);
        width = get("width", vehicle_type);
        width = get("width", vehicle_type);
        position_x = i;
        position_y = j;
        representation = 'c';
        id = idd;
    };
};

vector<vehicle *> automobiles;

void add_vectors(vector<string> tokens)
{
    vector<string> params;
    boost::split(params, tokens[0], boost::is_any_of("_"));

    if (params[1].compare("Type") == 0)
    {
        vector_vehicle_types.push_back(tokens[2]);
    }
    else if (params[1].compare("Length") == 0)
    {
        vector_length.push_back((float)stoi(tokens[2]));
    }
    else if (params[1].compare("Width") == 0)
    {
        vector_width.push_back((float)stoi(tokens[2]));
    }
    else if (params[1].compare("MaxSpeed") == 0)
    {
        vector_max_speed.push_back((float)stoi(tokens[2]));
    }
    else if (params[1].compare("Acceleration") == 0)
    {
        vector_acceleration.push_back((float)stoi(tokens[2]));
    }
}

vector<string> parseLine(const string &line)
{
    vector<string> tokens;
    if (line[0] == '#' || line.empty())
        return tokens;
    boost::split(tokens, line, boost::is_any_of(" "));
    return tokens;
}

struct RGB{
    float r;
    float g;
    float b;
    RGB(float rr , float gg , float bb){
        r = rr;
        g = gg;
        b = bb;
    }
};

void RenderString(float x, float y, void *font, const unsigned char *string, RGB const &rgb)
{
    char *c;

    glColor3f(rgb.r, rgb.g, rgb.b);
    glRasterPos2f(x, y);

    glutBitmapString(font, string);
}


void processNormalKeys(unsigned char key, int x, int y)
{
    if (key == 32){
        int temp = signal_status;
        if (signal_status == 1) {
            signal_status = 2 - signal_prev_status;
        }
        else{
            signal_status = 1;
        } 
        signal_prev_status = temp;
    }

    if (key >= 97 and key <=122)
    {
        // default_random_engine generator;
        string colours[5] = {"WHITE", "PINK", "BLUE", "RED", "YELLOW"};
        int iSecret = rand() % 5;
        // normal_distribution<double> distribution(0.0, 1500.0);
       

        string ctype;
        if (key == 97)
        {
            ctype = "auto";
        }
        else if (key == 98) {
            ctype = "bike";
        }
        else if (key == 99) {
            ctype = "Car";
        }
        else if (key == 116)
        {
            ctype = "Truck";
        }
        float temp_width = get("width", ctype);
        double number = distribution(generator);
        while (number >= road_width || (number - temp_width) <= -road_width)
        {
            number = distribution(generator);
        }
        vehicle *a = new vehicle(-4950, number, automobiles.size(), colours[iSecret], ctype);
        automobiles.push_back(a);
    }
    if (key == 27)
        exit(0);
}

void spinDisplay() // ORIGINAL FUNCTION
{
    if(accident_status == false){
        for (int i = 0; i < automobiles.size(); i++)
        {
            automobiles[i]->move(automobiles);
        }
    }
    if(khatam == true){
        glutLeaveMainLoop();
    }
    glutPostRedisplay();
}

void display_road()
{
    glColor3f(0.1, 0.9, 0.1);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(-1, -1);
    glVertex2f(-1, 1);
    glVertex2f(1, 1);
    glVertex2f(1, -1);
    glEnd();
    
    glColor3f(0.5, 0.5, 0.5);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(-rl * road_length, -rw * road_width);
    glVertex2f(-rl * road_length, rw * road_width);
    glVertex2f(rl * road_length, rw * road_width);
    glVertex2f(rl * road_length, -rw * road_width);
    glEnd();


    glColor3f(0.1, 0.1, 0.1);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(-rl * road_length, rw * (road_width + 100));
    glVertex2f(-rl * road_length, rw * road_width);
    glVertex2f(rl * road_length, rw * road_width);
    glVertex2f(rl * road_length, rw * (road_width + 100));
    glEnd();

    glColor3f(0.1, 0.1, 0.1);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(-rl * road_length, -rw * (road_width + 100));
    glVertex2f(-rl * road_length, -rw * road_width);
    glVertex2f(rl * road_length, -rw * road_width);
    glVertex2f(rl * road_length, -rw * (road_width + 100));
    glEnd();


    // signal
    glColor3f(0, 0, 0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f( road_signal*rl - 0.01, (rw * road_width) + 0.25 );
    glVertex2f(road_signal*rl - 0.01 , (rw * road_width) + 0.1 );
    glVertex2f((road_signal*rl) + 0.09 , (rw * road_width) + 0.1);
    glVertex2f((road_signal*rl) + 0.09 , (rw * road_width) + 0.25 );
    glEnd();

    if(signal_status == 0){
        glColor3f(1, 0, 0);
    }
    else if ( signal_status == 1)
    {
        glColor3f(1, 1, 0);
    }
    else
        glColor3f(0.3, 1, 0.3);


    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f( road_signal*rl , (rw * road_width) + 0.23);
    glVertex2f( road_signal*rl, (rw * road_width) + 0.12);
    glVertex2f((road_signal*rl) + 0.08, (rw * road_width) + 0.12);
    glVertex2f((road_signal*rl) + 0.08, (rw * road_width) + 0.23);
    glEnd();

    // signal road

    string str = "Press Space Bar to change signal";
    RenderString(road_signal * rl, 0.7f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)str.c_str(), RGB(0.0f, 0.0f, 0.0f));

    string Info = "Indian Road simulation : Signal";
    string Info1 = "You can add vehicles by pressing 'c' for car ; 'b' for bike ; 't' for truck ; 'a' for auto";
    RenderString(-4000 * rl, -3000 * rw, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)Info.c_str(), RGB(0.0f, 0.0f, 0.0f));
    RenderString(-4000 * rl, -3300 * rw, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)Info1.c_str(), RGB(0.0f, 0.0f, 0.0f));

    if (accident_status == true)
    {
        string acc_string = "Accident !! ... Dialing 102 ...";
        RenderString(road_signal * rl, 0.8f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)acc_string.c_str(), RGB(1.0f, 0.0f, 0.0f));
    }

    glColor3f(0.2, 0.2 , 0.2 );
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(road_signal * rl,-rw * road_width );
    glVertex2f(road_signal * rl, rw * road_width);
    glVertex2f((road_signal * rl) + 0.08, rw * road_width );
    glVertex2f((road_signal * rl) + 0.08, -rw * road_width );
    glEnd();
}

void display()
{
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    display_road();

    for (int i = 0; i < automobiles.size(); i++)
    {
        string strid = automobiles[i]->type; //+ (string)((automobiles[i])->id);
        RenderString((automobiles[i]->position_x - 50) * rl,automobiles[i]->position_y*rw, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)strid.c_str(), RGB(0.0f, 0.0f, 0.0f));

        glColor3f(automobiles[i]->colours[0], automobiles[i]->colours[1], automobiles[i]->colours[2]);
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        float x1 = automobiles[i]->position_x;
        float y1 = automobiles[i]->position_y;
        float x2 = x1 - automobiles[i]->length;
        float y2 = y1 - automobiles[i]->width;
        glBegin(GL_POLYGON);

        glVertex2f( x1 * rl,  y1  * rw);
        glVertex2f( x1 * rl,  y2  * rw);
        glVertex2f( x2 * rl,  y2 * rw);
        glVertex2f( x2 * rl,  y1 * rw);
        glEnd();

    }

    glFlush();
    glutSwapBuffers();
}


void glthread(void* glu)
{
    gluint a = gluint(((gluint*) glu)->argc,((gluint*) glu)->argv );
    // cout << " this is glthread " << endl;
    glutInit(a.argc, a.argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(10000, 10000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("RajivGandhi Chawk");

    try
    {
        // call backs
        glutDisplayFunc(display);
        glutIdleFunc(spinDisplay);
        
        // input from keyboard
        glutKeyboardFunc(processNormalKeys);
        
        glutMainLoop();
    }
    catch (const char *msg)
    {
        cout << ":" << endl;
    }
    // here are the new entries
}

void controller()
{
    // cout << " this is controller " << endl;
    default_random_engine generator;
    normal_distribution<double> distribution(0.0, 1500.0);

    ifstream file;
    file.open("config.ini");
    vector<string> tokens;
    if (file.is_open())
    {
        while (file.good())
        {
            char line[255];
            file.getline(line, 255);
            // cout << line << endl;
            string linestring(line);
            tokens = parseLine(linestring);
            if (tokens.size() == 0)
            {
                /* code */
                continue;
            }
            for(int i = 0; i < tokens.size(); i++)
            {
                cout << tokens[i] << " " ; 
            }
            cout << endl;
            
            if (tokens[0].find("Road") == 0)
            {
                vector<string> params;
                boost::split(params, tokens[0], boost::is_any_of("_"));
                if (params[1].compare("Id") == 0)
                {
                    road_id = stoi(tokens[2]);
                }
                else if (params[1].compare("Length") == 0)
                {
                    road_length = (float)stoi(tokens[2]);
                    rl = (1.00) / road_length;
                }
                else if (params[1].compare("Signal") == 0)
                {
                    road_signal = (float)stoi(tokens[2]);
                }
                else if (params[1].compare("Width") == 0)
                {
                    road_width = (float)stoi(tokens[2]);
                    rw = (1.00) / road_length;
                }
            }

            else if (tokens[0].find("Default") == 0)
            {
                add_vectors(tokens);
            }
            else if (tokens[0].find("Vehicle") == 0)
            {
                add_vectors(tokens);
            }
            else if (tokens[0] == "START")
            {
                continue;
            }
            else if (tokens[0] == "Signal")
            {
                // set signal_status based on tokens[2]
                if (tokens[1] == "TOGGLE") {
                    int temp ;
                    temp = signal_status;
                    if (signal_status == 1)
                    {
                        signal_status = 2 - signal_prev_status;
                    }
                    else
                        signal_status = 1;
                    signal_prev_status = temp;
                    cout << tokens[2];
                    sleep(stoi(tokens[2]));

                    temp = signal_status;
                    if (signal_status == 1)
                    {
                        signal_status = 2 - signal_prev_status;
                    }
                    else
                        signal_status = 1;
                    signal_prev_status = temp;

                }
                
                else if (tokens[1] == "RED")
                {
                    signal_prev_status = signal_status;
                    signal_status = 0;
                }
                else if (tokens[1] == "GREEN")
                {
                    signal_prev_status = signal_status;
                    signal_status = 2;
                }
                else
                {
                    continue;
                }
            }
            else if (tokens[0] == "Pass")
            {
                sleep(stoi(tokens[1]));
            }
            else if (tokens[0] == "END")
            {
                khatam = true;
                break;
            }
            else
            {
                double number = distribution(generator);
                float temp_width = get("width",tokens[0]);
                while (number >= road_width ||( number - temp_width)<= -road_width)
                {
                    number = distribution(generator);
                }

                vehicle *a = new vehicle(-4950, number, automobiles.size(), tokens[1], tokens[0]);
                automobiles.push_back(a);
            }
        }
        file.close();
    }
    else
    {
        cout << "Unable to open the file" << endl;
    }

}

int main(int argc, char **argv)
{
    gluint a = gluint(&argc , argv);
    thread th1(glthread,(void*) &a);
    thread th2(controller);
    
    th1.join();
    th2.join();

    return 0;

}