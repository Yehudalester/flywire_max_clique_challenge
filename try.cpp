#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<queue>
#include<random>
#include<algorithm>
using namespace std;
using ll=long long;
map<ll,vector<ll>> g;
map<pair<ll,ll>,bool> adj_m;
map<ll,ll> deg;
vector<ll> nodes;
inline ll f(ll n){
    if(n<=4)return 2;
    if(n<=50)return 4;
    if(n<=100)return 6;
    return 8;
}
vector<ll> attempt(ll x){
    queue<ll> q;
    q.push(x);
    vector<ll> clique;
    ll edge_count=0;
    map<ll,bool> vis;
    vis[x]=1;
    while(!q.empty()){
        ll a=q.front();q.pop();
        ll e=0;
        for(ll node:clique){
            e+=adj_m[{a,node}];
        }
        if(e+edge_count<clique.size()*(clique.size()+1)/f(clique.size()))continue;
        edge_count+=e;
        clique.push_back(a);
        for(ll nei:g[a])if(!vis[nei]){vis[nei]=1;q.push(nei);}
    }
    return clique;
}
vector<ll> try_benchmark(){
    ifstream benchmark("benchmark_162.csv");
    string str;
    queue<ll> q;
    vector<ll> clique;
    map<ll,bool> vis;
    map<ll,bool> in_clique;
    ll edge_count=0;
    while(getline(benchmark,str)){
        ll a=0;
        for(char c:str){
            a*=10;
            a+=c-'0';
        }
        q.push(a);
        for(ll x:clique){
            if(adj_m[{a,x}])edge_count++;
        }
        clique.push_back(a);
        in_clique[a]=1;
        vis[a]=1;
    }
    while(!q.empty()){
        ll a=q.front();q.pop();
        if(!in_clique[a]){
        ll e=0;
        for(ll node:clique){
            e+=adj_m[{a,node}];
        }
        if(e+edge_count<clique.size()*(clique.size()+1)/f(clique.size()))continue;
        edge_count+=e;
        clique.push_back(a);
        in_clique[a]=1;
        }
        for(ll nei:g[a])if(!vis[nei]){vis[nei]=1;q.push(nei);}
    }
    return clique;
}
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
    for(pair<ll,ll> p:deg){
        nodes.push_back(p.first);
    }
    sort(nodes.begin(),nodes.end(), [&](ll a, ll b){return deg[a]<deg[b]||(deg[a]==deg[b]&&a<b);});
    vector<ll> res;
    vector<ll> vect;
    ll f=0;
    for(ll x:nodes){
        if(f==0){vect=try_benchmark();f=1;}else vect=attempt(x);
        if(vect.size()>res.size()){swap(res,vect);cout << res.size() << " ";
        if(res.size()>best){
        ofstream result("result.csv");
        for(ll x:res){
            result << x <<"\n";
        }
        }
    }
    }
    return 0;
}