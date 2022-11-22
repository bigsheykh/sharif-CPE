#include <fstream>

#include "commons.h"

class AnaliticCalculatorFixed : public AnaliticCalculator
{
	public:
	AnaliticCalculatorFixed(long double average_waiting_time, long double u, long double lambda):
		AnaliticCalculator(average_waiting_time, u, lambda)
	{

	}

	long double phi(int n) override
	{
		long double sum = 0;
		for (int i = 0; i < n; i++)
			sum += pow(u * average_waiting_time, i) / double_factoriel[i];
		return double_factoriel[n] / pow(u, n + 1) * (1 - exp(-u * average_waiting_time) * sum );
	}
};

int main()
{
	calculate_first();
	auto parameters = get_parameters();
	ofstream out_file("analitic_fixed.csv");
	out_file << "lambda,Pb,Pd,Nc\n";
	for (int i = 1; i <= 200; i++)
	{
		long double lambda = (long double) i / 10;
		AnaliticCalculatorFixed calculator(parameters.first, parameters.second, lambda);
		calculator.calculate();
		out_file << lambda << "," << calculator.p_b << "," << calculator.p_d << "," << calculator.n_c << "\n";
	}
	return 0;
}
