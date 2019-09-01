//  Created by hannahchz on 31/7/19.
//g++ -std=c++11 Servers.cpp ServerQueue.cpp EventQueue.cpp HannahChuaA2Q4.cpp -o HannahChuaA2Q4.exe

#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip> 

#include "Servers.h"
#include "Customers.h"
#include "ServerQueue.h"
#include "EventQueue.h"

using namespace std;
//macro definitions
#define BUFFER_SZ 50
#define MAX(a, b) ( ((a) > (b)) ? (a) : (b) )
#define PARENT(i) ( (i-1)/2 )
#define LEFT(i) ( (i*2)+1 )
#define RIGHT(i) ((i*2)+2 ) 

//struct and enum definitions

typedef Server Server;
typedef Event Event;
typedef Customer Customer;

//class definitions


//function prototypes
void print_statistics(Servers, Servers, ServerQueue, ServerQueue);
void swap (Event *A, Event *B);

//Global variable definitions
int n_total_cust;
double last_service_completed, total_service_time, p_server_q_wait_times, s_server_q_wait_times;
int p_server_n_cust, s_server_n_cust;

int main(int argc, const char* argv[]) //argc = no. of arguments, argv = pointer to array of arguments
{
    char filename[BUFFER_SZ];
    ifstream fin;
    
    if (argc == 2)
        strncpy(filename, argv[1], BUFFER_SZ);
    else {
         cout << "Input file name >> ";
         cin.getline(filename, BUFFER_SZ);
    }
    
    fin.open(filename);
    if (!fin.good()){
        cout << "[!!] Error: Failed to open file " << filename << "\nExiting...\n";
        return -1;
    }
    
    //initialise servers from read in file parameters
    unsigned int n_p_servers, n_s_servers;
    fin >> n_p_servers >> n_s_servers;
    char p_name[] = "Primary";
    char s_name[] = "Secondary";
    Servers p_servers = Servers(n_p_servers, p_name); // wrapper class for array of primary server structs
    Servers s_servers = Servers(n_s_servers, s_name); //secondary servers
    
    //Initialise queues
    ServerQueue p_server_q = ServerQueue(2000, p_name); //FIFO queue
    ServerQueue s_server_q = ServerQueue(2000, s_name); //FIFO queue, waiting to be served by a s server
    EventQueue event_q = EventQueue(200); //Priority queue as a heap with an array - main event queue
    
    //Statistic counters initialisers
    n_total_cust = 0;
    last_service_completed = total_service_time = p_server_q_wait_times = s_server_q_wait_times = 0;
    p_server_n_cust = s_server_n_cust = 0;
    
    bool cust_arrival_flag = true; //flag to be changed if no other events to read from file
    
    //Read from customer from file
    Customer first_cust;
    first_cust.wait_duration = 0;
    fin >> first_cust.arrival_time
        >> first_cust.p_service_duration
        >> first_cust.s_service_duration;
        
    //add the first customer to the event q
    event_q.add_event(eCustomerArrived, first_cust.arrival_time, first_cust);

        /* Events are either:
         *  eCustomerArrived
        - customer is served by a p server if available, and event_time is set to when service complete
        - customer is put in a p server queue if no server available, time they went in queue stored
         *  eCustPrimaryFinished 
         - customer has finished with p server
         - customer is served by a s server if available, and event_time is set to when service completed
         - customer is put in a s server FIFO queue if no server available, time they went in queue stored
         * eCustSecondaryFinished - customer has finished with a s server and being served
         * */
             
    //main simulation event loop
    while (event_q.more_events()){
    // check if there are any more events waiting to be processed
    // the top priority event based on (min) event_time when added
        Event ev = event_q.extract_next_event();
        
        switch (ev.type) {
            case eCustomerArrived:
                 if (cust_arrival_flag) {
                    Customer next_read_cust;
                    fin >> next_read_cust.arrival_time
                        >> next_read_cust.p_service_duration
                        >> next_read_cust.s_service_duration;
                    
                    //break loop if 0, 0, 0 reached in txt file and close file
                    if (next_read_cust.arrival_time == 0 && next_read_cust.p_service_duration == 0) {
                        cust_arrival_flag = false;
                        fin.close();
                    } else {
                        next_read_cust.wait_duration = 0;
                        event_q.add_event(eCustomerArrived, next_read_cust.arrival_time, next_read_cust);
                    }
                 }
                 
                 if (p_servers.is_available()) { //p server in array to be dequeued
                      double p_service_finish_time = ev.cust.arrival_time + ev.cust.p_service_duration;
                      p_servers.add_customer(ev.cust, ev.ev_time, p_service_finish_time);
                      event_q.add_event(eCustPrimaryFinished, p_service_finish_time, ev.cust);
                 } else {
                      ev.cust.cust_queued_time = ev.ev_time;
                      p_server_q.add_waiting_customer(ev.cust, ev.ev_time);
                      p_server_n_cust++;
                 }
                 break;
                 
            case eCustPrimaryFinished:
                 //free up a server in primary server array
                 p_servers.remove_customer(ev.cust.server_idx);
            
                 total_service_time += (ev.cust.p_service_duration + ev.cust.wait_duration);
                 //wait duration only for p
                 if (s_servers.is_available()) {
                      double s_service_finish_time = ev.ev_time + ev.cust.s_service_duration;
                      s_servers.add_customer(ev.cust, ev.ev_time, s_service_finish_time);
                      event_q.add_event(eCustSecondaryFinished, s_service_finish_time, ev.cust);
                 } else {
                       ev.cust.cust_queued_time = ev.ev_time;
                       s_server_q.add_waiting_customer(ev.cust, ev.ev_time);
                       s_server_n_cust++;
                 }
                 break;
                 
            case eCustSecondaryFinished:
                 s_servers.remove_customer(ev.cust.server_idx);
                 n_total_cust++;
                 total_service_time += (ev.cust.s_service_duration + ev.cust.wait_duration);
                 
                 if (!event_q.more_events()) {
                       last_service_completed = ev.ev_time;
                       p_server_q.calculate_average_queue_len(last_service_completed);
                       s_server_q.calculate_average_queue_len(last_service_completed);
                 }
                 break;
        }
        //check if there are any secondary servers available to process someone in queue if there is a queue
        if (!s_server_q.is_empty() && s_servers.is_available()) {
              Customer waiting_cust = s_server_q.next_waiting_customer(ev.ev_time);
              waiting_cust.wait_duration = ev.ev_time - waiting_cust.cust_queued_time;
              double s_server_finish_time = ev.ev_time + waiting_cust.s_service_duration;
              
              s_servers.add_customer(waiting_cust, ev.ev_time, s_server_finish_time);
              event_q.add_event(eCustSecondaryFinished, s_server_finish_time, waiting_cust);
              
              s_server_q_wait_times += waiting_cust.wait_duration;
        }
        
        if (!p_server_q.is_empty() && p_servers.is_available()) {
              Customer waiting_cust = p_server_q.next_waiting_customer(ev.ev_time);
              
              waiting_cust.wait_duration = ev.ev_time - waiting_cust.cust_queued_time;
              double p_server_finish_time = ev.ev_time + waiting_cust.p_service_duration;
              
              p_servers.add_customer(waiting_cust, ev.ev_time, p_server_finish_time);
              event_q.add_event(eCustPrimaryFinished, p_server_finish_time, waiting_cust);
              
              p_server_q_wait_times += waiting_cust.wait_duration;
              
        }
     }
     print_statistics(p_servers, s_servers, p_server_q, s_server_q);
     cout << "|=======|  Assignment 02 -- Simulation Complete  |=======|" << endl << endl;
     return 0;
}     

