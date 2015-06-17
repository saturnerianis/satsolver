/*
 * Hannah Christenson
 *

 */
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <unordered_map>
#include <ctime>
#include "two_sat.h"
#include "graph_formulas.h"

using namespace std;

typedef vector<vector<int> > bool_formula;
typedef unordered_map<int, bool> assignment;
typedef bool_formula::iterator fml_itr;
typedef vector<int>::iterator v_itr;

//function prototypes for printing/making formulas
void print_formula(const bool_formula &fml);
void print_assignment(const assignment &asgt);


//returns the index of num in vector v. -1 if num is not present
int find(const vector<int> &v, int num){ 
	for(int i = 0; i < v.size(); ++i)
		if (v[i] == num) return i;
	return -1;
}

//assigns value in a way that simplifies the assignment map (un peu)
void assign_true(assignment &asgt, int var){
	if(var > 0) asgt[var] = true;
	else asgt[-var] = false;
}



/* removes instances of var, assuming it to be true, from fml.
 * deletes clauses in which var occurs and removes -var from 
 * clauses in which it occurs. If -var occurs in a unit clause
 * it returns false to indicate that assigning true to var
 * is not a satisfying assignment
 */
bool simplify(bool_formula &fml, int var){
	for(int i = 0; i < fml.size(); i++){ 
		if(find(fml[i], var) > -1){ //instance of var:
			fml.erase(fml.begin()+i); //delete clause
			i--;
		}
		else{ //remove any instances of -var in the clause
			int index = find(fml[i], -var);
			while(index > -1){
				if(fml[i].size() == 1) return false; //unit check
				fml[i].erase(fml[i].begin()+index); 
				index = find(fml[i], -var);
			}
		}
	}
	return true;
}


/*
 * checks for unit clauses, simplifying the formula and assigning
 * the variable to true if one is found, then repeating the process
 * @returns false if the simplification demonstrates that the
 * formula is not satisfiable
 */
bool check_unit_clauses(bool_formula &fml, assignment &asgt){
	for(vector<int> vec: fml){
		if(vec.size() == 1){
			assign_true(asgt,vec[0]);
			if(simplify(fml, vec[0])) 
				return check_unit_clauses(fml, asgt);
			else return false;
		}
	}
	// if(unit){
	// 	assign_true(asgt,var);
	// 	if(simplify(fml, var)) 
	// 		return check_unit_clauses(fml, asgt);
	// 	else return false;
	// }
	return true;
}


/*
 * returns 0 if no int occurs more than the specified # times, otherwise returns 
 * the number representing the literal which occurs most often
 */
int find_most_common_var(const bool_formula &fml){
	unordered_map<int,int> counts;

	for(vector<int> vec: fml)
		for(int var: vec) counts[var]++;
	int max = (counts.begin())->first;
	for(pair<int,int> count: counts)
		if(count.second > counts[max]) max = count.first;	

	// if(counts[max] >= 5*fml.size()/6) return max; 
	// return 0;
	return max;
}


int find_most_common_var_weighted(const bool_formula &fml){
	unordered_map<int,double> weights;
	for(vector<int> vec: fml)
		for(int var: vec) weights[var] += 1/vec.size();
	int max = (weights.begin())->first;
	for(pair<int,int> weight: weights)
		if(weight.second > weights[max]) max = weight.first;

	if(weights[max] > 2) return max;
	return 0; 
}


//returns the index of the shortest clause in fml
int find_smallest_clause(const bool_formula &fml){
	int min_index = 0;
	for(int i = 0; i < fml.size(); ++i)
		if(fml[i].size() < fml[min_index].size()) min_index = i;	
	return min_index;
}

//-1 represents a neg literal found, 1 a pos literal - replace with 5 if both found
// int checking_pure_literals(const bool_formula &fml){
// 	unordered_map<int,int> check;
// 	for(vector<int> vec: fml)
// 		for(int var: vec){
// 			if(var < 0)
// 				if(check[abs(var)] > 0) check[abs(var)] = 0;
// 				else check[abs(var)] = -1;
// 			else //var is positive
// 				if(check[abs(var)]) < 0) check[abs(var)]
// 		}
// }




/*
 * a recursive algorithm to determine if the fml is satisfiable.
 * satisfiability is stored in the key of zero in the assignment
 */
