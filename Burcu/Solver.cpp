//https://en.wikipedia.org/wiki/2-satisfiability
//https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
//http://stackoverflow.com/questions/4555565/generate-all-subsets-of-size-k-from-a-set
//http://www.cs.rit.edu/~spr/ElJC/ejcram14.pdf
//Hannah
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <list>
#include <stack>
#include <ctime>
using namespace std;

void addSubset(vector<int> sub, vector<vector<int>>& subsets)
{
	subsets.push_back(sub);
}
void subset(int arr[], int size, int left, int index, vector<int> &sub, vector<vector<int>>& subsets)
{

	if (left == 0)
	{
		addSubset(sub, subsets);
		return;
	}

	for (int i = index; i < size; i++)
	{
		sub.push_back(arr[i]);
		subset(arr, size, left - 1, i + 1, sub, subsets);
		sub.pop_back();
	}
}
void generateRamseyFile(int n, int r, int b)
{
	vector<vector<int>> subsets;
	vector<int> sub;
	int** edges = new int*[n];
	for (int i = 0; i < n; i++)
		edges[i] = new int[n];

	int edgeSize = 1;
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			edges[i][j] = edgeSize;
			edgeSize++;
		}
	}

	edgeSize--;
	ofstream out("Ramsey.txt");
	out << edgeSize << endl;

	int* vertices = new int[n];
	for (int i = 0; i < n; i++)
		vertices[i] = i;


	subset(vertices, n, r, 0, sub, subsets);
	for (int i = 0; i < (int)subsets.size(); i++)
	{
		for (int j = 0; j < (int)subsets[i].size() - 1; j++)
		{
			for (int k = j + 1; k < (int)subsets[i].size(); k++)
			{
				out << -edges[subsets[i][j]][subsets[i][k]] << " ";
			}
		}
		out << endl;
	}

	sub.clear();
	subsets.clear();

	subset(vertices, n, b, 0, sub, subsets);
	for (int i = 0; i < (int)subsets.size(); i++)
	{
		for (int j = 0; j < (int)subsets[i].size() - 1; j++)
		{
			for (int k = j + 1; k < (int)subsets[i].size(); k++)
			{
				out << edges[subsets[i][j]][subsets[i][k]] << " ";
			}
		}
		out << endl;
	}

	out << 0;

	delete[]vertices;
	for (int i = 0; i < n; i++)
		delete[]edges[i];
	delete[]edges;
}
void generateRamseyPathFile(int n, int p, int in)
{
	vector<vector<int>> subsets;
	vector<int> sub;

	int** edges = new int*[n];
	for (int i = 0; i < n; i++)
		edges[i] = new int[n];

	int edgeSize = 1;
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			edges[i][j] = edgeSize;
			edgeSize++;
		}
	}
	edgeSize--;

	int* vertices = new int[n];
	for (int i = 0; i < n; i++)
		vertices[i] = i;

	subset(vertices, n, p + 1, 0, sub, subsets);
	vector<vector<int>> perms;
	vector<int> newPerm;
	perms.push_back(newPerm);
	for (int i = 0; i < (int)subsets.size(); i++)
	{
		sort(subsets[i].begin(), subsets[i].end());
		do
		{
			for (int j = 0; j < (int)subsets[i].size() - 1; j++)
			{
				if (subsets[i][j] < subsets[i][j + 1])
					perms.at(perms.size() - 1).push_back(-edges[subsets[i][j]][subsets[i][j + 1]]);
				else
					perms.at(perms.size() - 1).push_back(-edges[subsets[i][j + 1]][subsets[i][j]]);
			}
			vector<int> newPerm;
			perms.push_back(newPerm);
		} while (next_permutation(subsets[i].begin(), subsets[i].end()));
	}

	for (int i = 0; i < (int)perms.size(); i++)
	{
		reverse(perms[i].begin(), perms[i].end());
		for (int j = i + 1; j < (int)perms.size(); j++)
		{
			if (perms[i] == perms[j])
			{
				perms.erase(perms.begin() + j);
				break;
			}
		}
	}

	ofstream out("Ramsey.txt");
	out << edgeSize << endl;
	for (int i = 0; i < (int)perms.size(); i++)
	{
		for (int j = 0; j < (int)perms[i].size(); j++)
		{
			out << perms[i][j] << " ";
		}
		out << endl;
	}

	sub.clear();
	subsets.clear();
	perms.clear();
	newPerm.clear();
	subset(vertices, n, in + 1, 0, sub, subsets);
	perms.push_back(newPerm);
	for (int i = 0; i < (int)subsets.size(); i++)
	{
		sort(subsets[i].begin(), subsets[i].end());
		do
		{
			for (int j = 0; j < (int)subsets[i].size() - 1; j++)
			{
				if (subsets[i][j] < subsets[i][j + 1])
					perms.at(perms.size() - 1).push_back(edges[subsets[i][j]][subsets[i][j + 1]]);
				else
					perms.at(perms.size() - 1).push_back(edges[subsets[i][j + 1]][subsets[i][j]]);
			}
			vector<int> newPerm;
			perms.push_back(newPerm);
		} while (next_permutation(subsets[i].begin(), subsets[i].end()));
	}

	for (int i = 0; i < (int)perms.size(); i++)
	{
		reverse(perms[i].begin(), perms[i].end());
		for (int j = i + 1; j < (int)perms.size(); j++)
		{
			if (perms[i] == perms[j])
			{
				perms.erase(perms.begin() + j);
				break;
			}
		}
	}

	for (int i = 0; i < (int)perms.size(); i++)
	{
		for (int j = 0; j < (int)perms[i].size(); j++)
		{
			out << perms[i][j] << " ";
		}
		out << endl;
	}

	out << 0;

	delete[]vertices;
	for (int i = 0; i < n; i++)
		delete[]edges[i];
	delete[]edges;
}
void generateRamseyCycleFile(int n, int c, int in)
{
	vector<vector<int>> subsets;
	vector<int> sub;

	int** edges = new int*[n];
	for (int i = 0; i < n; i++)
		edges[i] = new int[n];

	int edgeSize = 1;
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			edges[i][j] = edgeSize;
			edgeSize++;
		}
	}
	edgeSize--;

	int* vertices = new int[n];
	for (int i = 0; i < n; i++)
		vertices[i] = i;

	ofstream out("Ramsey.txt");
	out << edgeSize << endl;

	if (c > 2)
	{
		vector<vector<int>> perms;
		vector<int> newPerm;
		subset(vertices, n, c, 0, sub, subsets);
		perms.push_back(newPerm);
		for (int i = 0; i < (int)subsets.size(); i++)
		{
			sort(subsets[i].begin(), subsets[i].end());
			do
			{
				for (int j = 0; j < (int)subsets[i].size() - 1; j++)
				{
					if (subsets[i][j] < subsets[i][j + 1])
						perms.at(perms.size() - 1).push_back(-edges[subsets[i][j]][subsets[i][j + 1]]);
					else
						perms.at(perms.size() - 1).push_back(-edges[subsets[i][j + 1]][subsets[i][j]]);
				}
				if (subsets[i][0] < subsets[i][subsets[i].size() - 1])
					perms.at(perms.size() - 1).push_back(-edges[subsets[i][0]][subsets[i][subsets[i].size() - 1]]);
				else
					perms.at(perms.size() - 1).push_back(-edges[subsets[i][subsets[i].size() - 1]][subsets[i][0]]);
				sort(perms.at(perms.size() - 1).begin(), perms.at(perms.size() - 1).end());
				vector<int> newPerm;
				perms.push_back(newPerm);
			} while (next_permutation(subsets[i].begin(), subsets[i].end()));
		}

		sort(perms.begin(), perms.end());
		for (int i = 0; i < (int)perms.size() - 1; i++)
		{
			if (perms[i] == perms[i + 1])
			{
				perms.erase(perms.begin() + i + 1);
				i--;
			}
		}

		for (int i = 0; i < (int)perms.size(); i++)
		{
			for (int j = 0; j < (int)perms[i].size(); j++)
			{
				out << perms[i][j] << " ";
			}
			out << endl;
		}
	}

	if (in > 2)
	{
		vector<vector<int>> perms;
		vector<int> newPerm;
		sub.clear();
		subsets.clear();
		subset(vertices, n, in, 0, sub, subsets);
		perms.push_back(newPerm);
		for (int i = 0; i < (int)subsets.size(); i++)
		{
			sort(subsets[i].begin(), subsets[i].end());
			do
			{
				for (int j = 0; j < (int)subsets[i].size() - 1; j++)
				{
					if (subsets[i][j] < subsets[i][j + 1])
						perms.at(perms.size() - 1).push_back(edges[subsets[i][j]][subsets[i][j + 1]]);
					else
						perms.at(perms.size() - 1).push_back(edges[subsets[i][j + 1]][subsets[i][j]]);
				}
				if (subsets[i][0] < subsets[i][subsets[i].size() - 1])
					perms.at(perms.size() - 1).push_back(edges[subsets[i][0]][subsets[i][subsets[i].size() - 1]]);
				else
					perms.at(perms.size() - 1).push_back(edges[subsets[i][subsets[i].size() - 1]][subsets[i][0]]);
				sort(perms.at(perms.size() - 1).begin(), perms.at(perms.size() - 1).end());
				vector<int> newPerm;
				perms.push_back(newPerm);
			} while (next_permutation(subsets[i].begin(), subsets[i].end()));
		}


		sort(perms.begin(), perms.end());
		for (int i = 0; i < (int)perms.size() - 1; i++)
		{
			if (perms[i] == perms[i + 1])
			{
				perms.erase(perms.begin() + i + 1);
				i--;
			}
		}

		for (int i = 0; i < (int)perms.size(); i++)
		{
			for (int j = 0; j < (int)perms[i].size(); j++)
			{
				out << perms[i][j] << " ";
			}
			out << endl;
		}
	}

	out << 0;

	delete[]vertices;
	for (int i = 0; i < n; i++)
		delete[]edges[i];
	delete[]edges;
}
void generateRamseyStarFile(int n, int c, int in)
{
	vector<vector<int>> subsets;
	vector<int> sub;

	int** edges = new int*[n];
	for (int i = 0; i < n; i++)
		edges[i] = new int[n];

	int edgeSize = 1;
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			edges[i][j] = edgeSize;
			edgeSize++;
		}
	}
	edgeSize--;

	int* vertices = new int[n];
	for (int i = 0; i < n; i++)
		vertices[i] = i;

	ofstream out("Ramsey.txt");
	out << edgeSize << endl;

	int* v = new int[n - 1];
	int k;

	for (int i = 0; i < n; i++)
	{
		k = 0;
		for (int j = 0; j < n; j++)
		{
			if (j != i)
			{
				v[k] = vertices[j];
				k++;
			}
		}
		subset(v, n - 1, c - 1, 0, sub, subsets);
		for (int t = 0; t < (int)subsets.size(); t++)
		{
			for (int s = 0; s < (int)subsets[t].size(); s++)
			{
				if (i < subsets[t][s])
					out << -edges[i][subsets[t][s]] << " ";
				else
					out << -edges[subsets[t][s]][i] << " ";
			}
			out << endl;
		}
		subsets.clear();
		sub.clear();
	}

	for (int i = 0; i < n; i++)
	{
		k = 0;
		for (int j = 0; j < n; j++)
		{
			if (j != i)
			{
				v[k] = vertices[j];
				k++;
			}
		}
		subset(v, n - 1, in - 1, 0, sub, subsets);
		for (int t = 0; t < (int)subsets.size(); t++)
		{
			for (int s = 0; s < (int)subsets[t].size(); s++)
			{

				if (i < subsets[t][s])
					out << edges[i][subsets[t][s]] << " ";
				else
					out << edges[subsets[t][s]][i] << " ";
			}
			out << endl;
		}
		subsets.clear();
		sub.clear();
	}

	out << 0;

	delete[]v;
	delete[]vertices;
	for (int i = 0; i < n; i++)
		delete[]edges[i];
	delete[]edges;
}
bool check2CNF(vector<vector<int>> current)
{
	if (current.empty())
		return false;
	for (int i = 0; i < (int)current.size(); i++)
		if (current[i].size() != 2)
			return false;
	return true;
}
void stronglyConnect2CNF(unordered_map<int, list<int>>& graph, unordered_map<int, list<int>>::iterator it, unordered_map<int, int>& index, unordered_map<int, int>&  lowlink, unordered_map<int, bool>&  onStack, unordered_map<int, int>& components, int& ind, int& compSize, stack<int>& st)
{
	index[it->first] = ind;
	lowlink[it->first] = ind;
	ind++;
	st.push(it->first);
	onStack[it->first] = true;
	for (list<int>::iterator il = it->second.begin(); il != it->second.end(); il++)
	{
		if (index[*il] == -1)
		{
			unordered_map<int, list<int>>::iterator iter = graph.find(*il);
			stronglyConnect2CNF(graph, iter, index, lowlink, onStack, components, ind, compSize, st);
			lowlink[it->first] = min(lowlink[it->first], lowlink[*il]);
		}
		else if (onStack[*il])
		{
			lowlink[it->first] = min(lowlink[it->first], index[*il]);
		}
	}
	if (lowlink[it->first] == index[it->first])
	{
		int c;
		do
		{
			c = st.top();
			st.pop();
			onStack[c] = false;
			components[c] = compSize;
		} while (c != it->first);
		compSize++;
	}
}
bool solve2CNF(vector<vector<int>> current, unordered_map<int, bool>& assignments)
{
	unordered_map<int, list<int>> graph;
	unordered_map<int, int> index;
	unordered_map<int, int> lowlink;
	unordered_map<int, bool> onStack;
	unordered_map<int, int> components;

	for (int i = 0; i < (int)current.size(); i++)
	{
		graph[-(current[i][0])].push_back(current[i][1]);
		graph[current[i][0]];
		index[current[i][0]] = -1;
		lowlink[current[i][0]] = -1;
		onStack[current[i][0]] = false;
		index[-current[i][0]] = -1;
		lowlink[-current[i][0]] = -1;
		onStack[-current[i][0]] = false;
		if (current[i][0] != current[i][1])
		{
			graph[-(current[i][1])].push_back(current[i][0]);
			graph[current[i][1]];
			lowlink[current[i][1]] = -1;
			onStack[current[i][1]] = false;
			index[-current[i][1]] = -1;
			lowlink[-current[i][1]] = -1;
			onStack[-current[i][1]] = false;
		}
	}

	stack<int> st;
	int ind = 0;
	int compSize = 0;

	for (unordered_map<int, list<int>>::iterator it = graph.begin(); it != graph.end(); it++)
	{
		if (index[it->first] == -1)
			stronglyConnect2CNF(graph, it, index, lowlink, onStack, components, ind, compSize, st);
	}

	for (unordered_map<int, int>::iterator it = components.begin(); it != components.end(); it++)
	{
		if (components[-(it->first)] == components[it->first])
		{
			return false;
		}
	}

	list<int>* componentLists = new list<int>[compSize];
	for (unordered_map<int, int>::iterator it = components.begin(); it != components.end(); it++)
	{
		componentLists[components[it->first]].push_front(it->first);
	}

	bool test;
	for (int i = 0; i < compSize; i++)
	{
		test = true;
		for (list<int>::iterator il = componentLists[i].begin(); test && il != componentLists[i].end(); il++)
		{
			if (assignments.find(-*il) != assignments.end())
				test = false;
		}
		for (int i : componentLists[i])
			assignments[i] = test;
	}

	delete[]componentLists;
	return true;
}
bool selectLiteralWeighted(vector<vector<int>>& current, int& selected)
{
	unordered_map<int, double> counts;
	for (int i = 0; i < (int)current.size(); i++)
	{
		for (int j = 0; j < (int)current[i].size(); j++)
			counts[current[i][j]] += 1 / current[i].size();
	}
	selected = counts.begin()->first;
	for (pair<int, double> count : counts)
		if (count.second > counts[selected])
			selected = count.first;
	if (counts[selected] > 0)
		return true;
	return false;
}
bool selectLiteral(vector<vector<int>>& current, int& selected)
{
	unordered_map<int, int> counts;
	for (int i = 0; i < (int)current.size(); i++)
	{
		for (int j = 0; j < (int)current[i].size(); j++)
			counts[current[i][j]]++;
	}
	selected = counts.begin()->first;
	for (pair<int, int> count : counts)
		if (count.second > counts[selected])
			selected = count.first;
	if (counts[selected] > 0)
		return true;
	return false;
}
bool reClause(int var, vector<vector<int>>& current)
{
	bool erased;
	for (int i = 0; i < (int)current.size();)
	{
		erased = false;
		for (int j = 0; !erased && j < (int)current[i].size();)
		{
			if (current[i][j] == var)
			{
				erased = true;
				current.erase(current.begin() + i);
			}
			else if (current[i][j] == -var)
			{
				if (current[i].size() == 1)
					return false;
				current[i].erase(current[i].begin() + j);
			}
			else
				j++;
		}
		if (!erased)
			i++;
	}
	return true;
}
bool setLiteral(int var, vector<vector<int>> &current, unordered_map<int, bool>& assignments)
{
	if (var < 0)
		assignments[-var] = 0;

	else
		assignments[var] = 1;
	return reClause(var, current);
}

