#ifndef COMMONS_ANALITIC_H_
#define COMMONS_ANALITIC_H_

#include "Commons.h"
//-std=gnu++11

class AnaliticCalculator
{
public:
	__float128 u, lambda;
	__float128 p[K + 4];
	void calculate_p()
	{
		p[0] = 1;
		for (int i = 1;i <= K; i++)
		{
			p[i] = pow((long double) lambda, i);
			for (int j = 1; j <= i; j++)
				p[i] /= u + gamma(j);
		}

		__float128 sum = 0;
		for (int i = 0;i <= K; i++)
			sum += p[i];

		for (int i = 0;i <= K; i++)
			p[i] /= sum;
		
	}
	void calculate_n_c()
	{
		n_c = 0;
		for (int i = 1;i <= K; i++)
			n_c += (__float128) i * p[i];
	}

	void calculate_p_b_d()
	{
		p_b = p[K];
		p_d = 1 - p_b - u / lambda * (1 - p[0]);
	}

	virtual __float128 gamma(int n) = 0;

	__float128 n_c, p_d, p_b;
	__float128 tetha;

	AnaliticCalculator(__float128 tetha, __float128 u, __float128 lambda):
		u(u),
		lambda(lambda),
		tetha(tetha)
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
