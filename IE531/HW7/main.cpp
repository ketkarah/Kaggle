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
#include "hashes.h"

using namespace std;

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


int main(int argc, char *argv[])
{
    clock_t time_before, time_after;
    unsigned long long seed1;
    
    // took this off http://www.cplusplus.com/reference/random/mersenne_twister_engine/seed/
    typedef std::chrono::high_resolution_clock myclock;
    myclock::time_point beginning = myclock::now();
    
    // obtain a seed from the timer
    myclock::duration d = myclock::now() - beginning;
    seed1 = d.count();
    
    // we are going to use the MurmurHash2 as our hash function
    // to get different outcomes, we are relying on changing seed1
    // (which is set using the system clock; see above).
    string hashFunc = "mm2";
    unsigned int bit_limit = 0;
    
    string a;
    ios::sync_with_stdio(false);
    unsigned long long bit_fields = 0;
    time_before = clock();
    while(getline(cin,a))
    {
        if(a.empty())
        {
            a = "\n";
        }
        set_bit(bit_fields, int(rho(hashstr(hashFunc,a,bit_limit,seed1,seed1))));
    }
    time_after = clock();
    cout << get_estimated_distinct_nums(bit_fields) << endl;
    cout << "Time: " << ((double) time_after - (double) time_before)/CLOCKS_PER_SEC << " seconds"<< endl;
    return 0;
}


