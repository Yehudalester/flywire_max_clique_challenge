#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<queue>
#include<random>
using namespace std;
using ll=long long;
map<ll,vector<ll>> g;
map<pair<ll,ll>,bool> adj_m;
map<ll,ll> deg;
vector<ll> nodes;
int main(){
    ll best=285;
    ifstream edges("edges.csv");
    string str;
    while(getline(edges,str)){
        ll a=0,b=0,i=0,f=0;
        while(i<str.length()){
            if(str[i]==','){f=1;}
            else if(f==0){
                a*=10;
                a+=str[i]-'0';
            }
            else{
                b*=10;
                b+=str[i]-'0';
            }
            i++;
        }
        g[a].push_back(b);
        g[b].push_back(a);
        deg[a]++;deg[b]++;
        adj_m[{a,b}]=adj_m[{b,a}]=1;
    }
    ifstream ffff("result.csv");
    vector<ll> res;
    while(getline(ffff,str)){
        ll a=0;
        for(char c:str){
            a*=10;a+=c-'0';
        }
        res.push_back(a);
    }
    ll c=0;
    for(ll i=0;i<res.size();i++){
        for(ll j=i+1;j<res.size();j++){
            if(adj_m[{res[i],res[j]}])c++;
        }
    }
    cout << double(c)/(res.size()*(res.size()-1)/2);
}