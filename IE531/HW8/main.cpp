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
#include <set>
#include<bitset>

using namespace std;

int main(int argc, char* argv[])
{
    unsigned long long seed1;
    
    // took this off http://www.cplusplus.com/reference/random/mersenne_twister_engine/seed/
    typedef std::chrono::high_resolution_clock myclock;
    myclock::time_point beginning = myclock::now();
    
    // obtain a seed from the timer
    myclock::duration d = myclock::now() - beginning;
    seed1 = d.count();
    // let us look at the seed
    cout << "Current Seed: " << std::bitset<32> (seed1) << endl;
    
    // Default value of the number of 2nd moment estimates is 20
    // The user can ask for more/less
    unsigned long long S = 20;
    char *end; // need this for http://www.cplusplus.com/reference/cstdlib/strtoull/
    if(argc >= 2)
    {
        S = strtoull(argv[1], &end,10);
    }
    unsigned long long idx_to_del;
    string line_to_del;
    
    // X_ListIdx is a map that keeps track of the stream-positions where the
    // string-argument appeared in the data-stream.  The stream-positions are
    // stored as vectors, indexed to the name (i.e. string) of the stream-element.
    // Wrt my notes -- X_listIdx[j] keeps track of the stream-positions where "j"
    // occurred
    map <string, vector<unsigned long> > X_listIdx;
    
    // X_xcount keeps track of the number "r" (see my notes)
    vector <pair<string, unsigned long> > X_xcount;
    
    // something that I saw on web (https://github.com/kunalghosh/) to keep help
    // with the nitty-gritty of AMS-procedure
    pair <string , unsigned long> p;
    unsigned long n = -1;
    
    // Using the Marsenne Twister Engine as the "source" of randomness
    // (cf. http://www.cplusplus.com/reference/random/mersenne_twister_engine/mersenne_twister_engine/)
    // In previous code samples we (implicitly) used the Linear-Congruential-Engine
    // (cf. http://www.cplusplus.com/reference/random/linear_congruential_engine/)
    //
    mt19937 mt (seed1);

    // Need this for random sampling of streams
    // took it from http://www.cplusplus.com/reference/random/uniform_real_distribution/
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    
    // To get the random index to remove from X_xcount
    typedef uniform_int_distribution<unsigned long long> my_uniform_int;
    my_uniform_int UIID_INT(0,S);
	
    ios::sync_with_stdio(false);
    string a;
    while(getline(cin, a))
    {
        if(a.empty())
        {
            a = "\n";
        }
        n += 1;
        if(X_listIdx.size() < S)
        {
            if(X_listIdx.find(a) == X_listIdx.end()) // check if a was seen before
            {
                p = pair<string, unsigned long> (a,1);
                X_xcount.push_back(p);
                X_listIdx[a].push_back(n);
            }
            else
            {
                for(auto idx: X_listIdx[a]){
                    X_xcount[idx].second += 1;
                }
            }
            
        } else
        {
            // check if the new stream-element should replace one of the S-many estimators
            if(distribution(mt) < ((double) S)/((double) n))
            {
                // figuring out which of the S-many estimators is to be deleted
                idx_to_del = UIID_INT(mt);
                line_to_del = X_xcount[idx_to_del].first;
                X_listIdx[line_to_del].erase(remove(X_listIdx[line_to_del].begin(), X_listIdx[line_to_del].end(), idx_to_del), X_listIdx[line_to_del].end());
                if(X_listIdx[line_to_del].empty())
                {
                    X_listIdx.erase(X_listIdx.find(line_to_del));
                }
                // adding the new stream-element as the new member of the S-many estimators
                p = pair<string, unsigned long>(a,1);
                X_xcount[idx_to_del] = p;
                X_listIdx[a].push_back(idx_to_del);
            } else {
                // line was not picked
                if(X_listIdx.find(a) != X_listIdx.end()){
                    for(auto idx: X_listIdx[a]){
                        X_xcount[idx].second += 1;
                    }
                }
            }
        }
    }
    
    cout << "#Elements seen: " << n+1 << "; " << "#Estimators: " << S << endl;
        
    long long mean = 0;
    for (auto v: X_xcount)
    {
        mean += (n+1) * ((2*v.second) - 1);
    }
    cout << "F2 Estimate: " << mean/X_listIdx.size() << endl;

    return 0;
}
