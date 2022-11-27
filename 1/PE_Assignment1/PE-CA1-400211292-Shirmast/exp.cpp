#include <fstream>
#include <iostream>

#include "Commons.h"
#include "CommonsAnalitic.h"
#include "CommonsSimulation.h"

class AnaliticCalculatorExp : public AnaliticCalculator
{
	public:
	AnaliticCalculatorExp(long double average_waiting_time, long double u, long double lambda):
		AnaliticCalculator(average_waiting_time, u, lambda)
	{

	}

	long double phi(int n) override
	{
		long double mul = double_factoriel[n];
		for (int i = 0; i <= n; i++)
			mul /= u + (long double) i / average_waiting_time;
		return mul;
	}
};

int main()
{
	calculate_first();
	auto parameters = get_parameters();
	ofstream out_file("../analitic_exp.csv");
	out_file << "lambda,Pb,Pd,Nc\n";
	out_file.close();
	#pragma omp parallel for
	for (int i = 1; i <= 200; i++)
	{
		long double lambda = (long double) i / 10;
		AnaliticCalculatorExp calculator(parameters.first, parameters.second, lambda);
		calculator.calculate();
		ofstream out_file("../analitic_exp.csv", ofstream::app);
		out_file << lambda << "," << calculator.p_b << "," << calculator.p_d << "," << calculator.n_c << "\n";
		out_file.close();
		run_simulation(int (3e7), lambda, parameters.second, parameters.first, true);
	}
	// run_simulation(100000, 10, 1, 3, false);
	return 0;
}
