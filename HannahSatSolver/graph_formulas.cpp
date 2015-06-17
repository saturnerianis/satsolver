/*
 * Hannah Christenson
 */

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <unordered_map>

using namespace std;

typedef vector<vector<int> > bool_formula;
typedef vector<vector<int> > vec_vec;

void subset(int size,int left,vec_vec &c);
void subset_with_vector(vector<int> vec,int left,vec_vec &c);
void permutations(list<int> &nums,int num,int size,vec_vec &the_permutations);


/*
 * 
 */
void make_ramsey_fml(bool_formula & fml, int vertices, int num1, int num2){ 
    int index = 1;
    int grid[vertices][vertices]; //populate grid with how we will identify edges
    for(int i = 0; i < vertices-1; ++i){
        for(int j = i+1; j<vertices; ++j){
            grid[i][j] = index;
            index++;
        }
    }
    vector<vector<int> > c1;
    subset(vertices,num1,c1); //find the combinations of vertices of size num1

    //then add a clause for each combination, adding all of the edges between the
    //vertices in that combination
    for(vector<int> vec: c1){ 
        vector<int> clause;
        for(int i = 0; i < vec.size()-1; i++)
            for(int j = i + 1; j < vec.size(); j++)
                clause.push_back(-grid[vec[i]-1][vec[j]-1]);
        fml.push_back(clause);
    }
    //do the same for num2
    vector<vector<int> > c2;
    subset(vertices,num2,c2);
    for(vector<int> vec: c2){ 
        vector<int> clause;
        for(int i = 0; i < vec.size()-1; i++)
            for(int j = i + 1; j < vec.size(); j++)
                clause.push_back(grid[vec[i]-1][vec[j]-1]);
        fml.push_back(clause);
    }
} 


/*
 *
 */
void make_path_fml(bool_formula &fml,int vertices, int path_length1, int path_length2){
    int index = 1;
    int grid[vertices][vertices]; //populate grid with how we will identify edges
    for(int i = 0; i < vertices-1; ++i){
        for(int j = i+1; j<vertices; ++j){
            grid[i][j] = index;
            grid[j][i] = index;
            index++;
        }
    }
    //generate the vertex combinations to consider
    vector<vector<int> > combos1;
    subset(vertices,path_length1+1,combos1); 

    //for each combination, consider the permutations of the vertices
    //and add a clause representing that path to the formula
    for(vector<int> vec: combos1){
        list<int> nums;
        for(int i: vec) nums.push_back(i);
        vec_vec the_perms;
        permutations(nums,path_length1+1,path_length1+1,the_perms);
        for(vector<int> perm: the_perms){
            vector<int> clause;
            for(int i = 0; i < perm.size()-1; i++){
                clause.push_back(grid[perm[i]-1][perm[i+1]-1]);
            }
            sort(clause.begin(),clause.end());
            fml.push_back(clause);
        }
    }
    vector<vector<int> > combos2;
    subset(vertices,path_length2+1,combos2);
    for(vector<int> vec: combos2){
        list<int> nums;
        for(int i: vec) nums.push_back(i);
        vec_vec the_perms;
        permutations(nums,path_length2+1,path_length2+1,the_perms);
        for(vector<int> perm: the_perms){
            vector<int> clause;
            for(int i = 0; i < perm.size()-1; i++)
                clause.push_back(-grid[perm[i]-1][perm[i+1]-1]);
            sort(clause.begin(),clause.end());
            fml.push_back(clause);
        }
    }
    sort(fml.begin(),fml.end()); //how long does this take...? is it really better than burcu's way of doing things?
    for(int i = 0; i < fml.size()-1; i++)
        if(fml[i] == fml[i+1]){
            fml.erase(fml.begin()+i+1);
            i--;
        }
}


/*
 *
 */
