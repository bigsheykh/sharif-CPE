#ifndef COMMONS_H_
#define COMMONS_H_

#include <fstream>
#include <utility>
#include <cmath>
#include <string>

using namespace std;

static constexpr int K = 14;

pair<__float128, __float128> get_parameters()
{
	ifstream parameters("parameters.conf");
	long double tetha, service_rate;
	parameters >> tetha >> service_rate;
	return make_pair((__float128) tetha, (__float128) service_rate);
}

void calculate_first()
{
	srand(time(NULL));
}

#endif
