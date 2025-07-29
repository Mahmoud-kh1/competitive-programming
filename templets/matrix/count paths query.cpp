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
                    product.arr[i][j] = (product.arr[i][j] +  (__int128)(arr[i][k]) * b.arr[k][j]) % mod;
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
int p(int a, int b){
    int res=  1;
    while(b--){
        res *= a;
    }
    return  res;
}
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m , q; cin >> n >> m >> q;
    Matrix<int>T(n, n );
    for(int i = 0; i < m; i++){
        int u,v; cin >> u >> v;
        v--, u--;
        T[u][v]++;
    }
    vector<Matrix<int>>LG(31, Matrix<int>(n , n));
    LG[0] = T;
    for(int i = 1; i < 31; i++){
        LG[i] = LG[i - 1]  * LG[i - 1];
    }

    while(q--){
        int start, end, k; cin >> start >> end >> k;
        start--,end--;
        Matrix<int>base(1, n); 
        base[0][start] = 1;
        for(int i = 0; i < 31; i++){
            if(k >> i & 1){
                base = base * LG[i];
            }
        }
        cout << base[0][end] << endl;
    }





    return 0;
}
