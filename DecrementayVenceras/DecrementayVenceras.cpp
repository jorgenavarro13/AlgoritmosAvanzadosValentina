#include <bits/stdc++.h>
#include <tuple>
using namespace std;

namespace{
    string topologicalSortTestCaseFile="TopologicalSortTestCases.txt";    
    vector<int> arreglo = {3, 4, 5, 1 ,2 ,10, 15, 0, 20}; //Usado en quickSelect
}


#define DEGUG 
/*
    Jorge Arturo Montiel Navarro | A01278612
    Algoritmos de Decrementa y Venceras
    - TopologicalSort (Para este ejercicio utilicé el standard con números, si se quiere con letras solo es cuestión de cambiar los testcases y cambiar las estructuras para que acepten char)
    - FakeCoin (No sabemos si es mas pesada o más ligera)
    - QuickSelect(Esta versión escoge el n-esimo valor de acuerdo al arreglo, en este caso el 1er elemento sería el 0 del arreglo)

    Inclui una definición de debugeo para mayor detalle de la ejecución, habilitada en la parte superior a esta sección

    Declaro que NO use IA, de hecho, pura IN (inteligencia natural) y su asesoría
*/

/*
    Util para parsear casos de un archivo con test para TopologicalSort
    En este caso son nodos u->v representados en un stream de archivo que representan en grafo 
    Time complexity O(n) siendo n el tamaño del archivo
    Space complexity O(n) para guardar la relación de nodos
*/
vector<pair<vector<pair<int,int>>, int> > parseTestCasesTopologicalSort(){
    ifstream file;
    file.open(topologicalSortTestCaseFile);
    #ifdef DEBUG
        if(!file.is_open()){cout<<"Error opening file";}
    #endif
    int tests;
    file>>tests;
    vector<pair<vector<pair<int,int>>,int>> testcases(tests);
    for(int i=0; i<tests;i++){
        int nodes,connections,u,v;
        file>>nodes>>connections;
        testcases[i].second=nodes;
        while(connections--){
            file>>u>>v;
            testcases[i].first.push_back(make_pair(u,v));
        }
    }
    return testcases;
}

/*
    Given an array of nodes u->v representing a graph, we have to return the topological order of the graph on which the graph can be liberated
    - One node can be liberated if theres no u pointing to it, we stop when we dont have any remaining to liberate or we finished all the graph
    Time complexity O(n²) In case that we have a super dense graph with almost every node connected to all others, 
    the average complexity should be around O(n*k) on wich the k represent as the average connections between nodes
    Space complexity O(n) We store the graph as a collection
*/
vector<int> topologicalOrderNum(vector<pair<int,int>> connections,int nodes){
    unordered_map<int,unordered_set<int>> graph;
    vector<int> degrees(nodes+1,0);
    vector<int> order;
    queue<int> freeNodes;
    for(auto connection : connections){
        graph[connection.first].insert(connection.second);
        degrees[connection.second]+=1;
    }
    for(int i=1;i<=nodes;i++)if(degrees[i]==0) freeNodes.push(i);

    while(!freeNodes.empty()){
        int current = freeNodes.front();
        order.push_back(current);
        freeNodes.pop();
        auto f=graph.find(current);
        if (f == graph.end()) continue;
        unordered_set<int> connected_to=f->second;
        for(auto it=connected_to.begin();it!=connected_to.end();it++){
            degrees[*it]-=1;
            if(degrees[*it]==0){
                freeNodes.push(*it);
            }
        }
    }
    return order;
}


/*
    Used in quickSelect to arrange the elements below and above a certain value named "pivot", 
    as a result we modify in place an array, in the wich one, the left elements of such pivot are less 
    than it and the elements on the right of it are bigger

    Time Complexity: O(n) We iterate the full array on indexes left and right. N is defined as the difference between them, not the full size of the array
    Space complexity O(1) Only control variables, the vector is modified in place
*/
int lomuto(vector<int> & arreglo, int left, int right){
    int s=left;
    int pivot = arreglo[left];

    for(int i=left+1; i<=right; i++){
        if(arreglo[i]<pivot){
            s =  s+1;
            swap (arreglo[s],arreglo[i]);
        }
    }
    swap(arreglo[s],arreglo[left]);
    return s;
}

/*
    Select the n-th element of the array wich has fewer elements
    Time complexity O(n²): Worst case scenario, with a medium of O(nlogn) and the best case is O(n)
    Space complexity O(1): Only control variables for the index
*/
int quickSelect(vector<int> &arreglo, int left, int right, int k){
    int index = lomuto(arreglo, left,right);

    if(index > k) quickSelect(arreglo, left, index,k);
    if(index < k) quickSelect(arreglo, index+1,right,k);
    return arreglo[k-1];
}


/*
    Util for creating a vector with equal values, except for one and only one that
        - Can be located anywhere inside the array
        - Is different from the others
        - Can be more or less than the others

    Time Complexity O(n) The construction of the array is a O(n) operation
    Space complexity O(n) As we need to return and create an array of size n, we need to allocate such space
*/
pair<vector<int>,int> createCoinArray(){
    srand(time(nullptr));
    int lenght = rand() % 198 +3; //Numero aleatorio desde 3 hasta 200
    int weight = rand() % 11; 
    int weight_difference = (rand() % 11) + 1;
    int positive = rand() % 2;
    int position_of_different_coin = rand() % lenght;
    vector<int> coins (lenght,weight);
    coins[position_of_different_coin] += (positive == 1) ? weight_difference : -(weight_difference) ;
    return make_pair(coins,position_of_different_coin);
}

