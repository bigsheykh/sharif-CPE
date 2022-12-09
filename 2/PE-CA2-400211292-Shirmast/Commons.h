#ifndef COMMONS_H_
#define COMMONS_H_

#include <fstream>
#include <utility>
#include <cmath>
#include <string>

using namespace std;

static long double double_factoriel[20];
static constexpr int K = 14;

pair<long double, long double> get_parameters()
{
	ifstream parameters("parameters.conf");
	long double average_waiting_time, service_rate;
	parameters >> average_waiting_time >> service_rate;
	return make_pair(average_waiting_time, service_rate);
}


void calculate_first()
{
	srand(time(NULL));
	double_factoriel[0] = 1;
	for (int i = 1;i < 20; i++)
		double_factoriel[i] = double_factoriel[i - 1] * i;
}

#endif
