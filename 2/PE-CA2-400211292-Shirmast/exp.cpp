#include <fstream>
#include <iostream>

#include "Commons.h"
#include "CommonsAnalitic.h"
#include "CommonsSimulation.h"

class AnaliticCalculatorExp : public AnaliticCalculator
{
	public:
	AnaliticCalculatorExp(__float128 tetha, __float128 u, __float128 lambda):
		AnaliticCalculator(tetha, u, lambda)
	{

	}

	__float128 gamma(int n) override
	{
		return (__float128) n / tetha;
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
		__float128 lambda = (__float128) i / 10;

		AnaliticCalculatorExp calculator(parameters.first, parameters.second, lambda);
		calculator.calculate();
		ofstream out_file("../analitic_exp.csv", ofstream::app);
		out_file << (long double) lambda << "," <<
				(long double) calculator.p_b << "," <<
				(long double) calculator.p_d << "," <<
				(long double) calculator.n_c << "\n";
		out_file.close();

		run_simulation(int (3e7), lambda, parameters.second, parameters.first, false);
	}
	return 0;
}