/*
    Returs wich subset is the different one, comparing the sum of each set. If neither of them is different, then it's outside the current ones
    Time complexity : O(1) Only basic operations with prefixed sum concept for sum optimization in subsets
    Space complexity : O(1) Only control variables for comparisons
*/
char measure( vector<int> &coins, vector<int> &prefixed, pair<int,int> &A, pair<int,int> &B, pair<int,int> &C){
    int B_weight=prefixed[B.second] - prefixed[B.first] + coins[B.first];
    int A_weight=prefixed[A.second] - prefixed[A.first] + coins[A.first];
    int C_weight=prefixed[C.second] - prefixed[C.first] + coins[C.first];

    #ifdef DEBUG
        cout<<"A["<<A.first<<","<<A.second<<"]="<<A_weight<<endl;
        cout<<"B["<<B.first<<","<<B.second<<"]="<<B_weight<<endl;
        cout<<"C["<<C.first<<","<<C.second<<"]="<<C_weight<<endl;

    #endif

    if( A_weight == B_weight && B_weight == C_weight) return 'D';
    if( A_weight != B_weight && A_weight != C_weight) return 'A';
    if( B_weight != A_weight && B_weight != C_weight) return 'B';
    return 'C';
}


/*
    Given an array full of coins on which we dont know the location of a different coin, we have to find the index of it
        - Constraint: We dont know wheter the coin is heavier or lighter than the average, so whe have to compare with at least 3 elements to induce a valid response
        - The minimun elements on the array are guaranteed to be at least 3
    Time complexity O(log(n)) Each time, we divide the possible subarrays by 3
    Space complexity O(logn) We accumulate just certain variables in stack, not the full arrays log n times
*/
int findFake(vector<int> & coins, vector<int> & prefixed, int left, int right){
    if(left == right) return left;
    
    #ifdef DEBUG
        cout<<"left:"<<left<<" right:"<<right<<endl;   
    #endif
    int size = right - left + 1;
    
    if(size == 2){ // If we have a subarray of 2 elements, then we have to include some other already valid element to do the full comparison
        if(left - 1 >=0 ){left--;}
        else{ right++;}
        size++;
    }

    int size_per_sub_array = size/3;

    pair<int,int> A = make_pair(left,                     left + size_per_sub_array-1);
    pair<int,int> B = make_pair(left+size_per_sub_array,  left+2*size_per_sub_array-1);
    pair<int,int> C = make_pair(left+2*size_per_sub_array,left+3*size_per_sub_array-1);
    pair<int,int> D = make_pair(left+3*size_per_sub_array, right); // Used in case the remainder of the other subsets is not exactly the full set size
    // In such case that none of A, B or C is different from each other, this one is the one that should have the different coin

    char measure_result= measure(coins, prefixed, A,B,C);
    int newleft, newright;
    switch(measure_result){
        case 'A':
            newleft = A.first;
            newright = A.second;
            break;
        case 'B':
            newleft = B.first;
            newright = B.second;
            break;
        case 'C':
            newleft = C.first;
            newright = C.second;
            break;
        case 'D':
            newleft = D.first;
            newright = D.second;
    } 

    return findFake(coins, prefixed, newleft, newright);
}


int main(){

    cout<<"\n----TOPOLOGICAL SORT----"<<endl;
    vector<pair<vector<pair<int,int>>, int> > testcases = parseTestCasesTopologicalSort();
    for(auto test : testcases){
        vector<int> order = topologicalOrderNum(test.first,test.second);
        cout<<order.size()<<endl;
        for(int n : order) cout<<n<<" ";
        cout<<endl<<(test.second==order.size() ? "ACYCLIC" : "CYCLIC")<<endl;
    }
    
    cout<<"\n----QUICK SELECT------"<<endl;
    cout<<"{";for(int n : arreglo){cout<<n<<", ";} cout<<"}"<<endl;
    cout<<quickSelect(arreglo,0,arreglo.size()-1, 3)<<endl;
    cout<<quickSelect(arreglo,0,arreglo.size()-1, 8)<<endl;    
    cout<<quickSelect(arreglo,0,arreglo.size()-1, 9)<<endl;    
    
    cout<<"\n-----FAKE COIN------"<<endl;
    for(int i=0; i<=3;i++){
        auto coins = createCoinArray();
        vector<int> prefixed = coins.first;
        for(int i=1; i<prefixed.size();i++){prefixed[i]+=prefixed[i-1];}
        cout<<"{";for(int coin : coins.first){cout<<coin<<", ";} cout<<"}"<<endl;


        int fakeCoin = findFake(coins.first, prefixed, 0,coins.first.size()-1);
        cout<<endl<<"The fake coin is on index:"<<fakeCoin<<endl;
        cout<<(fakeCoin == coins.second ? "SUCCESS: COIN FOUND" : "FAILURE: COIN NOT FOUND")<<endl;
    }
    return 0;
}

/*
    REFERENCIAS:
    https://leetcode.com/problems/course-schedule/description/?envType=problem-list-v2&envId=topological-sort
*/

/*
    En caso de que el txt de TopologicalSortTestCases  no este disponible, este es su contenido
3
5 4
1 2
1 3
3 4
2 5
4 4
1 2
2 3
3 1
3 4
6 5
1 4
2 4
3 5
4 6
5 6

*/