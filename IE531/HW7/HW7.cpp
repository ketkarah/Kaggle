//
//  main.cpp
//  FMS 1
//
//  Created by Ramavarapu Sreenivas on 3/11/16.
//  Copyright Â© 2016 Ramavarapu Sreenivas. All rights reserved.
//
//  Worked off sample code found at https://github.com/kunalghosh/
// 
#include <cstdlib>
#include <cmath>
#include <unordered_map>
#include <functional>
#include <cstring>
#include <iostream>
#include <map>
#include <chrono>
#include <ctime>
#include <string>
#include <fstream>
#include <list>
#include <algorithm>
#include <vector>
#include "hashes.h"
#include <numeric>
#include <random>

using namespace std;
typedef unordered_map <string, long long> Wc;
using namespace std;
unsigned seed1 = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();


unsigned long long rho(unsigned long hashVal)
{
    // computing the tail-length of hashVal (we are looking for strings of 0's)
    unsigned long long rho = 0;
    int mask = 0x1;
    if(hashVal != 0)
    {
        // while-loop is executed only of the least-significant-bit of hashVal is 0
        while((hashVal & mask) == 0)
        {
            rho++;
            hashVal = hashVal  >> 1; // right-shift the hashVal by 1 bit position
        }
    }
    return rho;
}

void set_bit(unsigned long long &bit_str, long long pos)
{
    // see BITMAP operation in https://en.wikipedia.org/wiki/Flajoletâ€“Martin_algorithm
    long long mask = 0x1 << pos;
    bit_str = bit_str | mask;
}

unsigned long long get_estimated_distinct_nums(unsigned long long bit_str)
{
    // see https://en.wikipedia.org/wiki/Flajoletâ€“Martin_algorithm for details
    double phi = 0.77351;
    unsigned long long pos = 0;
    int mask = 0x1;
    
    while((bit_str & mask)!= 0)
    {
        bit_str = bit_str >> 1;
        pos++;
    }
    return (pow(2,pos) / phi) ;
}

//Find the mean of a vector
double find_mean(vector <long long> & v) {
	double sum = 0;
	for (int i = 0; i < v.size(); i++)
		sum += v[i];
	cout << "Mean of Runs = " << sum/v.size() << endl;
	return sum/v.size();
}

//Find the median of a vector
template<typename T>
T find_median(vector <T> & bin)
{
	//cout << "COde here" << endl;
	sort(bin.begin(), bin.end());
	return bin[5];
}


long long fmsEstimate(vector <string> v) {
	random_shuffle(v.begin(), v.end());
	string hashFunc = "mm2";
	unsigned int bit_limit = 0;
	long long dsums;
	string a;
	ios::sync_with_stdio(false);
	//ios::sync_with_stdio(false);
	unsigned long long bit_fields = 0;
	for(int i = 0; i < v.size(); i++)
	{
		a =v[i];
		
		//cout << a << endl;
		set_bit(bit_fields, int(rho(hashstr(hashFunc, a, bit_limit, seed1, seed1))));
		//seeding = find_seeds();
		//bit(bit_fields, a,seeding);
	}
	dsums = get_estimated_distinct_nums(bit_fields);
	return dsums;
}


int main(int argc, char *argv[])
{
	clock_t time_before, time_after;
    
    // we are going to use the MurmurHash2 as our hash function
    // to get different outcomes, we are relying on changing seed1
    // (which is set using the system clock; see above).
	typedef std::chrono::high_resolution_clock myclock;
	myclock::time_point beginning = myclock::now();

	// obtain a seed from the timer
	myclock::duration d = myclock::now() - beginning;
	//seed1 = d.count();

	// we are going to use the MurmurHash2 as our hash function
	// to get different outcomes, we are relying on changing seed1
	// (which is set using the system clock; see above).
	string hashFunc = "mm2";
	unsigned int bit_limit = 0;

	string a;
	ios::sync_with_stdio(false);
	//ios::sync_with_stdio(false);
    unsigned long long bit_fields = 0;
	Wc wc;
	vector <string> mystream;
	vector <double> means;
	vector <long long> medians;
	int count = 0;
	long long dsums=0;
	long long med1=0, med2=0, med3=0, med4=0;
	long long mean=0;
	vector <long long> bin1, bin2, bin3;
	time_before = clock();
	while (getline(cin, a))
	{
		count += 1;
		if (a.empty())
		{
			a = "\n";
		}
		mystream.push_back(a);
		//cout << a << endl;
		wc[a]++;
		set_bit(bit_fields, int(rho(hashstr(hashFunc, a, bit_limit, seed1, seed1))));
		//seeding = find_seeds();
		//bit(bit_fields, a,seeding);
	}
	//dsums = get_estimated_distinct_nums(bit_fields);
	//dsums = fmsEstimate(mystream);
	//runs.push_back(dsums);
	for (int j = 1; j <= 11; j++) {
		vector <long long> runs;
		for (int i = 1; i <= 11; i++) {
			runs.push_back(fmsEstimate(mystream));
		}
		means.push_back(find_mean(runs));
		medians.push_back(find_median(runs));
	}
	cout << "FMS Estimate - Median of Means = " << find_median(means) << endl;
	cout << "FMS Estimate - Median of Medians = " << find_median(medians) << endl;


	long long sum = 0, f2 = 0;
	for (Wc::const_iterator it = wc.begin(); it != wc.end(); ++it)
	{
		//cout << it->first << "\t" << it->second << endl;
		sum += 1;
	}
	
    time_after = clock();
	cout << "Actual No. of Distinct Words = " << sum << endl;
    //cout << "Estimated no of distinct words by Flajolet Martin="<<dsums<< endl;
    cout << "Time: " << ((double) time_after - (double) time_before)/CLOCKS_PER_SEC << " seconds"<< endl;
	//cout << "My stream contains " << mystream.size() << "  elements" << endl;
    return 0;
	/*ofstream outfile("results.txt", std::ios_base::app);
	outfile << sum << "," << find_median(means) << "," << find_median(medians) << endl;
	outfile.close();*/
}
