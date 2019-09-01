#include <stdio.h>
using namespace std;

#ifndef Customers_h
#define Customers_h

struct Customer {
   double arrival_time, p_service_duration, s_service_duration; //INT NOT DOUBLE
   double wait_duration, cust_queued_time;
   int server_idx;
};

#endif 
