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
