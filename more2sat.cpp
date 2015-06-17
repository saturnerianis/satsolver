/*
 * Hannah Christenson 
 * 
 * Functions to solve 2sat in linear time
 *
 */

#include <stack>
#include <list>
#include <unordered_map>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

typedef unordered_map<int, list<int> > graph;
typedef vector<vector<int> > bool_formula;
typedef unordered_map<int, bool> assignment;

void print_graph(graph &the_graph);
void print_soln(const unordered_map<int,bool> &soln);


/*
 * A function which takes a vector of vectors, representing
 * a vector of clauses of length less than or equal to 2
 * and, in any clause which has only one literal, duplicates that
 * literal within the clause, so that it has a length of two
 * PRECONDITION: each sub-vector has length 1 or 2 
 * POSTCONDITION: each sub-vector has length 2, and sub-vectors
 * that originally had length one have a copy of the first entry
 * as the second entry
 */
void make_2SAT(bool_formula &fml){
	for(vector<int> vec: fml)
		if(vec.size() == 0 || vec.size() > 2)
			cout << "clause length that is not 1 or 2 encountered" << endl;
		else if(vec.size() == 1) vec.push_back(vec[0]);
}


/*
 * a function that converts a 2sat formula to both a forward and 
 * reverse graph such that, for every clause of literals 1 and 2, 
 * there is an edge from -1->2 and -2->1 in the graph
 * PRECONDITION: each clause in the formula has exactly 2 literals
 */
pair<graph, graph> formula_to_graph(const bool_formula & clause_list){
	graph forward_graph;
	graph reverse_graph;

	for(vector<int> vec: clause_list){
		int literal_1 = vec[0];
		int literal_2 = vec[1];

		//add the edges, make sure that the counterparts exist as vertices
		forward_graph[-literal_1].push_back(literal_2);
		forward_graph[-literal_2].push_back(literal_1);
		reverse_graph[literal_1].push_back(-literal_2);
		reverse_graph[literal_2].push_back(-literal_1);
		forward_graph[literal_1];
		forward_graph[literal_2];
		reverse_graph[-literal_1];
		reverse_graph[-literal_2];
	}
	return pair<graph,graph>(forward_graph, reverse_graph);
}



/*
 * A function that performs depth first search in a given directed graph
 * from a given vertex, pushing that vertex onto the stack once it has
 * been fully expanded
 * PRECONDITION: vertex is in the graph and has not yet been visited
 */
void depth_first_search_stack(unordered_map<int, bool> & visited,
	 stack<int> & st, const graph & the_graph, int vertex){

	visited[vertex] = true;
	for(int edge: the_graph.at(vertex))
		if(visited.find(edge) == visited.cend())
			depth_first_search_stack(visited, st, the_graph, edge);
	st.push(vertex);
}

/*
 * A function that performs depth-first search, noting which vertex initiated
 * search for each vertex that is "discovered," and adding all discovered 
 * vertices to a list
 * PRECONDITIONS: vertex is in the graph and has not yet been marked off as having 
 * a leader
 */
void depth_first_search_list(list<int> & the_list, const graph & the_graph, 
	int vertex, int leader, unordered_map<int, int> &leaders){

	leaders[vertex] = leader;
	for(int edge: the_graph.at(vertex))
		if(leaders.find(edge) == leaders.end()) 
			depth_first_search_list(the_list, the_graph, edge, leader, leaders);
	the_list.push_back(vertex);
}


/*
 * A function that returns a vector of the strongly connected components of a graph, 
 * represented as lists themselves, and a map from vertices to the "leaders" of their
 * scc, in a pair
 */
pair<vector<list<int> >, unordered_map<int,int> > 
	find_strongly_connected_components(const pair<graph, graph> & graphs){

	unordered_map<int, bool> visited;
	stack<int> st;
	vector<list<int> > connected_components;
	unordered_map<int, int> leaders;

	//iterate through the vertices of the graph
	for(pair<int,list<int> > vertex: graphs.first){

		//if the vertex has not been visited perform depth first search with the stack
		//which will add vertices such that strongly connected components will be 
		//found in the reverse topological order in the second pass, since we use the 
		//reverse graph
		if (visited.find(vertex.first) == visited.end()) {
			depth_first_search_stack(visited, st, graphs.second, vertex.first);
		}
	}
	//while the stack is not empty, find the connected component of the top element whose
	//scc has not yet been identified by dfs. because we find the first node of the scc's in
	//reverse topological order, we will find exactly an scc with dfs
	while(!st.empty()){
		if(leaders.find(st.top()) == leaders.end()){
			list<int> connected_component;
			depth_first_search_list(connected_component, graphs.first, 
				st.top(), st.top(), leaders);
			connected_components.push_back(connected_component);
		}
		st.pop();
	}

	return pair<vector<list<int> >, unordered_map<int,int> > 
		(connected_components, leaders);
}


/*
 * A function which returns a bool indicating whether or not the original 
 * boolean formula is satisfiable, by determining if any literal and its
 * negation are in the same strongly connected component
 *
 * PRECONDITION: the negation of each key is also represented in leaders
 */
