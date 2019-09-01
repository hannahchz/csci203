#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip> 

#include "Servers.h"

using namespace std;

Servers::Servers(unsigned int size, char *name): _capacity(size), _name(name), _n_idle_servers(0)
{
    _head = _tail = -1;
    _servers = new Server[_capacity];
    _idle = new int[_capacity];
    
    //loop through and initialise each server to _server[] array and 
    //enqueue their index int to the _idle[] FIFI queue
    int i;
    for (i = 0; i < _capacity; i++) {
         Server new_server;
         new_server.idx = i;
         new_server.count = 0;
         new_server.finish_time = new_server.total_idle_time = 0; 
         enqueue(new_server.idx);
         _servers[i] = new_server;
    }
}

//dequeue a server from the next available server from _idle[] queue
//and add the customer to the server struct from the _server[]
void Servers:: add_customer(Customer &c, double start_time, double finish_time)
{
     //calculate idle times (start_time)
     int next_avail_idx = dequeue();
     if (next_avail_idx != -1) {
        c.server_idx = next_avail_idx;
        
     //stats for each server
     //this service time accumulated
     _servers[next_avail_idx].total_service_time += (finish_time - start_time);
     //last time they served someone minus starting time for this event
     _servers[next_avail_idx].total_idle_time += start_time - _servers[next_avail_idx].finish_time;  
     _servers[next_avail_idx].finish_time = finish_time;
     _servers[next_avail_idx].count++;
     } 
}

// implementation for the dequeue of an index int from the _idle[] queue
int Servers::dequeue() //item deleted from front of queue
{
    int s_id;
    //If “Front” is greater than the index of the last cell in the array, set “Front” to 1.
    //if the queue becomes empty, set “Rear”=“Front”=0.
    if (_head == -1) {
        cout << "No Servers Available" << endl;
        return -1;
    } else {
        s_id = _idle[_head];
        _head++;  //increment front
        if (_head > _tail)
            _head = _tail = -1;
        _n_idle_servers--;
        return s_id;
    }
}

// add the server index int back to the _idle[] FIFO queue
void Servers::remove_customer(int s_idx)
{
    enqueue(s_idx);  // add server to idle queue
}

// FIFO enqueue implementation for an _idle[] int array of server indexes
void Servers::enqueue(int s_idx)
{
    if (_n_idle_servers == _capacity)
        cout << "Too many servers. \n";
    else {
        if (_head == -1) //if queue is empty, set front to 1 & put item at index 1
            _head = 0;
        _tail++; //increment rear
        _idle[_tail] = s_idx; //put item at index rear
        _n_idle_servers++; //increase size of queue
    }
}

bool Servers::is_available() {
     return _n_idle_servers !=0;
}
//return whether _idle[] queue is not empty

void Servers:: display_server_statistics (double last_service_time)
{
    int i;
    cout << "Statistics for " << _name << " servers:" << endl;
    cout << "|--------|-----------------|--------------------|" << endl;
    cout << left << setw(10) << "| Server |"
         << setw(18) << " Total Idle Time |"
         << setw(21) << " Total Service Time |" << endl;
    cout << "|--------|-----------------|--------------------|" << endl;
    for (i = 0; i < _capacity; i++) {
        // Final update
        _servers[i].total_idle_time += last_service_time - _servers[i].finish_time;

        cout << left << fixed << setprecision(5) << "|    " << setw(3) << _servers[i].idx + 1 << " |"
             << right << setw(16) << _servers[i].total_idle_time << " |"
             << setw(19) << _servers[i].total_service_time << " |" << endl;
    }
    cout << "|--------|-----------------|--------------------|" << endl;
    cout << endl;
}

