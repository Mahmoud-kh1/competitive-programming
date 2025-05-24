#include "bits/stdc++.h"
#define int long long
using namespace std;
struct Node{
    int ch[2]{};
    int sz = 0;
    int &operator[](int x){
        return ch[x];
    }
};
int M = 30;
#define int long long
struct Trie{
    vector<Node>trie;

    void init(){
        trie.clear();
        trie.emplace_back();
    }
    Trie(){init();}
    int newNode(){
        trie.emplace_back();
        return (int)trie.size() -1 ;
    }

    void insert(int x){
        int cur = 0;
        for(int i = M - 1; i >= 0; i--){
            int bit = (x >> i) & 1;
            if (trie[cur][bit] == 0){
                trie[cur][bit] = newNode();
            }
            cur = trie[cur][bit];
            trie[cur].sz++;
        }
    }
    void delet(int x){
            int cur = 0;
            for(int i = M - 1; i >= 0; i--){
                int bit = (x >> i) & 1;
                int nxt = trie[cur][bit];
                if (trie[nxt].sz == 1) trie[cur][bit] = 0;
                trie[nxt].sz--;
                cur = nxt;
            }
    }
    int anss(int x){
        int cur = 0;
        int ans = 0;
        for(int i = M - 1; i >= 0; i--){
            int bit = (x >> i) & 1;
            if (trie[cur][bit]){
                cur = trie[cur][bit];
            }
            else {
                cur = trie[cur][!bit];ans+= (1ll << i);
            }
        }
        return ans;
    }

};
void fast() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}
signed  main() {
//    fast();
    int q; cin >> q;
    Trie trie;
    set<int>s;
    trie.init();
    while(q--){
        int ty, x; cin >> ty >> x;
        if (ty == 0){
            if (s.find(x) == s.end())
            trie.insert(x);
            s.insert(x);
        }
        else if (ty == 1){
            if (s.find(x) != s.end()){
                trie.delet(x);
                s.erase(x);
            }
        }
        else cout << trie.anss(x) << '\n';
    }



    return 0;
}
