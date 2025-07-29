#include <bits/stdc++.h>
using namespace std;

const int mod = 1e9 + 7;
#define int long long
template<class T = int>
struct Matrix {
    vector<vector<T>> arr;
    int rows, cols;

    Matrix(int r, int c) {
        rows = r; cols = c;
        arr.resize(rows, vector<T>(cols, 0));
    }
    void makeIdentity(){
        assert(cols == rows);
        for(int i = 0; i < rows; i++) arr[i][i] = 1;
    }
    Matrix operator *(const Matrix& b) {
        Matrix product(rows, b.cols);
        assert(cols == b.rows);
        for (int i = 0; i < rows; i++) {
            for (int k = 0; k < cols; k++) {
                if (arr[i][k] == 0) continue;
                for (int j = 0; j < b.cols; j++) {
                    if (b.arr[k][j] == 0) continue;
                    product.arr[i][j] += (__int128)(arr[i][k]) * b.arr[k][j] % mod;
                    if(product.arr[i][j] >= mod) product.arr[i][j]-=mod;
                }
            }
        }
        return product;
    }
    vector<T>& operator [](int i){
        return arr[i];
    }
};

template<class T = int>
Matrix<T> power(Matrix<T> a, int b) {
    Matrix<T> res(a.rows, a.cols);
    for(int i = 0;  i < a.rows; i++) res[i][i] = 1;
    while (b > 0) {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}
const int M = 400;
vector<int>primes(M, 1);
vector<pair<long long,int>> factor(long long k) {
    vector<pair<long long,int>> fac;
    for(long long p=2; p*p<=k; p++){
        if(k%p==0){
            int e=0;
            while(k%p==0){ k/=p; e++; }
            fac.emplace_back(p,e);
        }
    }
    if(k>1) fac.emplace_back(k,1);
    return fac;
}
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t; cin >> t;
    while(t--){
        int n, k; cin >> n >> k;
        vector<pair<int,int>>fac = factor(k);
        int ans = 1; 
        // we solve for each prime how many ways we can make that every two adj the max of power is the in lcm 
        // if we solve it dp it will be like dp[idx][bool of last if we put it as max or not] 
        // dp[i][1] = dp[i - 1][0] + dp[i - 1][1] , dp[i][0] = dp[i - 1][1] * power of that prime we solve for 
        for(auto [p, e] : fac){
            Matrix<int>T(2, 2);
            T[0][1] = 1;
            T[1][0] = e;
            T[1][1] = 1;
            Matrix<int>base(1, 2);
            base[0][0] = e;
            base[0][1] = 1;
            T = power(T, n - 1);
            T = base * T;
            ans = (ans * (T[0][0] + T[0][1])) %mod;
        }
        cout << ans << endl;
    }









    return 0;
}
