#include <fstream>

#include "Commons.h"
#include "CommonsAnalitic.h"
#include "CommonsSimulation.h"

class AnaliticCalculatorFixed : public AnaliticCalculator
{
	public:
	AnaliticCalculatorFixed(__float128 tetha, __float128 u, __float128 lambda):
		AnaliticCalculator(tetha, u, lambda)
	{

	}

	__float128 gamma(int n) override
	{
		return u / (exp((long double) (u * tetha / n)) - 1);
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
				__float128 lambda = (__float128) i / 10;

				// AnaliticCalculatorFixed calculator(parameters.first, parameters.second, lambda);
				// calculator.calculate();
				// ofstream out_file("../analitic_fixed.csv", ofstream::app);
				// out_file.precision(10);
				// out_file.setf(std::ios::fixed, std:: ios::floatfield);
				// out_file << (long double) lambda << "," <<
				// 		(long double) calculator.p_b << "," <<
				// 		(long double) calculator.p_d << "," <<
				// 		(long double) calculator.n_c << "\n";
				// out_file.close();
				run_simulation(int (3e7), lambda, parameters.second, parameters.first, true);
			}

	return 0;
}
