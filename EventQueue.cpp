#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip> 
#include "EventQueue.h"

using namespace std;

#define PARENT(i) ( (i-1)/2 )
#define LEFT(i) ( (i*2)+1 )
#define RIGHT(i) ((i*2)+2 ) 

void swap(Event *A, Event *B)
{
    Event temp = *A;
    *A = *B;
    *B = temp;
}

EventQueue::EventQueue(int size) : _capacity( size), _n_events(0)
{
   _q = new Event[_capacity];
}

void EventQueue::min_heapify(int i)
{
    int smallest = i;
    if (LEFT(i) <= _n_events && _q[LEFT(i)].ev_time < _q[i].ev_time)
        smallest = LEFT(i);
    
    if (RIGHT(i) <= _n_events && _q[RIGHT(i)].ev_time < _q[smallest].ev_time)
        smallest = RIGHT(i);
    
    if (smallest != i){
       swap(&_q[i], &_q[smallest]);
       min_heapify(smallest);
    }
}

void EventQueue::add_event(EventType ev_type, double ev_time, Customer &cust)
{
    if (_n_events == _capacity - 1){
         std::cout << "Event queue overflow." << std::endl;
         return;
    }
    
    if (_n_events == (_capacity / 2)) {
        Event *tmp = _q;
        _q = new Event[_capacity * 2]; //new event
        
        int i;
        for (i = 0; i < _capacity; i++) { 
             _q[i] = tmp[i]; 
        }
        delete[] tmp;
    }
    
    int i = _n_events++;
    Event new_event = { ev_type, ev_time, cust }; //new event
    _q[i] = new_event; 
    
    //Fix min-heap property
    while (i != 0 && _q[PARENT(i)].ev_time > _q[i].ev_time) {
         swap(&_q[i], &_q[PARENT(i)]); //swap address of node
         i = PARENT(i); //swap index
    }
}

Event EventQueue::extract_next_event() //extract from heap 
{
     if (_n_events <= 0)
        std::cout << "Event Queue underflow." << std::endl;
        
     if (_n_events == 1) {
         _n_events--;
         return _q[0];
     }
     
     Event next = _q[0];
     _q[0] = _q[_n_events - 1];
     _n_events--; //size of heap
     min_heapify(0);
     return next;
}

bool EventQueue::more_events() 
{ 
    return _n_events > 0; 
}

