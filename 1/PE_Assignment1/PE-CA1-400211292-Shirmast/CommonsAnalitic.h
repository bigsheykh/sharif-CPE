#ifndef COMMONS_ANALITIC_H_
#define COMMONS_ANALITIC_H_

#include "Commons.h"

class AnaliticCalculator
{
public:
	long double u, lambda;
	long double p[K + 4];
	void calculate_p()
	{
		p[0] = 1;
		p[1] = lambda / u;
		for (int i = 2;i <= K; i++)
			p[i] = pow(lambda, i) / double_factoriel[i - 1] * phi(i-1) ;

		long double sum = 0;
		for (int i = 0;i <= K; i++)
			sum += p[i];

		for (int i = 0;i <= K; i++)
			p[i] /= sum;
		
	}
	void calculate_n_c()
	{
		n_c = 0;
		for (int i = 1;i <= K; i++)
			n_c += (long double) i * p[i];
	}

	void calculate_p_b_d()
	{
		p_b = p[K];
		p_d = 1 - p_b - u / lambda * (1 - p[0]);
	}

	virtual long double phi(int n) = 0;
	long double n_c, p_d, p_b;
	long double average_waiting_time;
	AnaliticCalculator(long double average_waiting_time, long double u, long double lambda):
		u(u),
		lambda(lambda),
		average_waiting_time(average_waiting_time)
	{
	}

	void calculate()
	{
		calculate_p();
		calculate_n_c();
		calculate_p_b_d();
	}

};

#endif
