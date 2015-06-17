#ifndef TWO_SAT_H
#define TWO_SAT_H

#include <unordered_map>
#include <vector>

using namespace std; 

typedef vector<vector<int> > bool_formula; 
typedef unordered_map<int, bool> assignment;



unordered_map<int,bool> two_sat_solver(bool_formula &fml, assignment &asgt);


#endif