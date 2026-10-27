#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;


/*
To run this code, you need to place this file inside the inputs_folder of your preference and then 
execute the following command inside that folder

g++ CollectCoins.cpp -o CollectCoins.exe && ./ColectCoins.exe
*/

namespace{
    string inputs_folder = "./inputs";
}

/* This problem is more efficiently solved with DP, here's the pure backtracking with no prune optimization for this problem
    
    Time complexity is O(2^n) and the space complexity is the same because of the stack of calls dimensions in the worst case.
    This algorithm should start slowing dow starting from n=16 

    n=size of one side of the square
*/
int coinCollecting(vector<vector<int>> & grid,int i,int j){
    if( i==grid.size()-1  && j==grid.size()-1) return grid[i][j];
 
    if( i==grid.size()-1 ) return grid[i][j] + coinCollecting(grid, i, j+1);
    if( j==grid.size()-1 ) return grid[i][j] + coinCollecting(grid, i+1, j);

    
    return max( grid[i][j] + coinCollecting(grid, i, j+1), grid[i][j] + coinCollecting(grid, i+1, j)  );
}


/*
    This is the dynamic programming solution our source of truth, to compare the results agains a proved and efficient algoritm
    with a time complexity of O(n) and space of O(n)
*/ 
int dpCoinCollecting(vector<vector<int>> & grid, vector<vector<int>> & memo, int i, int j){
    if(i < 0 || j < 0) return 0;
    if(memo[i][j]!=-1) return memo[i][j];
    memo[i][j] = grid[i][j] + max( dpCoinCollecting(grid, memo, i-1, j),  dpCoinCollecting(grid, memo, i, j-1));
    return memo[i][j];
}


/*
    Now, how do we implement the branch and bound solution? 
    Idk yet but the first clarification is that the bound is the hard part of the algoritm
    Basically we're solving a bactracking aproach with a naive aproach to save exploration and prune before. 

    Probably the following processing is interesting...
    Compute the maximum for each row and column, and do the prefixed sum for each of them so, we have this oracle of vision 
*/


/* This was the first aproach, here, what we have is that we compute the maximum sum possible per cell,
this by adding all of the right and left elements that the robot can reach. Ex

x - - -
- - - -
- - - -
- - - -

In this case we are going to compute the sum of all the elements different than x, 

Or this one...

. . x - -
. . - - -
. . - - -
. . - - -
. . - - -

We compute only the - elements, that value is stored on a n*n grid on position x

This aproach is valid because its always valid that the maximum reachable sum is less than the value produced by this method, 
but the upper bound is very loose and doesnt help us to prune the paths as kick as possible.

Most of the time, pruning taking in mind this upper bound precomputation is going to prune 1 or 2 squares apart from the final destination,
making this a valid, but not eficient algorithm as we would want
*/


/*
    For this we're going to use a structure to contenerize the info of each possible row,
    also, we're going to create the 
*/
struct PossiblePath{
    int upperBound; // The maximum possible value of a possible path is going to reach under the best possible situations
    int current_sum; // Sum until the current point of that possible solution
    int i; // indexes in the grid
    int j;
    vector<pair<int,int>> path; // Collection of the path taken so far to reach that point
};


struct comparePossiblePath{
    bool operator()(PossiblePath& pp1, PossiblePath& pp2) {
        return pp1.upperBound > pp2.upperBound; // Min-heap structure to help us with the priority queue
    }
};


  /*

  This second aproach is more tight because we only consider the best possible values per row and column,
  just one of them per row/column.
  By taking this aproach we reduce the upperbound for every position in the grid while keeping the sentence
  true, because: 
  In the best case possible, you have n steps to the right 
  and m steps down.

  If you take the best possible for each case on n+m steps, that would be your upper bound, and there's no better solution

  This aproach helps us to prune before and have a better performance overall

    grid:
  1  2  10
  3  4   5
  6  7   8

    |
    v
  row_max:
 10  10  10   Taking 10, 5, 8 is the "best scenario ", it's not always possible, but we have to be sure of the sentence being true
  5   5   5
  8   8   8

   col_max:
  6  7  10   Here, taking 6,7, and 10 to the right is the best scenario possible
  6  7   8
  6  7   8

  Below is the implementation of this upper bounds
    */

