#include<iostream>
#include<Vector>
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
map<ll,ll> mp,imp;//mp:nodes->[0,n), imp=mp^-1
struct min_seg{//what you think it is
    vector<ll> vec;
    min_seg(){vec.resize(2*SZ,INF);}
    void upd(ll i, ll k){//O(log(n))
        i+=SZ;
        vec[i]=k;
        while(i/=2){
            vec[i]=min(vec[2*i],vec[2*i+1]);
        }
    }
    ll get_min_ind(){//O(log(n))
        ll mini=vec[1];
        ll node=1;
        while(node<SZ){
            if(vec[2*node]==mini&&vec[2*node+1]==mini)node=(rand()%2?2*node:2*node+1);
            else if(vec[2*node]==mini)node=2*node;
            else node=2*node+1;
        }
        return node-SZ;
    }
    ll get(ll i){
        return vec[i+SZ];
    }
};
struct max_seg{//make a guess
    vector<ll> vec;
    max_seg(){vec.resize(2*SZ,0);}
    void upd(ll i, ll k){//O(log(n))
        i+=SZ;
        vec[i]=k;
        while(i/=2){
            vec[i]=max(vec[2*i],vec[2*i+1]);
        }
    }
    ll get_max_ind(){//O(log(n))
        ll maxi=vec[1];
        ll node=1;
        while(node<SZ){
            if(vec[2*node]==maxi&&vec[2*node+1]==maxi)node=(rand()%2?2*node:2*node+1);
            else if(vec[2*node]==maxi)node=2*node;
            else node=2*node+1;
        }
        return node-SZ;
    }
    ll get(ll i){
        return vec[i+SZ];
    }
};
struct graph{
    ll n;//number of nodes
    ll num_edges=0;//number of edges in clique
    vector<vector<ll>> g;//adjacency list of the graph
    set<ll> clique;//a quasi-clique
    max_seg deg_out=max_seg();//the degree to nodes in the clique for nodes outside the clique, 0 otherwise
    min_seg deg_in=min_seg();//the degree to nodes in the clique for nodes inside the clique, infinity otherwise
    graph(ll size,vector<pll> edges, ll node){//build the graph and initialize a quasi-clique
        n=size;
        g.resize(size);
        for(pll p:edges){
            g[p.first].push_back(p.second);
            g[p.second].push_back(p.first);
        }
        clique={node};
        deg_in.upd(node,0);
        for(ll nei:g[node]){
            deg_out.upd(nei,1);
        }
    }
    void erase(ll node){//erase a node form the quasi-clique, O(deg(node)*log(n))
        auto it=clique.find(node);
        if(it==clique.end())return;
        clique.erase(it);
        deg_out.upd(node,deg_in.get(node));
        deg_in.upd(node,INF);
        for(ll nei:g[node]){
            if(clique.find(nei)==clique.end())deg_out.upd(nei,deg_out.get(nei)-1);
            else deg_in.upd(nei,deg_in.get(nei)-1),num_edges--;
        }
    }
    void add(ll node){//add a node to the quasi-clique,O(deg(node)*log(n))
        auto it=clique.find(node);
        if(it!=clique.end())return;
        ll d_in=0;
        for(ll nei:g[node]){
            if(clique.find(nei)==clique.end())deg_out.upd(nei,deg_out.get(nei)+1);
            else deg_in.upd(nei,deg_in.get(nei)+1),num_edges++,d_in++;
        }
        deg_out.upd(node,0);
        deg_in.upd(node,d_in);
        clique.insert(node);
    }
    double density(){//make a luck guess
        if(clique.size()==1)return 0.0;
        return double(num_edges)/(clique.size()*(clique.size()-1)/2.0);
    }
    bool valid(){
        return density()>0.25;
    }
    ll improve(){//improve the pair (size,density), O((max_deg+min_deg)*log(n)) 
        ll max_out=deg_out.get_max_ind();
        ll min_in=deg_in.get_min_ind();
        if(deg_out.get(max_out)==0)return 0;
        if(deg_in.get(min_in)==INF)return 0;
        add(max_out);
        if(valid())return 1;
        erase(max_out);
        if(deg_out.get(max_out)>deg_in.get(min_in)){
            add(max_out);erase(min_in);return 1;
        }
        return 0;
    }
    vll do_it(){
        pair<ll,double> p;
        do{
            p={clique.size(),density()};
            improve();
        }
        while(pair<ll,double>{clique.size(),density()}>p);
        vector<ll> res;
        for(ll node:clique)res.push_back(node);
        return res;
    }
};
vector<pll> read_edges(){
    ifstream edges("edges.csv");
    vector<pair<ll,ll>> res;
    ll cnt=0;
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
        if(!mp.count(a))mp[a]=cnt++,imp[mp[a]]=a;
        if(!mp.count(b))mp[b]=cnt++,imp[mp[b]]=b;
        res.push_back({mp[a],mp[b]});
    }
    return res;
}
void write_to_file(vll nodes,string file_name){
    ofstream result(file_name);
    for(ll node:nodes){
        result << imp[node] << "\n";
    }
    result << flush;
}
int main(){
    vector<pll> edges=read_edges();
    for(ll t=0;t<mp.size();t++){
        //cout << t <<" "<<flush;
        graph g=graph(mp.size(),edges,t);
        vll res=g.do_it();
        if(res.size()>350){cout << res.size()<<" "<<flush;write_to_file(res,"results/result"+to_string(res.size())+"_"+to_string(t)+".csv");}
    }
    cout << "done"<<flush;
    return 0;
}