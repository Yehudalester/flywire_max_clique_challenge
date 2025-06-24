#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<queue>
#include<random>
#include<algorithm>
#include<set>
using namespace std;
using ll=long long;
map<ll,vector<ll>> g;
map<pair<ll,ll>,bool> adj_m;
map<ll,ll> deg;
vector<ll> nodes;
mt19937 rng(time(0));
set<ll> inside;
bool valid(){
    ll count_edges=0;
    for(ll x:inside){
        for(ll y:inside){
            if(adj_m[{x,y}])count_edges++;
        }
    }
    return (count_edges*4>inside.size()*(inside.size()-1));
}
vector<ll> attempt(){
    map<ll,ll> d;
    for(ll x:inside){
        for(ll y:inside){
            if(adj_m[{x,y}])d[x]++;
        }
    }
    ll mini=1e18,ind=-1,maxi=0,ind2=-1;
    for(pair<ll,ll> p:d){
        if(p.second<mini||(rand()%2&&p.second==mini))mini=p.second,ind=p.first;
    }
    for(ll node:nodes){
        ll de=-1;
        for(ll x:inside){if(x==node)de=-1e18;
            if(adj_m[{node,x}])de++;}
        if(de>maxi||(rand()%2&&de==maxi))maxi=de,ind2=node;
    }
    if(ind2==-1)goto l;
    inside.insert(ind2);
    if(valid())goto l;
    if(maxi>=mini||rand()%7){
        inside.erase(ind);
    }
    if(!valid()){
        inside.erase(ind2);
    }
    l:
    vector<ll> res;
    for(ll x:inside)res.push_back(x);
    return res;
}
int main(){
    srand(time(0));
    ll best=200;
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
    ifstream curr("result.csv");
    while(getline(curr,str)){
        ll a=0;
        for(char c:str){
            a*=10;
            a+=c-'0';
        }
        inside.insert(a);
    }
    for(pair<ll,ll> p:deg){
        nodes.push_back(p.first);
    }
    vector<ll> res;
    vector<ll> vect;
    ll f=0;
    srand(time(0));
    while(true){
        vect=attempt();
        if(vect.size()>res.size()){swap(res,vect);cout << res.size() << " ";
        if(res.size()>best){
        best=res.size();
        ofstream result("result.csv");
        for(ll x:res){
            result << x <<"\n";
        }
        }
    }
}
    return 0;
}