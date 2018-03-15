//Worked off code outline provided by Prof. Ramavarapu Sreenivas, University of Illinois-Urbana Champaign

#include <boost/math/distributions.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <random>
#include <chrono>

using namespace std;
unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine gen(seed);
default_random_engine generator(seed);


double get_uniform()
{
	std::uniform_real_distribution <double> distribution(0.0, 1.0);
	double number = distribution(gen);
	return (number);
}
double get_binomial(long s,double r)
{
	std::binomial_distribution<long> d1(s, r);
	long binom = d1(generator);
	return (binom);
}
int main(int argc, char* argv[])
{
	long no_of_successes_for_A, no_of_successes_for_B;
	long sample_trial_size, repeated_no_trials;

	sscanf_s(argv[1], "%ld", &sample_trial_size);
	sscanf_s(argv[2], "%ld", &no_of_successes_for_A);
	sscanf_s(argv[3], "%ld", &no_of_successes_for_B);
	sscanf_s(argv[4], "%ld", &repeated_no_trials);
	ofstream outfile_1(argv[5]);

	//long alpha_A = 1 + no_of_successes_for_A;
	//long beta_A = 1 + (sample_trial_size - no_of_successes_for_A);
	//long alpha_B = 1 + no_of_successes_for_B;
	//long beta_B = 1 + (sample_trial_size - no_of_successes_for_B);//

	vector<double> r1, r2, profitA, profitB;
	vector<long> no1, no2;
	for (int i = 0; i < repeated_no_trials; i++)
	{
		r1.push_back(get_uniform());
		r2.push_back(get_uniform());
	}
	for (int j = 0; j < r1.size(); j++)
	{
		no1.push_back(get_binomial(sample_trial_size, r1[j]));
		no2.push_back(get_binomial(sample_trial_size, r2[j]));
	}
	for (int k = 0; k < no1.size(); k++) {
		if (no1[k] == no_of_successes_for_A && no2[k] == no_of_successes_for_B)
		{   
			//outfile_1 << r2[k]-r1[k]<< endl;
			outfile_1 << ((r2[k]*1000-300) - (r1[k]*1000-30)) << endl;
			cout << ((r2[k] * 1000 - 300) - (r1[k] * 1000 - 30)) << endl;
		}
	}

	// implementing the formula from http://www.evanmiller.org/bayesian-ab-testing.html
	//for (int j = 0; j < repeated_no_trials; j++) {
		//for (int i = 0; i <= alpha_B; i++)
		//{
		//	prob += (boost::math::beta(alpha_A + i, beta_B + beta_A)) /
		//		((beta_B + i)*(boost::math::beta(1 + i, beta_B))*(boost::math::beta(alpha_A, beta_A)));
		//}
		//cout << prob << endl;
		//prob = 0.0;
	//}//
	return 0;
}

