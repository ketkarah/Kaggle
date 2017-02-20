#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <random>
#include <chrono> 
#include <cmath>
#include <fstream>

#define HEADS 1
#define TAILS 0
using namespace std;

// cf http://www.cplusplus.com/reference/random/uniform_real_distribution/operator()/
// If you want to set a seed -- do it only after debug phase is completed
// otherwise errors will not be repeatable.
// unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
// default_random_engine generator (seed);
// initially just use default_random_engine generator;

unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);

// I need this uniform i.i.d. r.v. generator to simulate the unfair-coin in
// simulate_coin_toss(double heads_probability)
double get_uniform()
{
	std::uniform_real_distribution <double> distribution(0.0, 1.0);
	double number = distribution(generator);
	return (number);
}

// simulating the unfair-coin; HEADS (resp. TAILS) with probability "heads_probability"
// (resp. "1-heads_probability")
int simulate_coin_toss(double heads_probability)
{
	if (get_uniform() < heads_probability)
		return HEADS;
	else
		return TAILS;
}

// This procedure uses the unfair-coin and creates a fair-outcome by using
// "fair-heads" := <HEADS, TAILS> and "fair-tails" := <TAILS, HEADS>
// (Note, <HEADS, HEADS> and <TAILS, TAILS> will be rejected (which is an
// inefficiency/loss)
/*int simulate_fair_coin_toss_from_unfair_coin(double heads_probability)
{
	// while (out1 + out2 != 1) { 
		int out1 = simulate_coin_toss(heads_probability);
		int out2 = simulate_coin_toss(heads_probability);
		if (out1 == 1 && out2 == 0) {
			return 1;
		}
		else if (out1 == 0 && out2 == 1) {
			return 0;
		}
		else{
			return simulate_fair_coin_toss_from_unfair_coin(heads_probability);
	}
}*/

int simulate_fair_coin_toss_from_unfair_coin(double heads_probability)
{
	int out1 = simulate_coin_toss(heads_probability);
	int out2 = simulate_coin_toss(heads_probability);
		if (out1 == 1 && out2 == 0) {
			//cout << out1 << " " << out2 << endl;
			return 1;
		}
		else if (out1 == 0 && out2 == 1) {
			//cout << out1 << " " << out2 << endl;
			return 0;
		}
		else {
		simulate_fair_coin_toss_from_unfair_coin(heads_probability);
		}
}

// This procedure gets 32 fair-tosses from (> 32 tosses) of the unfair-coin
// For every "fair-heads" (resp. "fair-tails") we write down a "1" (resp. "0")
// and interpret the 32-bit unsigned integer as a random unsigned int between
// 0 and 2^32-1.
double get_uniform_from_unfair_coin(double heads_probability)
{
	int arr[32];
	double sum = 0.0;
	for (int i = 0; i < 32; i++) {
		arr[i] = simulate_fair_coin_toss_from_unfair_coin(heads_probability);
	}
	for (int j = 0; j < 32; j++) {
		sum += (arr[j] * pow(2, j));
	}
	double div = sum / (pow(2, 32) - 1);
	return div;
}

// This generates i.i.d. rv.s that are exponentially distributed with rate lambda
// using an unfair-coin
	double get_exp_from_unfair_coin(double lambda, double heads_probability)
{
	return ((-1.0 / lambda)*log(get_uniform_from_unfair_coin(heads_probability)));
}

// This generates i.i.d. rv.s that are unit-normal distributed using an unfair-coin
// (I am using the Box-Muller method here)
double get_gaussian_from_unfair_coin(double heads_probability)
{
	return (sqrt(-2.0*log(get_uniform_from_unfair_coin(heads_probability)))*cos(2 * 3.141592654*get_uniform_from_unfair_coin(heads_probability)));
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	double probability_of_heads, lambda;
	long no_of_trials;
	probability_of_heads = (rand() % 100 + 1) / (double)100; //((double)rand()) / RAND_MAX;
	sscanf_s(argv[1], "%ld", &no_of_trials);
	sscanf_s(argv[2], "%lf", &lambda);
	ofstream outfile_1(argv[3]);
	ofstream outfile_2(argv[4]);

	cout << "Generating i.i.d. Unit-Normals and Exponentials from an Unfair-Coin Toss" << endl;
	cout << "Probability of Heads of the unfair coin   = " << probability_of_heads << endl;
	cout << "Number of trials                          = " << no_of_trials << endl;
	cout << "Output File Name for the Unit-Normal Data = " << argv[3] << endl;
	cout << "Output File Name for the Exponential Data = " << argv[4] << endl;
	cout << "Rate of Exponential RVs                   = " << lambda << endl;

	// i.i.d. exponential r.v. generation
	int count[100];
	for (int i = 0; i < 100; i++)
		count[i] = 0;

	for (int i = 0; i < no_of_trials; i++)
	{
		double y = get_exp_from_unfair_coin(lambda, probability_of_heads);
		for (int j = 0; j < 100; j++)
			if (y < ((double)j / 10))
				count[j]++;
	}

	// Exponential CDF -- experimental vs. theory
	for (int j = 0; j < 100; j++)
		outfile_2 << ((double)j / 10) << ", " << ((double)count[j]) / ((double)no_of_trials) << ", " << (1.0 - exp(-1.0*lambda*((double)j / 10))) << endl;

	// i.i.d. unit-normal generation
	double cdf_gaussian[100];
	for (int i = 0; i < 100; i++)
	{
		count[i] = 0;
		cdf_gaussian[i] = 0.0;
	}

	for (int i = 0; i < no_of_trials; i++)
	{
		double y = get_gaussian_from_unfair_coin(probability_of_heads);
		for (int j = 0; j < 100; j++)
			if (y < ((float)(j - 50) / 10))
				count[j]++;
	}
	// Unit-Normal CDF -- experimental vs. theory
	for (int j = 1; j < 100; j++)
		cdf_gaussian[j] = cdf_gaussian[j - 1] +
		((1 / sqrt(2 * 3.141592654))*exp(-1.0*(((double)(j - 50) / 10)*
		((double)(j - 50) / 10)) / 2) / 10.0);

	for (int j = 0; j < 100; j++)
		outfile_1 << ((double)(j - 50) / 10) << ", " <<
		((double)count[j]) / ((double)no_of_trials) << ", " << cdf_gaussian[j]
		<< endl;
	//getchar();
	return 0;
}