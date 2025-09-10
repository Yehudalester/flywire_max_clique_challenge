#include<iostream>
#include<vector>
#include<algorithm>
#include<set>
#include<map>
#include<random>
#include<string>
#include<fstream>
using namespace std;
using ll=long long;
using vll=vector<ll>;
using pll=pair<ll,ll>;
#define SZ (1<<19)
#define INF (1e18)
vector<ll> read_nodes(string f){
    ifstream edges(f);
    string str;
    vector<ll> res;
    while(getline(edges,str)){
        ll a=0,i=0;
        while(i<str.length()){
                a*=10;
                a+=str[i]-'0';
            i++;
        }
        res.push_back(a);
    }
    return res;
    
}
int main(){
    ofstream result("res.csv");
    vector<vector<ll>> vec(12);
    for(ll i=0;i<12;i++){
        vec[i]=read_nodes("res"+to_string(i)+".csv");
        for(ll j=0;j<vec[i].size();j++){
            result << vec[i][j] << (j!=vec[i].size()-1?",":"\n");
        }
    }
    return 0;
}