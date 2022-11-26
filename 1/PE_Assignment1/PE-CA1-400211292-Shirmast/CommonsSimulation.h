#ifndef COMMONS_SIMULATION_H_
#define COMMONS_SIMULATION_H_

#include <cmath>
#include <string>
#include <set>
#include <deque>
#include <iostream>
#include <algorithm>

#include "Commons.h"


constexpr int CACHE_LOAD = (int) 1e6; 
long double generate_random(long double lambda)
{
    return - log(1 - (((long double) rand() / (RAND_MAX)))) / lambda;
}


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

    const bool operator == (const Customer& other) const
    {
        return node_name == other.node_name;
    }
};

Customer get_customer(long double& general_last_time, int n, long double lambda, long double u, long double theta, bool fixed)
{
    Customer a;
    a.enter_time = general_last_time;
    if (fixed)
        a.wait_time = theta;
    else
        a.wait_time = generate_random(1 / theta);
    a.process_time = generate_random(u);
    general_last_time += generate_random(lambda);
    a.node_name = n;
    return a;
}


void run_simulation(int n, long double lambda, long double u, long double theta, bool fixed)
{
    set<Customer> customers;
    deque<Customer> line;
    long double all_waiting_time = 0, last_time = 0;
    int blocked = 0, dropped = 0;
    long double general_last_time = 0;
    int i = 0;
    while (i < n || !customers.empty())
    {
        if(customers.size() < 100)
            while (i < n && customers.size() < 200)
            {
                customers.insert(get_customer(general_last_time, n, lambda, u, theta, fixed));
                i++;
                if (i % 1048576 == 0)
                    cout << lambda << " " << i / 1048576 << "\n";
            }
        Customer customer_event = *(customers.begin());
        customers.erase(customers.begin());
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
            auto customer_iteration = find(line.begin(), line.end(), customer_event);
            line.erase(customer_iteration);
            dropped ++;
        }
        else
        {
            all_waiting_time += line.front().process_time;
            line.pop_front();
            if (!line.empty())
            {
                Customer front_customer = line.front();
                customers.erase(front_customer);
                all_waiting_time += front_customer.wait_time;// + last_time - front_customer.next_time();
                front_customer.situation = EXITING;
                front_customer.start_processing_time = last_time;
                customers.insert(front_customer);
            }
        }
    }

    cout << "ended:" << lambda << " " << fixed << "\n";
    ofstream out_file("../simulation.csv", std::ofstream::app);
	out_file.precision(10);
	out_file.setf(std::ios::fixed, std:: ios::floatfield);
    out_file << lambda << "," << fixed << ",";
    out_file << (long double) blocked / (long double) n << "," 
            << (long double)  dropped / (long double) n << ",";
    out_file << all_waiting_time / last_time << "," << n << "\n";
    out_file.close();
}


#endif
