#include <fstream>
#include <iostream>

#include "commons.h"

class AnaliticCalculatorExp : public AnaliticCalculator
{
	public:
	AnaliticCalculatorExp(long double average_waiting_time, long double u, long double lambda):
		AnaliticCalculator(average_waiting_time, u, lambda)
	{

	}

	long double phi(int n) override
	{
		long double sum = 0;
		for (int i = 0; i <= n; i++)
			sum += u + (long double) i / average_waiting_time;
		if (n < 4)
			cout << n << ":" << double_factoriel[n] / sum << endl;
		return double_factoriel[n] / sum;
	}
};
int main()
{
	calculate_first();
	auto parameters = get_parameters();
	ofstream out_file("analitic_exp.csv");
	out_file << "lambda,Pb,Pd,Nc\n";
	for (int i = 1; i <= 200; i++)
	{
		long double lambda = (long double) i / 10;
		AnaliticCalculatorExp calculator(parameters.first, parameters.second, lambda);
		calculator.calculate();
		out_file << lambda << "," << calculator.p_b << "," << calculator.p_d << "," << calculator.n_c << "\n";
	}
	return 0;
}
