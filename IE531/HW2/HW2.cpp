
// Worked off code outline provided by Prof. Ramavarapu Sreenivas, University of Illinois-Urbana Champaign

#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
using namespace std;

// cf http://www.cplusplus.com/reference/random/uniform_real_distribution/operator()/
// If you want to set a seed -- do it only after debug phase is completed
// otherwise errors will not be repeatable.
// unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
//default_random_engine generator (seed);

default_random_engine generator;

double get_uniform()
{
	std::uniform_real_distribution <double> distribution(0.0, 1.0);
	double number = distribution(generator);
	return (number);
}

double get_standard_cauchy()
{
	return tan(-1.570796327 + (get_uniform()*3.141592654));
}

double selection_from_bubble_sort(vector <double> & list, long k)
{
	int flag = 1;
	float temp;
	for (int i = 1; (i <= list.size()) && flag; i++) {
		flag = 0;
		for (int j = 0; j < (list.size() - 1); j++)
			if (list[j + 1] < list[j]) {
				temp = list[j];
				list[j] = list[j + 1];
				list[j + 1] = temp;
				flag = 1;
			}
	}
	// list[] is now sorted.   Pick the k-th element from the sorted list
	// since the index starts from 0, we are looking for the (k-1)-th element
	return (list[k - 1]);
}

double selection_from_quick_sort(vector <double> & list, long k)
{
	sort(list.begin(), list.end());
	// list[] is now sorted.   Pick the k-th element from the sorted list
	// since the index starts from 0, we are looking for the (k-1)-th element
	return (list[k - 1]);
}

double deterministic_selection(vector <double> & list, long k)
{
	double median_of_medians;
	if (list.size() <= 5)
		return (selection_from_bubble_sort(list, k));
	else {
		std::random_device random_device;
		std::mt19937 engine{ random_device() };
		std::uniform_int_distribution<int> dist(0, list.size() - 1);
		median_of_medians = list[dist(engine)];
	}
	// split list into three lists called: less_than_median_of_medians, equal_to_median_of_medians, and 
	// greater-than-median_of_medians 
	vector <double> less_than_median_of_medians;
	vector <double> equal_to_median_of_medians;
	vector <double> greater_than_median_of_medians;
	for (int i = 0; i < list.size(); i++) {
		if (list[i] < median_of_medians)
			less_than_median_of_medians.push_back(list[i]);
		if (list[i] == median_of_medians)
			equal_to_median_of_medians.push_back(list[i]);
		if (list[i] > median_of_medians)
			greater_than_median_of_medians.push_back(list[i]);
	}
	if (k <= less_than_median_of_medians.size())
		return (deterministic_selection(less_than_median_of_medians, k));
	else if (k > (less_than_median_of_medians.size() + equal_to_median_of_medians.size()))
		return (deterministic_selection(greater_than_median_of_medians, (k - less_than_median_of_medians.size() - equal_to_median_of_medians.size())));
	else
		return (median_of_medians);
}


int main(int argc, char* argv[])
{
	int no_of_trials;
	int list_size;
	clock_t time_before, time_after;
	float diff;

	vector <double> my_list;
	vector <double> copy_of_my_list;
	sscanf_s(argv[1], "%d", &list_size);
	sscanf_s(argv[2], "%d", &no_of_trials);
	ofstream outfile_1(argv[3]);
	for (int i = 0; i < list_size; i++) {
		double x = get_standard_cauchy();
		my_list.push_back(x);
		copy_of_my_list.push_back(x);
	}
	cout << "List_size = " << list_size << endl;
	cout << "Number of trials = " << no_of_trials << endl;
	cout << "-------------" << endl;
	cout << "Deterministic Selection Based Method" << endl;
	for (int j = 0; j < no_of_trials; j++) {
		time_before = clock(); // recording time before Deterministic Selection algorithm starts
		cout << "The (" << my_list.size() / 2 + 1 << ")-th smallest element in a " << my_list.size() << "-long list is " << deterministic_selection(my_list, my_list.size() / 2 + 1) << endl;
		time_after = clock(); // recording time after Deterministic Selection algorithm finishes
		diff = ((float)time_after - (float)time_before);
		cout << diff / CLOCKS_PER_SEC << endl;
		outfile_1 << diff / CLOCKS_PER_SEC << endl;
	}
	return 0;
}
 
