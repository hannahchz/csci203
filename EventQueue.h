#include "Customers.h"

#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip> 

using namespace std;

#ifndef EventQueue_h
#define EventQueue_h

enum EventType { eCustomerArrived = 0, eCustPrimaryFinished = 1, eCustSecondaryFinished = 2};

struct Event {
    EventType type;
    double ev_time;
    Customer cust;
};

class EventQueue {
   public:
      explicit EventQueue (int size);
      void add_event(EventType ev_type, double ev_time, Customer& cust);
      Event extract_next_event();
      bool more_events();
      
   private:
      void min_heapify(int i);
      int _n_events, _capacity;
      Event *_q;
};
#endif 