bool solve(vector<vector<int>>& current, unordered_map<int, bool>& assignments)
{
	for (int i = 0; i < (int)current.size();)
	{
		if (current[i].size() == 1)
		{
			if (!setLiteral(current[i][0], current, assignments))
				return false;
			i = 0;
		}
		else
			i++;
	}
	if (current.empty())
		return true;

	if (check2CNF(current))
		return solve2CNF(current, assignments);

	/*for (vector<int> cur : current)
	{
	if (cur.size() == 2)
	{
	vector<vector<int>> original = current;
	unordered_map<int, bool> originalAssignments = assignments;

	if (setLiteral(cur[0], current, assignments))
	{
	if (solve(current, assignments))
	return true;
	}
	if (setLiteral(-cur[0], original, originalAssignments))
	return solve(original, originalAssignments);
	else
	return false;
	}
	}*/

	int selected;
	if (selectLiteral(current, selected))
	{
		vector<vector<int>> original = current;
		unordered_map<int, bool> originalAssignments = assignments;
		if (setLiteral(selected, current, assignments))
		{
			if (solve(current, assignments))
				return true;
		}
		if (setLiteral(-selected, original, originalAssignments))
			return solve(original, originalAssignments);
		else
			return false;
	}

	selected = 0;
	for (int i = 1; i < (int)current.size(); i++)
		if (current[i].size() < current[selected].size())
			selected = i;
	vector<vector<int>> original;
	unordered_map<int, bool> originalAssignments;
	vector<int> clause = current[selected];
	for (int i = 0; i < (int)clause.size(); i++)
	{
		original = current;
		originalAssignments = assignments;
		if (setLiteral(clause[i], current, assignments))
		{
			if (solve(current, assignments))
				return true;
		}
		if (!setLiteral(-clause[i], original, originalAssignments))
			return false;

		current = original;
		assignments = originalAssignments;
	}
	return solve(current, assignments);
}

int main()
{
	int litSize;
	generateRamseyPathFile(9, 6, 6);
	ifstream stream("Ramsey.txt");
	stream >> litSize;
	vector<vector<int>> clauses;
	unordered_map<int, bool> assignments;
	if (stream)
	{
		int var = -1;
		string clause;
		bool first;
		do
		{
			getline(stream, clause);
			stringstream sstream(clause);
			first = true;
			while (sstream >> var)
			{
				if (var == 0)
					break;
				else
				{
					if (first)
					{
						vector<int> v;
						clauses.push_back(v);
						first = false;
					}
					clauses[clauses.size() - 1].push_back(var);
				}
			}
		} while (var != 0);
	}

	cout << "-------" << endl;
	clock_t start = clock();
	bool solved = solve(clauses, assignments);
	clock_t end = clock();
	if (solved)
		cout << "Satisfiable" << endl;
	else
		cout << "Unsatisfiable" << endl;
	cout << "-------" << endl << "Time: " << (end - start) / (double)(CLOCKS_PER_SEC) << "s" << endl << "End of Program" << endl;
	return 0;
}
