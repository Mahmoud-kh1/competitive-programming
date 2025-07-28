#include "bits/stdc++.h"
using namespace std;
#define int long long
struct Node{
    int ch[2]{};
    int sz = 0;
    int isEnd = 0;
    int &operator[](int x){
        return ch[x];
    }
};
int M = 50;
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
            int xbit = (x >> i) & 1;
            int kBit = (k >> i) & 1;
            if (xbit == 0 && kBit == 0) ans += trie[trie[cur][1]].sz;
            if (xbit == 1 && kBit == 0) ans += trie[trie[cur][0]].sz;
            if (trie[cur][xbit ^ kBit]){
                cur = trie[cur][xbit ^ kBit];
            }
            else break;


        }
        return ans + trie[cur].isEnd;
    }
    // count x^a[i] <= k
    long long get2(int x, int k){
        int cur = 0;
        long long ans = 0;
        for(int i = M - 1; i >= 0; i--){
            int xbit = (x >> i) & 1;
            int kBit = (k >> i) & 1;
            if (xbit == 0 && kBit == 1) ans += trie[trie[cur][0]].sz;
            if (xbit == 1 && kBit == 1) ans += trie[trie[cur][1]].sz;
            if (trie[cur][xbit ^ kBit]){
                cur = trie[cur][xbit ^ kBit];
            }
            else break;


        }
        return ans + trie[cur].isEnd;
    }
    // get max xor with (x)
    long long get3(int x) {
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
    int t = 1; cin >> t;
    while(t--) {
        int n, k;
        cin >> n >> k;
        int a[n];
        for (int i = 0; i < n; i++) cin >> a[i];
        Trie trie;


        auto can = [&](int mid) {
            int cntPairs = 0;
            trie.init();
            int l = 0, r = 0, res = 0;
            while (r < n) {
                cntPairs += trie.get2(a[r], mid);
                trie.update(a[r], 1);
                while (cntPairs > 0) {
                    trie.update(a[l], -1);
                    cntPairs -= trie.get2(a[l], mid);
                    l++;
                }
                res += l;
                r++;
            }
            return res;
        };
        int L = 0, R =3e10, ans = 3e10;
        while (L <= R) {
            int mid = (L + R) / 2;
            int cnt = can(mid);
            if (cnt >= k) ans = mid, R = mid - 1;
            else L = mid + 1;
        }
        cout << ans << endl;;
    }




    return 0;
}
