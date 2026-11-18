#include <bits/stdc++.h>
using namespace std;

int climbingStairs(int n, unordered_map<int,int> & memo){
    if(n==1) return 1;
    if(n==2) return 2;

    if(memo.find(n)==memo.end()){
        memo[n]=climbingStairs(n-1,memo)+climbingStairs(n-2,memo);
    }

    return memo[n];
}

int climbingStairsThree(int n, unordered_map<int,int> & memo){
    if(n==1) return 1;
    if(n==2) return 2;
    if(n==3) return 4;

    if(memo.find(n)==memo.end()){
        memo[n]=climbingStairs(n-1,memo)+climbingStairs(n-2,memo)+climbingStairs(n-3,memo);
    }

    return memo[n];
}



int main(){
    unordered_map<int,int> memo;
    cout<<climbingStairs(4,memo)<<endl;
    cout<<climbingStairs(10,memo)<<endl;
    cout<<climbingStairs(11,memo)<<endl;
    cout<<climbingStairs(30,memo)<<endl;

    cout<<climbingStairsThree(1,memo)<<endl;
    cout<<climbingStairsThree(2,memo)<<endl;
    cout<<climbingStairsThree(3,memo)<<endl;
    cout<<climbingStairsThree(4,memo)<<endl;
    cout<<climbingStairsThree(5,memo)<<endl;

    return 0;
}