bool check_satisfiability(const unordered_map<int, int> &leaders){
	for(pair<int,int> leader: leaders)
		if(leaders.at(leader.first) == leaders.at(-leader.first))
			return false;
	return true;
}


/*
 * A function which checks to see if any vertex in the connected component has 
 * already been assigned a value. If that is the case, assign all variables in 
 * the cc to false; otherwise, assign all variables in the cc true
 */
void solution_helper(const list<int> & cc, assignment &soln){

	bool b = true;
	for(int i: cc)
		if(soln.find(-i) != soln.cend()){
			b = false;
			break;
		}
	for(int i: cc)
		soln[i] = b;
}

/*
 * A fucntion which assigns values to the literals in such a way 
 * that the formula is 
 *
 * PRECONDITION: strongly connected components are ordered in reverse 
 * topological order 
 */
void solution(const vector<list<int> > scc, assignment &soln){
	for(list<int> cc: scc)
		solution_helper(cc, soln);
}



unordered_map<int,bool> two_sat_solver(bool_formula &fml, assignment &asgt){
	make_2SAT(fml);

	pair<vector<list<int> >, unordered_map<int,int> > results = 
		find_strongly_connected_components(formula_to_graph(fml)); //way to avoid copy constructor??

	if(!check_satisfiability(results.second)){
		unordered_map<int,bool> empty;
		return empty;
	}

	solution(results.first, asgt);
	asgt[0] = true;
	return asgt;
}


// int main() {
// 	//make a formula
// 	bool_formula clause_list;


// 	// vector<int> pair_1(2);
// 	// pair_1[0] = -1;
// 	// pair_1[1] = 2;
// 	// vector<int> pair_2(2);
// 	// pair_2[0] = -2;
// 	// pair_2[1] = 3;
// 	// vector<int> pair_3(2);
// 	// pair_3[0] = 1;
// 	// pair_3[1] = -3;
// 	// vector<int> pair_4(2);
// 	// pair_4[0] = 3;
// 	// pair_4[1] = 2;

// 	// clause_list.push_back(pair_1);
// 	// clause_list.push_back(pair_2);
// 	// clause_list.push_back(pair_3);
// 	// clause_list.push_back(pair_4);

// 	vector<int> pair_1(2);
// 	pair_1[0] = 1;
// 	pair_1[1] = 3;
// 	vector<int> pair_2(2);
// 	pair_2[0] = -2;
// 	pair_2[1] = 4;
// 	vector<int> pair_3(2);
// 	pair_3[0] = -4;
// 	pair_3[1] = 3;
// 	vector<int> pair_4(2);
// 	pair_4[0] =-3;
// 	pair_4[1] = 5;
// 	vector<int> pair_5(2);
// 	pair_5[0] = -5;
// 	pair_5[1] = -1;
// 	// vector<int> pair_6(2);
// 	// pair_6[0] = 2;
// 	// pair_6[1] = 3;
// 	vector<int> pair_7(2);
// 	pair_7[0] = -3;
// 	pair_7[1] = -5;
// 	// vector<int> pair_8(2);
// 	// pair_8[0] = -12;
// 	// pair_8[1] = -32;
// 	// vector<int> pair_9(2);
// 	// pair_9[0] = -22;
// 	// pair_9[1] = -32;

// 	clause_list.push_back(pair_1);
// 	clause_list.push_back(pair_2);
// 	clause_list.push_back(pair_3);
// 	clause_list.push_back(pair_4);
// 	clause_list.push_back(pair_5);
// 	//clause_list.push_back(pair_6);
// 	clause_list.push_back(pair_7);
// 	// clause_list.push_back(pair_8);
// 	// clause_list.push_back(pair_9);

// 	pair<graph, graph> graphs = formula_to_graph(clause_list);


// 	print_graph(graphs.first);
// 	cout << endl << "reverse graph: " << endl << endl;
// 	print_graph(graphs.second);


// 	vector<list<int> > connected_components = 
// 		find_strongly_connected_components(graphs.first, graphs.second).first;

// 	unordered_map<int, int> leaders = 
// 		find_strongly_connected_components(graphs.first, graphs.second).second;

// 	vector<list<int> >::iterator itr_cc = connected_components.begin();
//  	for(; itr_cc != connected_components.end(); ++itr_cc){

//  		cout << "connected component: " << endl;

//  		list<int>::iterator list_itr = itr_cc->begin();
//  		for(; list_itr != itr_cc->end(); ++ list_itr){
//  			cout << *list_itr << " ";
//  		}

//  		cout << endl;
//  	}

//  	cout << endl << "satisfiability: " << check_satisfiability(leaders) << endl;


//  	print_soln(solution(connected_components));


// }


void print_graph(const graph & the_graph){
	for(pair<int, list<int> > vertex: the_graph){
		cout << "vertex: " << vertex.first;
		cout << " edges to: ";
		for(int edge: vertex.second)
			cout << edge << " ";
		cout << endl;
	}



}
void print_soln(const unordered_map<int,bool> &soln){
	cout << "assignment thus far: " << endl;
	for(pair<int,bool> val: soln)
		if(val.first > 0) cout << val.first << ": " 
			<< val.second << endl;
}





