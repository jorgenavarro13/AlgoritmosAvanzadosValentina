#include <bits/stdc++.h>
using namespace std;

namespace {
        vector<vector<char>> warshalExample = { // Ejemplo de clase
            {'A','B', '1'}, 
            {'B','D', '1'},
            {'D','A', '1'},
            {'D','C', '1'}
        };

        vector<vector<char>> floydExample = { // Ejemplo de clase
            {'A','C', '3'},
            {'B','A', '2'},
            {'C','B', '6'},
            {'C','D', '1'},
            {'D','A', '7'}
        };

}

/**
    Time Complexity: O(n^3)
    Space complexity: O(n^2)

    Whats different from the floyd-warshall?
    The actual difference is that the floyd-warshall algorithm is used to find the shortest path between all pairs of vertices in a weighted graph,
     while the warshall algorithm is used to find the transitive closure of a directed graph.
*/
pair<vector<vector<bool>>,vector<vector<vector<char>>>> warshall(vector<vector<char>> & adjacencyList){
    // Create the matrix first
    int n=0;
    for(auto connection : adjacencyList){
        n = max(connection[0]-'A', n);
        n = max(connection[1]-'A', n);
    }
    n+=1; // Elements in the grid

    vector<vector<bool>> m (n, vector<bool> (n, false));
    for(auto connection : adjacencyList){
        int i = connection[0]-'A';
        int j = connection[1]-'A';
        bool w = (connection[2] == '1');
        m[i][j]= w;
    }

    vector<vector<vector<char>>> p (n, vector<vector<char>> (n)); 
    for(int i=0; i<n; i++) p[i][i].push_back(i+'A');

    for(int k=0; k<n; k++){
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                if(i==j) continue;
                bool previous= m[i][j];
                bool possible = (m[i][j] || m[i][k] && m[k][j]);
                if (previous==false && possible==true){
                    m[i][j]= possible;
                    p[i][j]= p[k][j];
                    p[i][j].push_back(k+'A');
                }
            }
        }
    }
    return make_pair(m,p);
}
/*
    Time Complexity: O(n^3)
    Space complexity: O(n^2)

    This one is also a dynamic programming algorithm that finds the shortest paths in a weighted graph with positive 
    or negative edge weights (but with no negative cycles).
*/
vector<vector<int>> floyd (vector<vector<char>> & adjacencyList){ 
    int n=0;
    for(auto connection : adjacencyList){
        n = max(connection[0]-'A', n);
        n = max(connection[1]-'A', n);
    }
    n+=1; // Elements in the grid

    vector<vector<int>> m (n, vector<int> (n, INT_MAX));
    for(auto connection : adjacencyList){
        int i = connection[0]-'A';
        int j = connection[1]-'A';
        int  w = connection[2]-'0';
        m[i][j]= w;
    }
    for(int i=0; i<n; i++) m[i][i]=0;

    for(int k=0; k<n; k++){
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                if(m[i][k]==INT_MAX || m[k][j]==INT_MAX) continue;
                m[i][j]= min(m[i][j],m[i][k] + m[k][j]);
            }
        }
    }
    return m;
}


int main(){
    pair<vector<vector<bool>>,vector<vector<vector<char>>>> result = warshall(warshalExample);
    vector<vector<bool>> distances = result.first;
    vector<vector<vector<char>>> predecesors = result.second;
    cout<<"------WARSHALL-------"<<endl;
    for(auto row : distances){
        for(bool element : row){
            cout<< element<<" ";
        }
        cout<<endl;
    }
    cout<<"---Warshall(predecesors)------"<<endl;
    for(auto row : predecesors){
        for(auto element : row){
            cout<<"[";
            for(char c : element) cout<<c;
            cout<<"] ";
        }
        cout<<endl;
    }

    cout<<"------FLOYD-------"<<endl;
    vector<vector<int>> resultFloyd = floyd(floydExample);
    for(auto row : resultFloyd){
        for(auto distance : row){
            if(distance == INT_MAX) {cout<<"∞"<<" ";}
            else{cout<<distance<<" ";}
        }
        cout<<endl;
    }

    return 0;
}

/*
// Sleepery sequence
// Expasy
// Includes nucleotids sequence
// Forward and reverse 
// 6 reading frame 3 ida y 3 de regreso
// Se puede considerarn los 6  reading gramse si estas muy aburrido

// Diferencia en aminoacidos
// 
*/