void make_cycle_fml(bool_formula &fml,int vertices,int cycle_length1,int cycle_length2){
    int index = 1;
    int grid[vertices][vertices]; //populate grid with how we will identify edges
    for(int i = 0; i < vertices-1; ++i)
        for(int j = i+1; j<vertices; ++j){
            grid[i][j] = index;
            grid[j][i] = index;
            index++;
        }
    //find the combinations of the cycle size
    vector<vector<int> > combos1;
    subset(vertices,cycle_length1,combos1);
    for(vector<int> vec: combos1){
        list<int> nums;
        for(int i: vec) nums.push_back(i);
        vector<vector<int> > the_perms;
        permutations(nums,cycle_length1,cycle_length1,the_perms);

        for(vector<int> perm: the_perms){
            vector<int> clause;
            for(int i = 0; i < perm.size()-1; i++)
                clause.push_back(grid[perm[i]-1][perm[i+1]-1]);
            clause.push_back(grid[perm[perm.size()-1]-1][perm[0]-1]);
            sort(clause.begin(), clause.end());
            fml.push_back(clause);
        }
    }

    vector<vector<int> > combos2;
    subset(vertices,cycle_length2,combos2);
    for(vector<int> vec: combos2){
        list<int> nums;
        for(int i: vec) nums.push_back(i);
        vector<vector<int> > the_perms;
        permutations(nums,cycle_length2,cycle_length2,the_perms);

        for(vector<int> perm: the_perms){
            vector<int> clause;
            for(int i = 0; i < perm.size()-1; i++)
                clause.push_back(-grid[perm[i]-1][perm[i+1]-1]);
            clause.push_back(-grid[perm[perm.size()-1]-1][perm[0]-1]);
            sort(clause.begin(),clause.end());
            fml.push_back(clause);
        }
    }
    sort(fml.begin(),fml.end());
    for(int i = 0; i < fml.size()-1; i++)
        if(fml[i] == fml[i+1]){
            fml.erase(fml.begin()+i+1);
            i--;
        }
}


/*
 *
 */
void make_star_fml(bool_formula &fml,int vertices,int num1,int num2){
    int index = 1;
    int grid[vertices][vertices]; //populate grid with how we will identify edges
    for(int i = 0; i < vertices-1; ++i){
        for(int j = i+1; j<vertices; ++j){
            grid[i][j] = index;
            grid[j][i] = index;
            index++;
        }
    }
    for(int i = 0; i < vertices; i++){
        vector<int> other_v;
        for(int j = 0; j < vertices; j++) if(j!=i) other_v.push_back(j);
        vec_vec combos;
        subset_with_vector(other_v,num1-1,combos);
        for(vector<int> the_combo: combos){
            vector<int> clause;
            for(int k: the_combo) {
                clause.push_back(grid[i][k]);
            }
            fml.push_back(clause);
        }
    }
    for(int i = 0; i < vertices; i++){
        vector<int> other_v;
        for(int j = 0; j < vertices; j++) if(j!=i) other_v.push_back(j);
        vector<vector<int> > combos;
        subset_with_vector(other_v,num2-1,combos);
        for(vector<int> the_combo: combos){
            vector<int> clause;
            for(int k: the_combo) clause.push_back(-grid[i][k]);
            fml.push_back(clause);
        }
    }
}





/*
 * output for 5,3,1: 5 choose 3 stored in c, nums indexed from 1 to 5.
 * maybe switch to using default value for index of 1??
 */
void subset_helper(int size,int left,vector<int> &l,vec_vec &c,int index=1){ 
    if(left==0){
        c.push_back(l);
        return;
    }
    for(int i=index; i<=size;i++){
        l.push_back(i);
        subset_helper(size,left-1,l,c,i+1);
        l.pop_back();
    }
} 
void subset(int size,int left,vec_vec &c){
    vector<int> temp;
    subset_helper(size,left,temp,c);
}

/*
 * stores all combinations of elements of vec of size left in c
 */
void subset_with_vector_helper(vector<int> vec,int left,vector<int> &l,
    vec_vec &c,int index=0){ 

    if(left==0){
        c.push_back(l);
        return;
    }
    for(int i=index; i<vec.size();i++){
        l.push_back(vec[i]);
        subset_with_vector_helper(vec,left-1,l,c,i+1);
        l.pop_back();
    }
} 
void subset_with_vector(vector<int> vec,int left,vec_vec &c){
    vector<int> temp;
    subset_with_vector_helper(vec,left,temp,c);
}


void perm_helper(list<int> &nums,int num,int size,int left,vector<int> &temp,
        vec_vec &the_permutations){
    if(temp.size() == size){
        the_permutations.push_back(temp);
        return;
    }
    for(int i=0; i < num; i++){
        int cur = nums.front();
        temp.push_back(cur);
        nums.pop_front();
        perm_helper(nums,num-1,size,left-1,temp,the_permutations);
        temp.pop_back();
        nums.push_back(cur);
    }
}
/*
 * generates the permutations of size of nums
 */
void permutations(list<int> &nums,int num,int size,vec_vec &the_permutations){
    vector<int> temp;
    perm_helper(nums,num,size,size,temp,the_permutations);
}



