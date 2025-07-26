#include "bits/stdc++.h"
using namespace std;
#define arrRange(t, a , mn, mx) t a##_[mx-mn+1], *a=(a##_)-mn;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
     int n , q; cin >> n >> q;
     int a[n + 1]{};
     for(int i = 1; i <= n; i++) cin >> a[i], a[i] += a[i - 1];
     int SQ = sqrt(n) + 2;
     arrRange(int , f, -n , n);
     arrRange(int ,s, -n , n);
     memset(f_, -1, sizeof f_);
     memset(s_, -1, sizeof s_);
     vector<array<int,3>>query[SQ + 2];
     int ans[q];
     for(int i = 0; i < q; i++){
         int l, r; cin >> l >> r;
         l--;
         if (r - l + 1 <= SQ){
             int cur = 0;
             for(int j = l; j <= r; j++){
                 if (~f[a[j]]){
                     cur = max(cur, j - f[a[j]]);
                 }
                 else{
                     f[a[j]] =  j;
                 }
             }
             ans[i] = cur;
             for(int j = l; j <= r; j++) f[a[j]] = s[a[j]] = -1;
             continue;
         }
         query[l / SQ].push_back({r, l ,i});
     }

//      0 1 2 3 2] 1 0

      for(int blk = 0; blk < SQ; blk++){

          if (query[blk].empty())    continue;
          memset(f_, -1, sizeof f_);
          memset(s_, -1, sizeof s_);
          sort(query[blk].begin(), query[blk].end());
          int L = (blk + 1) * SQ - 1, R = L, cur_ans = 0;
          f[a[L]] = s[a[L]] = L;
          for(const auto [r, l , id] : query[blk]){
              while(r > R){
                  ++R;
                  if (~f[a[R]]){
                       cur_ans= max (cur_ans, R - f[a[R]]);
                       s[a[R]] = R;
                  }
                  else{
                      f[a[R]]  = R, s[a[R]] = R;
                  }
              }
              int temp = cur_ans;
              vector<array<int,3>>updates;

              while(L > l){
                  --L;
                  updates.push_back({a[L], f[a[L]], s[a[L]]});
                  if (~f[a[L]]){
                      temp = max(temp, s[a[L]] - L);
                      f[a[L]] = L;
                  }
                  else{
                      f[a[L]] = s[a[L]] = L;
                  }
              }
              ans[id] = temp;
              reverse(updates.begin(), updates.end());
              for(const auto[v , fi, se] : updates){
                  f[v] = fi;
                  s[v] = se;
              }
              L = (blk + 1) * SQ - 1;

          }
      }
      for(auto i : ans) cout << i << endl;







    return 0;
}
