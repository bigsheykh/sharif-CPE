#include <fstream>

#include "Commons.h"
#include "CommonsAnalitic.h"
#include "CommonsSimulation.h"

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

int main(int argc, char** argv)
{
	bool run_type = (argc > 1);
	calculate_first();
	auto parameters = get_parameters();
	ofstream out_file("../analitic_fixed.csv");
	out_file << "lambda,Pb,Pd,Nc\n";
	out_file.close();
	if (!run_type)
	{
		ofstream out_file("../simulation.csv");
		out_file << "lambda,is_tetha_fixed,Pb,Pd,Nc,client_numbers\n";
		out_file.close();
	}
	#pragma omp parallel for
	for (int i = 1; i <= 200; i++)
		if (run_type || i == 50 || i == 100 || i == 150)
			{
				long double lambda = (long double) i / 10;

				AnaliticCalculatorFixed calculator(parameters.first, parameters.second, lambda);
				calculator.calculate();
				ofstream out_file("../analitic_fixed.csv", ofstream::app);
				out_file.precision(10);
				out_file.setf(std::ios::fixed, std:: ios::floatfield);
				out_file << lambda << "," << calculator.p_b << "," << calculator.p_d << "," << calculator.n_c << "\n";
				out_file.close();

				run_simulation(int (3e7), lambda, parameters.second, parameters.first, true);
			}

	return 0;
}
