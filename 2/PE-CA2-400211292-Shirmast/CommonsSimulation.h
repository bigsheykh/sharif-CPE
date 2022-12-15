#ifndef COMMONS_SIMULATION_H_
#define COMMONS_SIMULATION_H_

#include <cmath>
#include <string>
#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>

#include "Commons.h"

__float128 generate_random(__float128 lambda)
{
    return - log(1 - (((long double) rand() / (RAND_MAX)))) / lambda;
}

enum Situation
{
    ENTERING = 0,
    ENTERED = 1,
    GETTING_DONE = 2,
    GETTING_OUT = 3
};

struct Customer
{
    __float128 enter_time, wait_time, process_time;
    __float128 now_time, remaining_time;
    __float128 work_rate;
    int node_name;
    Situation situation;

    Customer()
    {
        situation = ENTERING;
    }

    void change_to_in_line()
    {
        now_time = enter_time;
        remaining_time = process_time;
        situation = ENTERED;
    }

    void update_in_line(__float128 new_time, __float128 new_work_rate)
    {
        if(situation != ENTERED)
            remaining_time -= (new_time - now_time) * work_rate;
        now_time = new_time;
        work_rate = new_work_rate;
        if (now_time + remaining_time / work_rate > enter_time + wait_time)
            situation = GETTING_OUT;
        else
            situation = GETTING_DONE;
    }

    const __float128 next_time() const
    {
        if (situation == ENTERING || situation == ENTERED)
            return enter_time;
        if (situation == GETTING_OUT)
            return enter_time + wait_time;
        return now_time + remaining_time / work_rate;
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

Customer get_customer(__float128& general_last_time, int n, __float128 lambda, __float128 u, __float128 theta, bool fixed)
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


void run_simulation(int n, __float128 lambda, __float128 u, __float128 theta, bool fixed)
{
    queue<Customer> customers;
    vector<Customer> line;
    __float128 all_waiting_time = 0, last_time = 0;
    int blocked = 0, dropped = 0;
    __float128 general_last_time = 0;
    int i = 0;

    while (i < n || !customers.empty() || !line.empty())
    {
        while (i < n && customers.size() < 200)
            customers.push(get_customer(general_last_time, ++i, lambda, u, theta, fixed));
        Customer customer_event;
        if (customers.empty() || (!line.empty() && line.back() < customers.front()))
        {
            customer_event = line.back();
            line.pop_back();
            if (customer_event.situation == GETTING_OUT)
            {
                all_waiting_time += customer_event.wait_time;
                dropped ++;
            }
            else 
                all_waiting_time += customer_event.next_time() - customer_event.enter_time;
        }
        else
        {
            customer_event = customers.front();
            customers.pop(); 
            if (line.size() == K)
                blocked ++;
            else
            {
                customer_event.change_to_in_line();
                line.push_back(customer_event);
            }
        }

        // update line of work
        last_time = customer_event.next_time();
        for (int j = 0; j < line.size(); j++)
            line[j].update_in_line(last_time, (__float128) 1 / line.size());
        sort(line.begin(), line.end());
        reverse(line.begin(), line.end());
    }

    cout << "ended:" << (long double)lambda << " " << fixed << "\n";
    ofstream out_file("../simulation.csv", std::ofstream::app);
	out_file.precision(10);
	out_file.setf(std::ios::fixed, std:: ios::floatfield);
    out_file << (long double) lambda << "," << fixed << ",";
    out_file << (long double) blocked / (long double) n << "," 
            << (long double)  dropped / (long double) n << ",";
    out_file << (long double) all_waiting_time / (long double) last_time << "," << n << "\n";
    out_file.close();
}

#endif
