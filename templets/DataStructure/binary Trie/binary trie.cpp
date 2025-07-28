#include "bits/stdc++.h"
using namespace std;
struct Node{
    int ch[2]{};
    int sz = 0;
    int isEnd = 0;
    int &operator[](int x){
        return ch[x];
    }
};
// change if needed 
int M = 31;
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

    void update(int num, int op) {
        int cur = 0;
        for(int i = M - 1; i >= 0; i--){
            int bit = (num >> i) & 1;
            if (trie[trie[cur][bit]].sz == 0) trie[cur][bit] = newNode();
            cur = trie[cur][bit];
            trie[cur].sz+= op;
        }
        trie[cur].isEnd+= op;
    }
    // get number of elements that the xor   x^a[i] >= k
    long long get1(int x, int k){
        int cur = 0;
        long long ans = 0;
        for(int i = M - 1; i >= 0; i--){
            int preBit = (x >> i) & 1;
            int kBit = (k >> i) & 1;
            if (preBit == 0 && kBit == 0) ans += trie[trie[cur][1]].sz;
            if (preBit == 1 && kBit == 0) ans += trie[trie[cur][0]].sz;
            if (trie[cur][preBit ^ kBit]){
                cur = trie[cur][preBit ^ kBit];
            }
            else break;


        }
        return ans + trie[cur].isEnd;
    }

    // get max xor with (x)
    long long get2(int x) {
        int cur = 0;
        int  ans = 0;
        for(int i = M - 1; i >= 0; i--){
            int bit = (x >> i)  & 1;
            if (trie[trie[cur][!bit]].sz){
                ans += (1ll << i);
                cur = trie[cur][!bit];
            }
            else cur = trie[cur][bit];
        }
        return  ans;
    }



};
void fast() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
}
signed  main() {
    fast();
    int n, k; cin >> n >> k;
    vector<int>a(n);
    for(int i = 0; i < n; i++) cin >> a[i];
    for(int i = 1; i < n; i++) a[i]^= a[i - 1];
    Trie trie;
    trie.update(0, 1);
    long long  final = 0;
    for(int i = 0;  i < n; i++){
        final += trie.get1(a[i], k);
        trie.update(a[i], 1);
    }
    cout << final << endl;





    return 0;
}
