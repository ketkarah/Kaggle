//Worked off code outline provided by Prof. Ramavarapu Sreenivas, University of Illinois-Urbana Champaign

# include <iostream>
# include <unordered_map>
# include <string>
# include <math.h>
using namespace std;
typedef unordered_map <string, long long> Wc;

int main(int argc, char *argv[]) {
	string line;
	Wc wc;
	long long sum = 0, f2 = 0;
	while (getline(cin, line)) {
		if (line.empty()) {
			line = "\n";
		}
		wc[line]++;
	}
	for (Wc::const_iterator it = wc.begin(); it != wc.end(); ++it)
	{	
		//cout << it->first << "\t" << it->second << endl;
		sum += 1;
		f2 += pow(it->second, 2);
	}
	cout << "sum = " << sum << endl;
	cout << "f2 = " << f2 << endl;

	return 0;
}
