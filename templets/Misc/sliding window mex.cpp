#include "bits/stdc++.h"
using namespace std;
using ll = long long;
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
 
using namespace std;
using namespace __gnu_pbds;
 
typedef tree<pair<int,int>, null_type, less<pair<int,int>>, rb_tree_tag, tree_order_statistics_node_update> pbds;
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n >> k;
 
    vector<int>a(n); vector<pair<int,int>>comb;
    for(int i = 0; i < n; i++) {
        cin >> a[i];
        comb.push_back({a[i], i});
    }
    set<int>nothave;
    multiset<int>have;
    for(int i = 0; i <= n + 1; i++) nothave.insert(i);
    auto add =[&](int i){
        have.insert(a[i]);
        if (nothave.find(a[i]) != nothave.end()) nothave.erase(a[i]);
    };
    auto remove = [&](int i){
        have.erase(have.find(a[i]));
        if (have.count(a[i]) == 0) nothave.insert(a[i]);
    };
 
    auto get = [&](){
        cout << *nothave.begin() << ' ';
    };
    for(int i = 0; i < k; i++){
        add(i);
    }
    get();
    for(int j = k;  j < n; j++){
        remove(j - k);
        add(j);
        get();
    }
 
 
 
 
    return 0;
}