assignment solve_sat(bool_formula &fml, assignment& asgt){
	assignment empty;

	//step one. remove unit clauses and simplify
	//if (!check_unit_clauses(fml, asgt)) return empty;

	for (int i = 0; i < (int)fml.size();){
		if (fml[i].size() == 1){
			assign_true(asgt, fml[i][0]);
			if (!simplify(fml, fml[i][0])) return empty;
			i = 0;
		}
		else i++;
	}

	//check the base case - no more formula to evaluate = satisfying asgt
	if(fml.empty()){
		asgt[0] = true;
		return asgt;
	}

	//step two. check if fml is in 2cnf form - if so, call 2sat
	bool two_cnf = true;
	for(vector<int> vec: fml)
		if(vec.size() > 2) two_cnf = false;
	if(two_cnf) return two_sat_solver(fml, asgt); 

	// //step 2.5? if there is any clause of length two, recurse on that clause
	// bool two_clause = false;
	// for(vector<int> vec: fml){
	// 	if(vec.size() == 2){
	// 		assignment original_asgt = asgt; 
	// 		bool_formula original_fml = fml;

	// 		assign_true(asgt,vec[0]); //try true for vec[0]
	// 		if(simplify(fml, vec[0])){
	// 			assignment soln = solve_sat(fml, asgt); 
	// 			if(soln[0]) return soln; 
	// 		}
	// 		assign_true(original_asgt,-vec[0]); //try false
	// 		if(simplify(original_fml, -vec[0]))
	// 			return solve_sat(original_fml, original_asgt); 
	// 		else return empty; 
	// 	}
	// }


	//step 3. if there is a variable occurring in at least a quarter of 
	//the clauses, recurse on assigning that variable true and false. 
	int var = find_most_common_var(fml);
	if(var != 0){
		assignment original_asgt = asgt; 
		bool_formula original_fml = fml;

		assign_true(asgt,var); //try true
		if(simplify(fml, var)){
			assignment soln = solve_sat(fml, asgt); 
			if(soln[0]) return soln; 
		}
		assign_true(original_asgt,-var); //try false
		if(simplify(original_fml, -var))
			return solve_sat(original_fml, original_asgt); 
		else return empty; 
	}


	//step 4. otherwise, find the clause with the least #variables and recurse
	//on the possible assignments that make this clause true 	
	int index = find_smallest_clause(fml);
	vector<int> clause = fml[index];
	for(int true_var = 0; true_var < clause.size(); true_var++){
		assignment temp_asgt = asgt;
		bool_formula temp_fml = fml;
		bool simplified_successfully = true;

		for(int i = 0; i < true_var; i++){ //preceding variables assigned false
			assign_true(temp_asgt,-clause[i]);
			if(!simplify(temp_fml, -clause[i])){
				simplified_successfully = false; 
				break;
			} 
		}
		assign_true(temp_asgt,clause[true_var]); //one var assigned true
		if( !simplify(temp_fml, clause[true_var]) ) simplified_successfully = false;

		if(simplified_successfully){
			assignment the_soln = solve_sat(temp_fml, temp_asgt);
			if(the_soln[0]) return the_soln;
		}
	}
	return empty;
}








int main(){
 	// bool_formula fml;

 	// char c;
 	// do {

	 // 	string literals = "";
	 // 	cout << "List the literals as numbers separated by spaces" << endl;
	 // 	getline(cin, literals);

	 // 	istringstream in(literals);
	 // 	vector<int> clause;

	 // 	int var;
	 // 	while(in >> var){
	 // 		if(var == 0) 
	 // 			cout << "0 cannot be used as a literal, it will be ignored" << endl;
	 // 		else clause.push_back(var); 
	 // 	}
	 // 	if(!clause.empty()) fml.push_back(clause); //check that clause is not empty??

	 // 	cout << "Is there another clause? Input 'y' if yes.";
	 // 	cin >> c;
	 // 	getline(cin, literals); //finish reading the line

 	// } while(c == 'y');

 	// print_formula(fml);

 // 	bool_formula fml;

 // 	ifstream file;
	// file.open("ramsey934.txt"); 

	// string line = "";
	// while(getline(file, line)){
	// 	istringstream in(line);
	// 	vector<int> clause;

	// 	int var; 
	// 	while(in >> var){
	// 		clause.push_back(var);
	// 	}
	// 	if(!clause.empty()) fml.push_back(clause);
	// }



 	// bool_formula fml;
  //   make_ramsey_fml(fml, 10, 4, 4);
 	// assignment asgt;
 	// assignment results = solve_sat(fml, asgt); 
 	// cout << "satisfiable: " << results[0] << endl; 
 	// print_assignment(results); 
 	//cout << "Write path_length1,path_length2, and vertices: ";
 	cout << "write ramsey parameters: ";
 	int m,n,s;
 	cin >> m;
 	cin >> n;
 	cin >> s;


 	bool_formula fml2;
 	make_ramsey_fml(fml2,m,n,s);
 	//print_formula(fml2);
 	assignment asgt2;
 	cout << "formula made, waiting for solve_sat" << endl;
 	clock_t begin = clock();
  	assignment results2 = solve_sat(fml2, asgt2);
  	clock_t end = clock();
 	cout << "satisfiable: " << results2[0] << endl;
 	//print_assignment(results2);

 	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
 	cout << "time: " << elapsed_secs << endl;

 } //end of main



void print_formula(const bool_formula &fml){
	for(vector<int> vec: fml){
		cout << "clause: ";
		for(int i: vec) cout << i << " ";
		cout << endl;
	}
}


void print_assignment(const assignment &asgt){
	cout << "assignment thus far: " << endl;
	for(pair<int,bool> val: asgt)
		if(val.first > 0) cout << val.first << ": " 
			<< val.second << endl;
}


