#include<iostream>
#include<vector>
#include<algorithm>
#include<set>
#include<map>
#include<random>
#include<string>
#include<fstream>
#include<chrono>
#include<thread>
#include<mutex>
using namespace std;
using namespace chrono;
using ll=long long;
using vll=vector<ll>;
using pll=pair<ll,ll>;
#define SZ (1<<19)
const long long INF = (1LL<<60);
time_point<high_resolution_clock> start;
map<ll,ll> mp,imp;//mp:nodes->[0,n), imp=mp^-1
vector<ll> finalv0={40, 175, 369, 723, 1541, 3108, 6205, 11752, 20838, 35827, 60419, 97270};
vector<set<ll>> best(12);
mutex best_mtx;
mutex cout_mtx;
vector<pll> edges;
double randp(){
    thread_local mt19937 rng_thread(
        high_resolution_clock::now().time_since_epoch().count() ^ (hash<thread::id>()(this_thread::get_id()))
    );
    return std::uniform_real_distribution<double>(0.0,1.0)(rng_thread);
}
struct min_segll{
    vector<ll> vec;
    min_segll(){vec.resize(2*SZ,INF);}
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
            if(vec[2*node]==mini)node=2*node;
            else node=2*node+1;
        }
        return node-SZ;
    }
    ll get(ll i){
        return vec[i+SZ];
    }
};
struct max_segll{
    vector<ll> vec;
    max_segll(){vec.resize(2*SZ,0);}
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
            if(vec[2*node+1]==maxi)node=2*node+1;
            else node=2*node;
        }
        return node-SZ;
    }
    ll get(ll i){
        return vec[i+SZ];
    }
};
struct graph0{
    ll n;//number of nodes
    ll num_edges=0;//number of edges in clique
    double delta;
    vector<vector<ll>> g;//adjacency list of the graph
    set<ll> clique;//a quasi-clique
    max_segll deg_out=max_segll();//the degree to nodes in the clique for nodes outside the clique, 0 otherwise
    min_segll deg_in=min_segll();//the degree to nodes in the clique for nodes inside the clique, infinity otherwise
    graph0(ll size,vector<pll> edges, ll node,double d){//build the graph and initialize a quasi-clique
        n=size;
        delta=d;
        g.resize(size);
        for(pll p:edges){
            g[p.first].push_back(p.second);
            g[p.second].push_back(p.first);
        }
        if(node==-1)return;
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
        if(clique.size()==1)return 1.0;
        return double(num_edges)/(clique.size()*(clique.size()-1)/2.0);
    }
    bool valid(){
        return density()>=delta;
    }
    ll improve(){//improve the pair (size,density), O((max_deg+min_deg)*log(n)) 
        ll max_out=deg_out.get_max_ind();
        ll min_in=deg_in.get_min_ind();
        if(deg_out.get(max_out)==0)return 0;
        if(deg_in.get(min_in)==INF)return 0;
        if(deg_out.get(max_out)+num_edges>=delta*(clique.size()+1)*clique.size()/2){add(max_out);return 1;}
        erase(min_in);
        max_out=deg_out.get_max_ind();
        add(max_out);
        if(min_in==max_out)return 0;
        return valid();
    } 
};
struct min_segpll{
    vector<pll> vec;
    min_segpll(){vec.resize(2*SZ,{INF,INF});}
    void upd(ll i, pll k){//O(log(n))
        i+=SZ;
        vec[i]=k;
        while(i/=2){
            vec[i]=min(vec[2*i],vec[2*i+1]);
        }
    }
    ll get_min_ind(){//O(log(n))
        pll mini=vec[1];
        ll node=1;
        while(node<SZ){
            if(vec[2*node]==mini)node=2*node;
            else node=2*node+1;
        }
        return node-SZ;
    }
    pll get(ll i){
        return vec[i+SZ];
    }
};
struct max_segpll{
    vector<pll> vec;
    max_segpll(){vec.resize(2*SZ,{0,0});}
    void upd(ll i, pll k){//O(log(n))
        i+=SZ;
        vec[i]=k;
        while(i/=2){
            vec[i]=max(vec[2*i],vec[2*i+1]);
        }
    }
    ll get_max_ind(){//O(log(n))
        pll maxi=vec[1];
        ll node=1;
        while(node<SZ){
            if(vec[2*node+1]==maxi)node=2*node+1;
            else node=2*node;
        }
        return node-SZ;
    }
    pll get(ll i){
        return vec[i+SZ];
    }
};
struct graph1{
    ll n;//number of nodes
    ll num_edges=0;//number of edges in clique
    vector<vector<ll>> g;//adjacency list of the graph
    set<ll> clique;//a quasi-clique
    set<ll> dist1;
    double delta;
    max_segpll deg_out=max_segpll();//the degree to nodes in the clique for nodes outside the clique, 0 otherwise
    min_segpll deg_in=min_segpll();//the degree to nodes in the clique for nodes inside the clique, infinity otherwise
    graph1(ll size,vector<pll> edges, ll node,double d){//build the graph and initialize a quasi-clique
        delta=d;
        n=size;
        g.resize(size);
        for(pll p:edges){
            g[p.first].push_back(p.second);
            g[p.second].push_back(p.first);
        }
        if(node==-1)return;
        clique={node};
        deg_in.upd(node,{0,g[node].size()});
        for(ll nei:g[node]){
            dist1.insert(nei);
        }
        for(ll nei:g[node]){
            ll cnt=0;
            for(ll nei1:g[nei]){
                if(dist1.find(nei1)!=dist1.end())cnt++;
            }
            deg_out.upd(nei,{1,cnt});
        }
    }
    void erase1(ll node){
        auto it=dist1.find(node);
        if(it==dist1.end())return;
        dist1.erase(it);
        for(ll nei:g[node]){
            if(dist1.find(nei)!=dist1.end()){
                deg_out.upd(nei,{deg_out.get(nei).first,deg_out.get(nei).second-1});
            }
            if(clique.find(nei)!=clique.end()){
                deg_in.upd(nei,{deg_in.get(nei).first,deg_in.get(nei).second-1});
            }
        }
    }
    void erase(ll node){//erase a node form the quasi-clique, O(deg(node)*log(n))
        auto it=clique.find(node);
        if(it==clique.end())return;
        clique.erase(it);
        deg_out.upd(node,deg_in.get(node));
        deg_in.upd(node,{INF,INF});
        for(ll nei:g[node]){
            if(clique.find(nei)==clique.end()){
                deg_out.upd(nei,{deg_out.get(nei).first-1,deg_out.get(nei).second+1});
                if(deg_out.get(nei).first==0){
                    erase1(nei);
                }
            }
            else deg_in.upd(nei,{deg_in.get(nei).first-1,deg_in.get(nei).second+1}),num_edges--;
        }
        dist1.insert(node);
    }
    void add1(ll node){
        auto it=dist1.find(node);
        if(it!=dist1.end())return;
        for(ll nei:g[node]){
            if(dist1.find(nei)!=dist1.end()){
                deg_out.upd(nei,{deg_out.get(nei).first,deg_out.get(nei).second+1});
            }
            if(clique.find(nei)!=clique.end()){
                deg_in.upd(nei,{deg_in.get(nei).first,deg_in.get(nei).second+1});
            }
        }
        dist1.insert(node);
    }
    void add(ll node){//add a node to the quasi-clique,O(deg(node)*log(n))
        auto it=clique.find(node);
        if(it!=clique.end())return;
        ll d_in=0;
        dist1.erase(node);
        for(ll nei:g[node]){
            if(clique.find(nei)==clique.end()){
                deg_out.upd(nei,{deg_out.get(nei).first+1,deg_out.get(nei).second-1});
                add1(nei);
            }
            else deg_in.upd(nei,{deg_in.get(nei).first+1,deg_in.get(nei).second-1}),num_edges++,d_in++;
        }
        deg_out.upd(node,{0,0});
        deg_in.upd(node,{d_in,g[node].size()-d_in});
        clique.insert(node);
    }
    double density(){//make a luck guess
        if(clique.size()==1)return 1.0;
        return double(num_edges)/(clique.size()*(clique.size()-1)/2.0);
    }
    bool valid(){
        return density()>=delta;
    }
    void make_valid(){
        while(!valid()){
            ll min_in=deg_in.get_min_ind();
            erase(min_in);
        }
    }
    void random_swap(){
        double threshhold0 =randp(),threshhold1=randp();
        vector<ll> picks;
        for(ll x:clique){
            if(randp()<threshhold0)picks.push_back(x);
        }
        for(ll x:picks)erase(x);
        for(ll x=0;x<g.size();x++){
            if(randp()<threshhold1)add(x);
        }
    }
    ll improve(){//improve the pair (size,density), O((max_deg+min_deg)*log(n)) 
        ll max_out=deg_out.get_max_ind();
        ll min_in=deg_in.get_min_ind();
        if(deg_out.get(max_out)==pll{0,0})return 0;
        if(deg_in.get(min_in)==pll{INF,INF})return 0;
        if(deg_out.get(max_out).first+num_edges>=delta*(clique.size()+1)*clique.size()/2){add(max_out);return 1;}
        erase(min_in);
        max_out=deg_out.get_max_ind();
        add(max_out);
        if(min_in==max_out)return 0;
        return valid();
    }
    void init(set<ll> v){
        while(!clique.empty())erase(*clique.begin());
        for(ll x : v)add(x);
        make_valid();
    }
};
vector<pll> read_edges(){
    ifstream edges("edges.csv");
    vector<pair<ll,ll>> res;
    ll cnt=0;
    string str;
    set<ll> nodes;
    map<ll,ll> deg;
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
        nodes.insert(a);nodes.insert(b);
        deg[a]++;deg[b]++;
        res.push_back({a,b});
    }
    vector<ll> nds;
    for(ll node:nodes)nds.push_back(node);
    sort(nds.begin(),nds.end(),[&](ll a, ll b){return pll{deg[a],a}<pll{deg[b],b};});
    for(ll i=0;i<nds.size();i++)mp[nds[i]]=i,imp[i]=nds[i];
    for(pll&edge:res)edge={mp[edge.first],mp[edge.second]};
    return res;
}
void write_to_file(set<ll> nodes,string file_name){
    ofstream result(file_name);
    for(ll node:nodes){
        result << imp[node] << "\n";
    }
    result << flush;
}
void calc(ll ind){
    mt19937 rng(high_resolution_clock::now().time_since_epoch().count());
    auto dis=uniform_int_distribution<int>(0,mp.size()-1);
    vector<ll> good;
    ll time_since_last=0;
    set<ll> res;
    {
        lock_guard<mutex> lg(cout_mtx);
        cout << "thread for ind = " << ind << " started in time " << duration_cast<milliseconds>(high_resolution_clock::now()-start).count() << endl;
    }
    for(ll t=mp.size()-1;t>=0;t--){
        if(time_since_last++ > 1000 && res.size()+5>=finalv0[ind])break;
        graph0 g0=graph0(mp.size(),edges,t,1.0/(1<<ind));
        while(g0.improve());
        if(g0.clique.size()+100>=res.size())good.push_back(t);
        if(g0.clique.size()>res.size()){
            time_since_last=0;
            res=g0.clique;
            scoped_lock lock(best_mtx,cout_mtx);
            best[ind]=res;
            double score=0;
            for(ll i=0;i<=11;i++){
                score+=best[i].size()*(1.0/(1<<i));
            }
            write_to_file(res,"res"+to_string(ind)+".csv");
            cout << "thread for ind = " << ind << " found clique with size " << res.size() << " in time " << duration_cast<milliseconds>(high_resolution_clock::now()-start).count() << endl;
        }
    }
    {
        lock_guard<mutex> lg(cout_mtx);
        cout << "thread for ind = " << ind << " finished step 0 in time " << duration_cast<milliseconds>(high_resolution_clock::now()-start).count() << ". amount of good nodes: " << good.size() << endl;
    }
    for(ll t:good){
        graph1 g1=graph1(mp.size(),edges,t,1.0/(1<<ind));
        while(g1.improve())if(randp()<0.01);
        if(g1.clique.size()>res.size()){
            res=g1.clique;
            scoped_lock lock(best_mtx,cout_mtx);
            best[ind]=g1.clique;
            double score=0;
            for(ll i=0;i<=11;i++){
                score+=best[i].size()*(1.0/(1<<i));
            }
            write_to_file(res,"res"+to_string(ind)+".csv");
            cout << "thread for ind = " << ind << " found clique with size " << res.size() << " in time " << duration_cast<milliseconds>(high_resolution_clock::now()-start).count() << endl;
        }
    }
    {
        lock_guard<mutex> lg(cout_mtx);
        cout << "thread for ind = " << ind << " finished step 1 in time " << duration_cast<milliseconds>(high_resolution_clock::now()-start).count() << endl;
    }
    graph1 g2=graph1(mp.size(),edges,-1,1.0/(1<<ind));
    ll runs=5000;
    while(runs--){
        g2.init(res);
        for(ll j=0;j<5;j++){
            g2.random_swap();
            while(!g2.valid()){
                if(randp()<0.01);
                g2.erase(*g2.clique.begin());
            }
            while(g2.improve());
            if(g2.clique.size()>res.size()){
                res=g2.clique;
                scoped_lock lock(best_mtx,cout_mtx);
                best[ind]=g2.clique;
                double score=0;
                for(ll i=0;i<=11;i++){
                    score+=best[i].size()*(1.0/(1<<i));
                }
                write_to_file(res,"res"+to_string(ind)+".csv");
                cout << "thread for ind = " << ind << " found clique with size " << res.size() << " in time " << duration_cast<milliseconds>(high_resolution_clock::now()-start).count() << endl;
            }
        }
    }
    {
        lock_guard<mutex> lg(cout_mtx);
        cout << "thread for ind = " << ind << " finished in time " << duration_cast<milliseconds>(high_resolution_clock::now()-start).count() << endl;
    }
}
int main(){
    start = high_resolution_clock::now();
    edges=read_edges();
    vector<thread> threads;
    for (int i = 0; i < 12; i++) {
        threads.emplace_back(calc, i);
    }
    for (auto &t : threads) t.join();
    return 0;
}