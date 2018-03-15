#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <list>
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

double deterministic_selection(vector <double> & list, long k, int size)
{

	double median_of_medians;
	vector <double> list_of_medians;

	if (list.size() <= size)
		return (selection_from_bubble_sort(list, k));
	else {
		for (int i = 0; i < list.size(); i = i + size) {
			std::vector<double>n_element_long_segment;
     		if ((i + size) < list.size()) {
				std::vector<double>segment(list.begin()+i, list.begin()+i+(size - 1));
				n_element_long_segment = segment;
			}
			else{
				std::vector<double>segment(list.begin()+i, list.end());
			    n_element_long_segment = segment;
		}
			
			// pick the median of the five-element list (note, it might not have exactly
			// 5 elements all the time).

			if (n_element_long_segment.size() % 2 == 1)
				list_of_medians.push_back(deterministic_selection(n_element_long_segment, (((n_element_long_segment.size() - 1) / 2) + 1), size));
			else {
				list_of_medians.push_back(((deterministic_selection(n_element_long_segment, (n_element_long_segment.size() / 2), size)) + (deterministic_selection(n_element_long_segment, ((n_element_long_segment.size() / 2) + 1), size))) / 2);

			}
		}

			if (list_of_medians.size() % 2 == 1) // list-size is odd, so pick the "middle" element
				median_of_medians = deterministic_selection(list_of_medians, ((list_of_medians.size() - 1) / 2) + 1,size);
			else // list-size is even, so pick the average value of the two "middle" elements
				median_of_medians =
				(deterministic_selection(list_of_medians, (list_of_medians.size()) / 2,size) +
				(deterministic_selection(list_of_medians, ((list_of_medians.size()) / 2) + 1,size))) / 2.0;

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
				return (deterministic_selection(less_than_median_of_medians, k,size));
			else if (k > (less_than_median_of_medians.size() + equal_to_median_of_medians.size()))
				return (deterministic_selection(greater_than_median_of_medians, (k - less_than_median_of_medians.size() - equal_to_median_of_medians.size()),size));
			else
				return (median_of_medians);
		}
}


int main(int argc, char* argv[])
{
	//srand(time(NULL));
	int list_size;
	int cut_size;
	clock_t time_before, time_after;
	float diff;

	vector <double> my_list;
	vector <double> copy_of_my_list;
	sscanf_s(argv[1], "%d", &list_size);
	sscanf_s(argv[2], "%d", &cut_size);
	for (int i = 0; i < list_size; i++) {
		double x = get_standard_cauchy();
		my_list.push_back(x);
		copy_of_my_list.push_back(x);
	}
	
	//for (int i = 0; i < list_size; i++) {
		//my_list.push_back(rand()%100);
		//copy_of_my_list.push_back(rand()%100);
		//std::cout << my_list[i] << endl;
	//}//Test dataset to test whether code indeed working !
	cout << "List_size = " << list_size << endl;
	cout << "Cut_size = " << cut_size << endl;
	cout << "-------------" << endl;
	cout << "Deterministic Selection Based Method" << endl;
	
	time_before = clock(); // recording time before Deterministic Selection algorithm starts
	cout << "The (" << my_list.size() / 2 + 1 << ")-th smallest element in a " << my_list.size() << "-long list is " << deterministic_selection(my_list, my_list.size() / 2 + 1,cut_size) << endl;
	time_after = clock(); // recording time after Deterministic Selection algorithm finishes
	diff = ((float)time_after - (float)time_before);
	cout << diff / CLOCKS_PER_SEC << endl;
	return 0;
}
