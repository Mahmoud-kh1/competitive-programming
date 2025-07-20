#include "bits/stdc++.h"
using namespace std;
const int  N = 2e5 + 5;
int f[N], s[N];
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
     int n,m ,k; cin >> n >> m >> k;
     int a[n];
     for(int i = 0; i < n; i ++) cin >> a[i];
     memset(f, -1, sizeof f);
     memset(s, -1, sizeof s);
     int SQ = sqrt(n) + 2;
     vector<array<int,3>>query[SQ];
     int ans[k]{};
     for(int i = 0; i < k;  i++){
         int l, r; cin >> l >> r;
         l--, r--;
         if (r - l + 1 <= SQ){
             int cur_ans = 0;
             for(int j = l; j <= r; j++){
                 if (~f[a[j]]){
                     cur_ans = max(cur_ans, j - f[a[j]]);
                 }
                 else{
                     f[a[j]] = j;
                 }
             }
             ans[i] = cur_ans;
             for(int j = l; j <= r ; j++) f[a[j]] = s[a[j]] = -1;
             continue;
         }
         query[l / SQ].push_back({r , l , i});
     }

     for(int blk = 0; blk < SQ; blk++){
         if (query[blk].empty()) continue;
         memset(f, -1, sizeof f);
         memset(s, -1, sizeof s);
         sort(query[blk].begin(), query[blk].end());
         int LA = (blk + 1) * SQ - 1, RA = LA;
         f[a[LA]] = s[a[LA]] = LA;
         int curAns = 0;
         for(const auto [rq, lq, id] : query[blk]){
             while(rq > RA){
                 RA++;
                 if (~f[a[RA]]){
                     curAns = max(curAns, RA - f[a[RA]]);
                     s[a[RA]] = RA;
                 }
                 else{
                 f[a[RA]] = s[a[RA]] = RA;
                 }
             }
             int temp = curAns;
             vector<array<int,3>>updates;
             while(lq < LA){
                 LA--;
                 updates.push_back({a[LA], f[a[LA]], s[a[LA]]});
                 if (~f[a[LA]]){
                     temp = max(temp, s[a[LA]] - LA);
                     f[a[LA]] = LA;
                 }
                 else{
                     f[a[LA]] = s[a[LA]] = LA;
                 }
             }
             reverse(updates.begin(), updates.end());
             for(auto [v, fi, se] : updates){
                 f[v]   = fi;
                 s[v]   = se;
             }
             ans[id] = temp;
             LA = (blk + 1) * SQ - 1;
         }

     }
     for(auto i : ans) cout << i << endl;


    return 0;
}

