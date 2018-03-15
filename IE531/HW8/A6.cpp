//
//  main.cpp
//  FMS 3
//
//  Created by Ramavarapu Sreenivas on 3/12/16.
//  Copyright Â© 2016 Ramavarapu Sreenivas. All rights reserved.
//
//
//  Worked off sample code found at https://github.com/kunalghosh/
//

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <bitset>
#include <set>

using namespace std;

// http://stackoverflow.com/questions/322938/recommended-way-to-initialize-srand
// Creates an awesome seed
unsigned long long mix(unsigned long long a, unsigned long long b, unsigned long long c) {
	a = a - b;  a = a - c;  a = a ^ (c >> 13);
	b = b - c;  b = b - a;  b = b ^ (a << 8);
	c = c - a;  c = c - b;  c = c ^ (b >> 13);
	a = a - b;  a = a - c;  a = a ^ (c >> 12);
	b = b - c;  b = b - a;  b = b ^ (a << 16);
	c = c - a;  c = c - b;  c = c ^ (b >> 5);
	a = a - b;  a = a - c;  a = a ^ (c >> 3);
	b = b - c;  b = b - a;  b = b ^ (a << 10);
	c = c - a;  c = c - b;  c = c ^ (b >> 15);
	return c;
}

int main(int argc, char* argv[]) {
	unsigned long long seed1;

	unsigned long long no_of_words = atol(argv[2]);

	seed1 = mix(clock(), time(NULL), clock() * time(NULL)); //d.count();
	// let us look at the seed
	cout << "Current Seed: " << std::bitset<32>(seed1) << endl;

	// Default value of the number of 2nd moment estimates is 20
	// The user can ask for more/less
	unsigned long long S = 20;
	char *end; // need this for http://www.cplusplus.com/reference/cstdlib/strtoull/
	if (argc >= 4) {
		S = strtoull(argv[3], &end, 10);
	}
	//cout << S << endl;

	unsigned int idx_to_del;

	// X_xcount keeps track of the number "r" (see my notes)
	vector <pair <string, unsigned long>> X_xcount;
	
	// X_listIdx keeps track of the position of a word in X_xcount
	// X_ListIdx is a map that keeps track of the word-positions in X_xcount.
	// The word-positions are stored as vectors, indexed to the name
	// (i.e. string) of the stream-element.
	map <string, set <unsigned int>> X_listIdx;

	// something that I saw on web (https://github.com/kunalghosh/) to keep help
	// with the nitty-gritty of AMS-procedure
	pair <string, unsigned long> p;
	unsigned long n = -1;

	// Using the Marsenne Twister Engine as the "source" of randomness
	// (cf. http://www.cplusplus.com/reference/random/mersenne_twister_engine/mersenne_twister_engine/)
	// In previous code samples we (implicitly) used the Linear-Congruential-Engine
	// (cf. http://www.cplusplus.com/reference/random/linear_congruential_engine/)
	//
	mt19937 mt(seed1);

	// Need this for random sampling of streams
	// took it from http://www.cplusplus.com/reference/random/uniform_real_distribution/
	std::uniform_real_distribution<double> distribution(0.0, 1.0);

	// To get the random index to remove from X_xcount
	typedef uniform_int_distribution<unsigned long long> my_uniform_int;
	my_uniform_int UIID_INT(0, S - 1);

	unsigned long long i = 0;
	std::ifstream file;
	file.open(argv[1]);
	string a;
	while (getline(file, a) && i < no_of_words) {
		i++;
		if (a.empty()) {
			a = "\n";
		}
		n++;
		
		if (n < S) {
			// Get S-many estimators first
			unsigned int index = X_xcount.size();
			p = pair<string, unsigned long>(a, 0);
			X_xcount.push_back(p);
			X_listIdx[a].insert(index); 			
		} else {
			// check if the new stream-element should replace one of the S-many estimators
			if (distribution(mt) < ((double)S) / ((double)n)) {
				// figuring out which of the S-many estimators is to be deleted
				idx_to_del = (int)UIID_INT(mt);
				
				// Remove the index of element to be deleted from its vector
				X_listIdx[X_xcount[idx_to_del].first].erase(idx_to_del);
				if (X_listIdx[X_xcount[idx_to_del].first].size() == 0) {
					X_listIdx.erase(X_xcount[idx_to_del].first);
				}

				// Replace this estimator by the new one
				p = pair<string, unsigned long>(a, 0);
				X_xcount[idx_to_del] = p;
				X_listIdx[a].insert(idx_to_del);
			} 
		}

		// Increase the wordcount for all occurrences of word in transformed list
		for (unsigned int j : X_listIdx[a]) {
			X_xcount[j].second++;
		}
	}
	file.close();
	cout << "#Elements seen: " << n + 1 << "; " << "#Estimators: " << S << endl;

	long long mean = 0;
	for (auto v : X_xcount) {
		mean += (n + 1) * ((2 * v.second) - 1);
	}
	cout << "F2 Estimate: " << mean / X_xcount.size() << endl;

	return 0;
}
