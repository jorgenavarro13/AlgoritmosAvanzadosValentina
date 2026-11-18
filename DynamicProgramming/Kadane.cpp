#include <bits/stdc++.h>
using namespace std;


namespace{
    vector<int> arreglo={  1, 4, -6, 3, 4, -8, -1, 12, 14, 1, -7, 10, 2};
}

int kadane(vector<int> arreglo){
    int best=0, current =0, i=0;
    int besti=0, bestj=0;

    for(int j=0;j<arreglo.size();j++){
        current=max(current+arreglo[j],arreglo[j]);
        best=max(best,current);
        if(current == arreglo[j]) i=j;
        if(best == current){besti=i;bestj=j;}

        cout<< "Current:"<<current<<"   A[i]:"<<arreglo[j]<<"   start:"<<i<<"   end:"<<j<<"     best:"<<best<<"     best_start:"<<besti<<"     best_end:"<<bestj<<endl;
    }
    return best;
}


int main(){
    int result = kadane(arreglo);
    cout<< "---RESULTADO---"<<endl;
    cout<<result<<endl;
    return 0;
}