void print_statistics (Servers p_servers, Servers s_servers, ServerQueue p_server_q, ServerQueue s_server_q)
{
     cout << "\n\n|=======|  Assignment 02 -- Simulation Statistics  |=======|" << endl << endl;
     cout << fixed << setprecision(3) << left << setfill('.') << setw(50) << "Number of People Served:" << " "
          << n_total_cust << endl;
     cout << left << setw(50) << "Time Last Service Request Completed:" << " "
          << last_service_completed << endl;
     cout << left << setw(50) << "Total Service Time:" << " "
          << total_service_time << endl;
     cout << left << setw(50) << "Average Total Service Time: " << " "
          << total_service_time / n_total_cust << endl << endl;
     cout << left << setw(50) << "Total Time in Queues:" << " "
          << ((p_server_q_wait_times + s_server_q_wait_times) / (p_server_n_cust + s_server_n_cust))
            * (p_server_n_cust + s_server_n_cust) << endl;
     cout << left << setw(50) << "Average Time Spent in Primary Server Queue:" << " "
          << p_server_q_wait_times / p_server_n_cust << endl;
     cout << left << setw(50) << "Average Time Spent in Secondary Server Queue:" << " "
          << s_server_q_wait_times / s_server_n_cust << endl;
     cout << left << setw(50) << "Average Total Time in Queues:" << " "
          << (p_server_q_wait_times + s_server_q_wait_times) / (p_server_n_cust + s_server_n_cust)
          << endl << endl;

    // TODO
    cout << left << setw(50) << "Average Length of the Primary Server Queue:" << " "
         << p_server_q.get_avg_queue_len() << endl;
    cout << left << setw(50) << "Average Length of the Secondary Server Queue:" << " "
         << s_server_q.get_avg_queue_len() << endl;
    cout << left << setw(50) << "Average Length of the Queues:" << " "
         << (p_server_q.get_sum_q_len() + s_server_q.get_sum_q_len())  /
            (p_server_q.get_sum_q_len_time() + s_server_q.get_sum_q_len_time())
         << endl << endl;

    cout << left << setw(50) << "Max Length of Primary Server Queue:" << " "
         << p_server_q.get_max_q_len() << endl;
    cout << left << setw(50) << "Max Length of Secondary Server Queue:" << " "
         << s_server_q.get_max_q_len() << endl;
    cout << left << setw(50) << "Max Length of Queues:" << " "
         << MAX(p_server_q.get_max_q_len(), s_server_q.get_max_q_len())
         << endl << endl << setfill(' ');

    p_servers.display_server_statistics(last_service_completed);
    s_servers.display_server_statistics(last_service_completed);
}




