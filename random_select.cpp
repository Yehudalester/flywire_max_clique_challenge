#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<queue>
#include<random>
#include<algorithm>
#include<set>
#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx2")
using namespace std;
using ll=long long;
map<ll,vector<ll>> g;
map<pair<ll,ll>,bool> adj_m;
map<ll,ll> deg;
vector<ll> nodes;
mt19937 rng(time(0));
inline ll f(ll n){
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
        shuffle(g[a].begin(),g[a].end(),rng);
        for(ll nei:g[a])if(!vis[nei]){vis[nei]=1;q.push(nei);}
    }
    return clique;
}
set<ll> inside;
vector<ll> nds;
bool valid(){
    ll count_edges=0;
    for(ll x:inside){
        for(ll y:inside){
            if(adj_m[{x,y}])count_edges++;
        }
    }
    return (count_edges*4>inside.size()*(inside.size()-1));
}
vector<ll> at(){
    map<ll,ll> d;
    for(ll x:inside){
        for(ll y:inside){
            if(adj_m[{x,y}])d[x]++;
        }
    }
    ll mini=1e18,ind=-1,maxi=0,ind2=-1;
    ll f=0;
    for(pair<ll,ll> p:d){
        if(p.second<mini||(rand()%2&&p.second==mini))mini=p.second,ind=p.first;
    }
    if(!valid()){inside.erase(ind);goto l;}
    for(ll node:nds){
        ll de=-1;
        for(ll x:inside){if(x==node)de=-1e18;
            if(adj_m[{node,x}])de++;}
        if(de>maxi||(rand()%2&&de==maxi))maxi=de,ind2=node;
        if(maxi>mini)break;
    }
    if(ind2==-1){f=3;goto l;}
    inside.insert(ind2);
    if(valid())goto l;
    if(maxi>mini){
        inside.erase(ind);
    }
    else{
        inside.erase(ind2);f=3;
    }
    l:
    vector<ll> res;
    if(f==3)return {};
    for(ll x:inside)res.push_back(x);
    return res;
}
vector<ll> try_improve(vector<ll> vec){
    while(!inside.empty())inside.erase(inside.begin());
    for(ll x:vec)inside.insert(x);
    ll cnt=1e6;
    while(cnt--&&at().size()){
        ;
    }
    vector<ll> res;
    for(ll x:inside)res.push_back(x);
    return res;
}
int main(){
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
    for(pair<ll,ll> p:deg){
        if(p.second*4>370)nodes.push_back(p.first);
        nds.push_back(p.first);
    }
    vector<ll> res;
    vector<ll> vect;
    ll f=0;
    srand(time(0));
    set<vector<ll>> cliques;
    ll trying=1000;
    ll tttttt=1;
    while(trying--){
        vect=attempt(nodes[rand()%nodes.size()]);
        if(vect.size()>200)cliques.insert(vect);
        if(vect.size()>res.size()){swap(res,vect);cout << res.size()<<" "<< flush;
        if(res.size()==vect.size()){
        ofstream result("result"+to_string(tttttt++)+".csv");
        for(ll x:res){
            result << x <<"\n";
        }
        }
    }
    }
    for(vector<ll> c1:cliques){
                vector<ll> veci=try_improve(c1);
                if(veci.size()>res.size()){
                    swap(res,veci);cout << res.size()<<" "<<flush;
                }
                ofstream result("result"+to_string(tttttt++)+".csv");
                for(ll x:res){
                    result << x <<"\n";
                }
            }
    return 0;
}
