#include <bits/stdc++.h>
using namespace std;

/*
Blog : https://codeforces.com/blog/Khalwsh
*/
#define int long long
pair<int,int> merge (pair<int,int> g, pair<int,int> b){
    if (g.first > b.first) return g;
    else return b;
}
vector<vector<pair<int,int>>>sp;
void build (const vector<int>& a){
    int sz = a.size();
    sp = vector<vector<pair<int,int>>>(__lg(sz) + 5, vector<pair<int,int>>(sz));
    for(int i = 1; i < a.size(); i++){
        sp[0][i] = {a[i], i};
    }
    for (int i = 1; i < sp.size(); i++){
        for (int j = 0; j + (1 << i) - 1 < sz; j++){
            sp[i][j] = merge(sp[i - 1][j] , sp[i - 1][j + (1 << (i - 1))]);
        }
    }
}

pair<int,int>  query(int l, int r){
    int maxL = __lg(r - l + 1);
    pair<int,int> res  = sp[maxL][l];
    res = merge(res, sp[maxL][r - (1 << maxL) + 1]);
    return res;
}
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, s, x; cin >> n >> s >> x;
        vector<int>a(n + 1);
        int pre[n + 1]{};
        for(int i= 1; i <= n; i++){
            cin >> a[i];
            pre[i] = pre[i - 1] + a[i];
        }
        build(a);
        int ans = 0;
        function<map<int,int>*(int , int)> solve = [&](int l , int r)-> map<int,int>*{
            if (l > r){
                auto *mp = new map<int,int>();
                (*mp)[pre[l - 1]] = 1;
                return mp;
            }
            auto [mx, idx] = query(l , r);
            auto *mpL = solve(l, idx - 1);
            auto *mpR = solve(idx + 1, r);
            if (mx == x){
                if (mpL->size() < mpR->size()){
                    for(auto [prL, cnt] : *mpL){
                        int want = prL + s;
                        if ((*mpR).find(want) != (*mpR).end()){
                            ans+= (*mpR)[want] * cnt;
                        }
                    }
                }
                else{
                    for(auto [prR, cnt] : *mpR){
                        int want = prR - s;
                        if ((*mpL).find(want) != (*mpL).end()){
                            ans+= (*mpL)[want] * cnt;
                        }
                    }
                }
            }

            if (mpL->size() > mpR->size()){
                for(auto [p , cnt] : *mpR){
                    (*mpL)[p]+=cnt;
                }
                delete(mpR);
                return mpL;
            }
            else{
                for(auto [p , cnt] : *mpL){
                    (*mpR)[p]+=cnt;
                }
                delete(mpL);
                return mpR;
            }
        };
        auto *xxx = solve(1, n);
        delete(xxx);
        cout  << ans << endl;
    }

    return 0;
}


#include <bits/stdc++.h>
using namespace std;
pair<int,int> merge (pair<int,int> g, pair<int,int> b){
    if (g.first > b.first) return g;
    else return b;
}
vector<vector<pair<int,int>>>sp;
void build (const vector<int>& a){
    int sz = a.size();
    sp = vector<vector<pair<int,int>>>(__lg(sz) + 5, vector<pair<int,int>>(sz));
    for(int i = 1; i < a.size(); i++){
        sp[0][i] = {a[i], i};
    }
    for (int i = 1; i < sp.size(); i++){
        for (int j = 0; j + (1 << i) - 1 < sz; j++){
            sp[i][j] = merge(sp[i - 1][j] , sp[i - 1][j + (1 << (i - 1))]);
        }
    }
}

pair<int,int>  query(int l, int r){
    int maxL = __lg(r - l + 1);
    pair<int,int> res  = sp[maxL][l];
    res = merge(res, sp[maxL][r - (1 << maxL) + 1]);
    return res;
}
struct Node{
    int ch[2]{};
    int sz = 0;
    int &operator[](int x){
        return ch[x];
    }
};
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
    void del(int x){
        int cur = 0;
        for(int i = M - 1; i >= 0; i--){
            int bit = (x >> i) & 1;
            int nxt = trie[cur][bit];
            if (trie[nxt].sz == 1) trie[cur][bit] = 0;
            trie[nxt].sz--;
            cur = nxt;
        }
    }
    int maxXorwithX(int x){
        int v = 0, ans = 0;
        for(int b = M-1; b >= 0; --b){
            int bit = (x>>b)&1;
            if (trie[v][!bit] && trie[trie[v][!bit]].sz){
                ans |= 1LL<<b;
                v = trie[v][!bit];
            } else {
                v = trie[v][bit];
            }
        }
        return ans;
    }

};
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n; cin >> n;
        vector<int>a(n + 1);
        int pre[n + 1]{};
        for(int i= 1; i <= n; i++){
            cin >> a[i];
            pre[i] = pre[i - 1] ^ a[i];
        }
        build(a);
        int ans = 0;
        function<Trie(int , int)> solve = [&](int l , int r)-> Trie{
            if(l > r) {
                Trie x; x.init();
                x.insert(pre[l - 1]);
                return x;
            }

            auto [mx, idx] = query(l , r);
            auto tL = solve(l, idx - 1);
            auto tR = solve(idx + 1, r);
            int len = r - l + 1;
            int left_size = idx - l;
            int right_size = r - idx;
            if (left_size > right_size){
                for(int j = idx; j <= r; j++){
                    ans = max(ans, tL.maxXorwithX(pre[j] ^ mx));
                }
                for(int j = idx; j <= r; j++){
                    tL.insert(pre[j]);
                }
                return tL;

            }

            for(int j = l; j <= idx; j++){
                ans = max(ans, tR.maxXorwithX(pre[j - 1]  ^ mx));
            };
            for(int j = l-1; j <= idx - 1; j++){
                tR.insert(pre[j]);
            }

            return tR;

        };
        solve(1, n);
        cout << ans << endl;

    }

    return 0;
}