vector<vector<int>> precomputeUpperBounds(vector<vector<int>> &grid, int n){
    vector<vector<int>> row_max= grid;
    vector<vector<int>> col_max = grid;
    vector<vector<int>> upperBounds(n, vector<int> (n, 0));

    for(int i=0; i<n; i++){
        for(int j=n-2; j>=0; j--){
            row_max[i][j]=max(row_max[i][j], row_max[i][j+1]);
        }
    }

    for(int j=0; j<n; j++){
        for(int i=n-2; i>=0; i--){
            col_max[i][j] = max(col_max[i][j], col_max[i+1][j]);
        }
    }

    //  We have to invert the order, as we have the best of each column 
    // we have to sum it o n the current row they are placed
    vector<vector<int>> sumMaxRow = col_max;
    vector<vector<int>> sumMaxCol = row_max;


    for(int i=0; i<n; i++){
        for(int j=n-2; j>=0; j--){
            sumMaxRow[i][j] +=  sumMaxRow[i][j+1];
        }
    }

    for(int j=0; j<n; j++){
        for(int i=n-2; i>=0; i--){
            sumMaxCol[i][j] += sumMaxCol[i+1][j];
        }
    }

    for(int i=n-1; i>=0; i--){
        for(int j=n-1; j>=0; j--){
            int maxsumRow = (j+1 <n ) ? sumMaxRow[i][j+1]: 0;
            int maxsumCol = (i+1 <n ) ? sumMaxCol[i+1][j]: 0;

            upperBounds[i][j] = grid[i][j] + maxsumRow + maxsumCol;
        }
    }
    return upperBounds;
}


int helperCoinCollectingBB(vector<vector<int>> & grid){
    int n = grid.size();
    vector<vector<int>> upperBounds = precomputeUpperBounds(grid, n);

    // Using a priority queue reduces the number of comparisons and it allow us to always choose the best 
    // candidate so far.

    priority_queue<PossiblePath, vector<PossiblePath>, comparePossiblePath> pq;
    PossiblePath beggining;
    beggining.current_sum = 0; beggining.i = 0; beggining.j =0; beggining.upperBound=0;
    PossiblePath best_completed_path=beggining;

    pq.push(beggining);

    while(!pq.empty()){
        PossiblePath current = pq.top();
        pq.pop();

        // Update the value of the current best candidate with the grid and upper bounds precomputed values
        current.current_sum += grid[current.i][current.j];
        current.upperBound = current.current_sum + upperBounds[current.i][current.j];
        current.path.push_back(make_pair(current.i, current.j));

        if(current.i == grid.size()-1 && current.j == grid.size()-1 ){  // If we reach the final, here we compare against our current final best solution
            best_completed_path = (best_completed_path.current_sum > current.current_sum) ? best_completed_path : current;
        }
        
        if(current.upperBound < best_completed_path.current_sum) continue; // Not optimal solutions are discarded here

        if(current.j+1 <n){  // Produce the childs of this solution as they are hopefully as good as their father
            PossiblePath right = current;
            right.j +=1;
            pq.push(right);
        }

        if(current.i +1 <n){
            PossiblePath down = current; 
            down.i +=1;
            pq.push(down);
        }
    }


    // Print the current path

    unordered_map<int, unordered_set<int>> order;

    for (int k = 0; k < best_completed_path.path.size(); k++) {
       order[best_completed_path.path[k].first].insert(best_completed_path.path[k].second);
    }
    cout<<endl;
    cout<<"Path taken:"<<endl;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            auto it = order[i].find(j);

            if (it != order[i].end()) {
                cout << " "<< *it <<" ";
            } else {
                cout << " " << "." <<" ";
            }
        }
        cout << endl;
    }


    return best_completed_path.current_sum;
}

// Read the files
vector<vector<int>> readGridFile(string file_name){
    ifstream file;
    file.open(file_name);
    int n;
    file >> n;
    vector<vector<int>> grid(n,vector<int> (n));

    for(int i=0; i<n; i++){
        for(int j=0; j<n ; j++){
            file>>grid[i][j];
        }
    }
    file.close();
    return grid;
}


int main(){


    for( const auto & entry : filesystem::directory_iterator(inputs_folder)){
        string filtered_file = entry.path();
        if(filtered_file == "./CollectCoins.exe" || filtered_file=="./CollectCoins.cpp") continue;
        cout<<filtered_file<<endl;
        
        vector<vector<int>> test = readGridFile(filtered_file);
            
            /*
            
            auto inicio1 = high_resolution_clock::now();
            cout<<coinCollecting(test,0,0)<<endl;
            auto fin1 = high_resolution_clock::now();
            auto duracion1 = duration_cast<milliseconds>(fin1 - inicio1);
            cout << "Tiempo de ejecucion BACKTRACKING: " << duracion1.count() << " ms" << endl;
            */
            
            auto inicio2 = high_resolution_clock::now();
            vector<vector<int>> memo (test.size(), vector<int> (test.size(), -1));
            cout<<"Suma máxima para DP:"<<dpCoinCollecting(test,memo, test.size()-1, test.size()-1)<<endl;
            auto fin2 = high_resolution_clock::now();
            auto duracion2 = duration_cast<milliseconds>(fin2 - inicio2);
            cout << "Tiempo de ejecucion DP: " << duracion2.count() << " ms" << endl;
        
            cout<<endl;
            
            auto inicio3 = high_resolution_clock::now();
            cout<<"Suma máxima para BB:"<<helperCoinCollectingBB(test)<<endl;
            auto fin3 = high_resolution_clock::now();
            auto duracion3 = duration_cast<milliseconds>(fin3 - inicio3);
            cout << "Tiempo de ejecucion BB: " << duracion3.count() << " ms" << endl;
    }
    

    return 0;
}