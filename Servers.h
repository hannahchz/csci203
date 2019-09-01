#include "Customers.h"

using namespace std;

#ifndef Servers_h
#define Servers_h
struct Server {
     int idx;
     int count;
     double finish_time, total_idle_time, total_service_time;
};

class Servers {
     public:
       explicit Servers (unsigned int size, char *name);
       void add_customer (Customer &c, double start_time, double finish_time);
       void remove_customer(int s_idx);
       void enqueue(int s_idx);
       int dequeue();
       bool is_available();
       void display_server_statistics(double last_service_time);
       
    private:
       Server *_servers;
       int *_idle;
       char *_name;
       int _head, _tail;
       unsigned int _capacity, _n_idle_servers;
};

#endif 
