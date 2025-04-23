#include <bits/stdc++.h>
using namespace std;
#define int long long
int merge (int g, int b){
    return min(g, b);
}
vector<vector<int>>sp;
void build (const vector<int>& a){
    int sz = a.size();
    sp = vector<vector<int>>(__lg(sz) + 5, vector<int>(sz));
    sp[0] = a;
    for (int i = 1; i < sp.size(); i++){
        for (int j = 0; j + (1 << i) - 1 < sz; j++){
            sp[i][j] = merge(sp[i - 1][j] , sp[i - 1][j + (1 << (i - 1))]);
        }
    }
}

int  query2(int l, int r){
    int maxL = __lg(r - l + 1);
    int res  = sp[maxL][l];
    res = merge(res, sp[maxL][r - (1 << maxL) + 1]);
    return res;
}
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("random.in", "r", stdin);
//    freopen("Output.txt", "w", stdout);
    int t = 1;
//    cin >> t;
    while(t--){
        int n; cin >> n;
        vector<int>a(n);
        for(int i = 0; i < n; i++) cin >> a[i];
        vector<int>nFmx(n, n);
        stack<int>st;
        for(int i = 0; i < n; i++){
            while (!st.empty() && a[i] >= a[st.top()]) {
                nFmx[st.top()] = i;
                st.pop();
            }
            st.push(i);
        }
        vector<int>nSmx(n, -1);
        stack<int>st2;
        vector<int>b = a;
        for(int i = n - 1; i >= 0; i--){
            while(!st2.empty() && a[i] > a[st2.top()]){
                nSmx[st2.top()] = i;
                st2.pop();
            }
            st2.push(i);
        }
        int dem = n * (n + 1)  / 2;
        long double sum = 0;
        for(int i = 0; i < n; i++){
            int L = nSmx[i];
            int R = nFmx[i];
             L++;
             R--;
            int num = (i - L + 1) * (R - i + 1);
            num *= a[i];
            sum += num;
        }
        cout << fixed << setprecision(20);
        cout << (long double) sum / (long double)dem << endl;



    }
    return 0;
} 

