#ifndef GRAPH_FORMULAS_H
#define GRAPH_FORMULAS_H

#include <vector>

using namespace std; 

typedef vector<vector<int> > bool_formula; 
typedef vector<vector<int> > vec_vec;


void make_ramsey_fml(bool_formula & fml, int vertices, int num1, int num2);
void make_path_fml(bool_formula &fml,int vertices, int path_length1, int path_length2);
void make_cycle_fml(bool_formula &fml,int vertices,int cycle_length1,int cycle_length2);
void make_star_fml(bool_formula &fml,int vertices,int num1,int num2);


#endif