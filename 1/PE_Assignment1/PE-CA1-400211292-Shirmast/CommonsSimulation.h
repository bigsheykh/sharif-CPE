#ifndef COMMONS_SIMULATION_H_
#define COMMONS_SIMULATION_H_

#include <cmath>
#include <string>
#include <set>
#include <deque>
#include <iostream>

#include "Commons.h"


constexpr int CACHE_LOAD = 8192; 
long double generate_random(long double lambda)
{
    return - log(1 - (((long double) rand() / (RAND_MAX)))) / lambda;
}

void create_random_array(int n, long double lambda, long double u, long double tetha, bool fixed)
{
    long double last_time = 0;
    ofstream out_array("0");
    for (int i = 0; i < n; i++)
    {
        if (i % CACHE_LOAD == 0)
        {
            out_array.close();
            out_array.open(to_string(i / CACHE_LOAD) + ".txt", std::ofstream::out);
            out_array.precision(10);
        	out_array.setf(std::ios::fixed, std::ios::floatfield);
        }
        long double wait_time = tetha;
        if (fixed)
            wait_time = tetha;
        else
            wait_time = generate_random(1 / tetha);
        out_array << last_time << " " << wait_time << " " << generate_random(u) << "\n";
        last_time += generate_random(lambda);
    }
    out_array.close();
}

int loaded_cache = -1;

enum Situation
{
    ENTERING = 0,
    WAITING = 1,
    EXITING = 2
};

struct Customer
{
    long double enter_time, wait_time, process_time;
    long double start_processing_time;
    int node_name;
    Situation situation;
    Customer()
    {
        situation = ENTERING;
    }

    const long double next_time() const
    {
        if (situation == ENTERING)
            return enter_time;
        if (situation == WAITING)
            return enter_time + wait_time;
        return start_processing_time + process_time;
    }

    const bool operator < (const Customer& other) const
    {
        if (next_time() != other.next_time())
            return next_time() < other.next_time();
        return situation > other.situation;
    }
} cached[CACHE_LOAD + 1];

Customer get_customer(int n)
{
    if (n / CACHE_LOAD != loaded_cache)
    {
        loaded_cache = n / CACHE_LOAD;
        ifstream loading_array(to_string(loaded_cache) + ".txt", std::ifstream::in);
        for (int i = 0; i < CACHE_LOAD; i++)
            loading_array >> cached[i].enter_time >> cached[i].wait_time >> cached[i].process_time;
        loading_array.close();
    }
    cached[n % CACHE_LOAD].node_name = n;
    return cached[n % CACHE_LOAD];
}


void run_simulation(int n, long double lambda, long double u, long double tetha, bool fixed)
{
    set<Customer> customers;
    deque<Customer> line;
    long double all_waiting_time = 0, last_time = 0;
    int blocked = 0, dropped = 0;
    create_random_array(n, lambda, u, tetha, fixed);
    int i = 0;
    while (i < n || !customers.empty())
    {
        if(customers.size() < 100)
            while (i < n && customers.size() < 200)
            {
                customers.insert(get_customer(i));
                i++;
            }
        Customer customer_event = *(customers.begin());
        customers.erase(*(customers.begin()));
        last_time = customer_event.next_time();
        if (customer_event.situation == ENTERING)
        {
            if (line.size() == K)
                blocked ++;
            else if (line.empty())
            {
                customer_event.situation = EXITING;
                customer_event.start_processing_time = last_time;
                customers.insert(customer_event);
                line.push_back(customer_event);
            }
            else
            {
                customer_event.situation = WAITING;
                customers.insert(customer_event);
                line.push_back(customer_event);
            }
        }
        else if (customer_event.situation == WAITING)
        {
            all_waiting_time += customer_event.wait_time;
            auto customer_iteration = line.end();
            for (auto it = line.begin(); it != line.end(); it++)
                if((*it).node_name == customer_event.node_name)
                    customer_iteration = it;
            line.erase(customer_iteration);
            dropped ++;
        }
        else
        {
            all_waiting_time += line.front().process_time;
            line.pop_front();
            if (!line.empty())
            {
                Customer& front_customer = line.front();
                customers.erase(front_customer);
                all_waiting_time += front_customer.wait_time + last_time - front_customer.next_time();
                front_customer.situation = EXITING;
                front_customer.start_processing_time = last_time;
                customers.insert(front_customer);
            }
        }
    }

    cout << blocked << " " << dropped << "\n";
    cout << (long double) blocked / (long double) n << " " 
            << (long double)  dropped / (long double) n << "\n";
    cout << all_waiting_time << " " << all_waiting_time / last_time << "\n\n";
}


#endif
