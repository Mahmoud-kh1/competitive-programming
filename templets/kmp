#include "bits/stdc++.h"
using namespace std;

const int INF = 1000000000;
const int N = 100 + 5;
int nxt[N][129];
vector<int>fail(N);
void buildAutomata(string &s){
    for(int i = 1, k = 0; i < s.size(); i++){
        while(k and s[i] != s[k]) k = fail[k - 1];
        if (s[i] == s[k])k++;
        fail[i] = k;
    }
    for(int i = 0; i < s.size(); i++){
        for(char ch = 'A'; ch <= 'Z'; ch++){
            if (i and s[i] != ch) {
                nxt[i][ch] = nxt[fail[i - 1]][ch];
            }
            else {
                nxt[i][ch] = i + (ch == s[i]);
            }
        }
    }
    for(auto ch = 'A'; ch <= 'Z'; ch++){
        nxt[s.size()][ch] = nxt[fail[s.size() - 1]][ch];
    }
}
string s1, s2, pat;
int dp[N][N][N];
int solve(int i,int j, int k){
    if (k == pat.size()) return -1e9;
    if (i == s1.size() || j == s2.size()) return 0;
    int &ret = dp[i][j][k];
    if (~ret) return ret;
    ret = max(
            solve(i , j + 1, k),
            solve(i + 1, j ,k)
            );
    if (s1[i] == s2[j]){
        int new_k  = nxt[k][s1[i]];
        ret = max(ret, solve(i + 1, j + 1, new_k) + 1);
    }
    return ret;

}
void print(int i,int j, int k){
    if (i == s1.size() || j == s2.size()) return;
    int ret = solve(i , j , k);
    if (ret == solve(i , j + 1, k)){
        print(i , j + 1, k);
    }
    else if (ret == solve(i + 1, j, k)){
        print(i + 1, j , k);
    }
    else {
        cout << s1[i];
        print(i + 1, j + 1, nxt[k][s1[i]]);
    }
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
     cin >> s1 >> s2 >> pat;
    memset(dp, -1 , sizeof dp);
    buildAutomata(pat);
    if (solve(0, 0, 0)==0) cout << 0;
    else
    print(0, 0, 0);









    return 0;
}



signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
     int n; cin >> n;
     int x, y; cin >> x;
     string s = "";
     for(int i = 1; i < n; i++){
          cin >> y;
         if (y > x) s+= 'U';
         else if (x == y) s+= 'C';
         else s += 'D';
         x = y;
     }
     reverse(s.begin(), s.end());
     n = s.size();
     vector<int>fail(n,0);
     long long count[n + 1]{};
     for(int i =1 , k = 0; i  < n; i++){
         while(k and s[i] != s[k]) k = fail[k - 1];
         if (s[i] == s[k]) k++;
         fail[i] = k;
         count[k]++;
     }

     for(int sz = n - 1; sz > 0 ; sz--) count[fail[sz - 1]] += count[sz];
     for(int i =  1; i <= n; i++) count[i] ++;
     int q; cin >> q;
     while(q--){
         int x ; cin >> x;
         x--;
         cout << count[x] << endl;
     }
    return 0;
}
