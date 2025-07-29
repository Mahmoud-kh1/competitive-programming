#include "bits/stdc++.h"
using namespace std;

void fast() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
}


int mod = 1e9 + 7;

template<class T = int>
struct Matrix {
    array<array<int,2>, 2>arr{};
    int rows, cols;

    Matrix(int r = 2, int c = 2) {
        rows = r; cols = c;
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
                    product.arr[i][j] = (product.arr[i][j] + __int128 (arr[i][k]) * b.arr[k][j]) % mod;
                }
            }
        }
        return product;
    }
    Matrix operator+(const Matrix &other) const {
        Matrix sum(other.rows, other.cols);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                sum.arr[i][j] = (1LL * arr[i][j] + other.arr[i][j]) % mod;
        return sum;
    }

    array<int,2>& operator [](int i){
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


struct Seg {
    vector<Matrix<int>> tree;
    vector<Matrix<int>> lazy;
    int sz = 0;

    void build(vector<int>&a) {
        int n = a.size();
        int old = n;
        if (old & (old - 1)) sz = (1 << (__lg(old) + 1));
        else sz = old;

        Matrix<int> identity(2, 2);
        identity.arr[0][0] = identity.arr[1][1] = 1;
        lazy.resize(sz << 1, identity);

        tree.resize(sz << 1, Matrix<int>(1, 2));

        Matrix<int>Trans(2,2);
        Trans[0][0] = Trans[0][1] = Trans[1][0] = 1;
        Matrix<int> base(2, 2);

        base[0][0] = 1, base[0][1] = 0;
        for (int i = 0; i < old; i++) {
            tree[i + sz] = base * power(Trans, a[i] - 1);
        }
        for (int node = sz - 1; node >= 1; node--) {
            tree[node] = tree[node << 1] + tree[node << 1 | 1];
        }

    }


        void propagate(int l, int r, int k) {
                if (lazy[k][0][0] == 1 && lazy[k][0][1] == 0 &&
                   lazy[k][1][1] == 1 && lazy[k][1][0] == 0
                )
                    return;

                if (l != r){
                    lazy[k << 1] = lazy[k << 1] * lazy[k];
                    lazy[k << 1 | 1] = lazy[k << 1| 1] * lazy[k];
                }
                tree[k] = tree[k] * lazy[k];
                for(int i = 0; i <2; i++)for(int j = 0; j < 2; j++) lazy[k][i][j] = 0;
                lazy[k][0][0] = 1;
                lazy[k][1][1] = 1;
          }

    void update(int lx, int rx, int l, int r, int k,  Matrix<int>shift) {
        propagate(l, r , k);
        if (l > rx || r < lx) {
            return;
        }
        if  (lx <= l && rx >= r){
            lazy[k] =  lazy[k] * shift;
            propagate(l, r, k);
            return;
        }
        int mid = (l + r) / 2;
        update(lx, rx , l , mid, k << 1 , shift);
        update(lx, rx , mid + 1, r, k << 1 | 1, shift);
        tree[k] = tree[k << 1] +  tree[k << 1 | 1];
    }

    Matrix<int> query(int lx, int rx, int l, int r, int k) {
        propagate(l, r, k);
        if (l > rx || r < lx){
            return Matrix<int>(2, 2);
        }
        if (lx <= l && rx >= r){
            return tree[k];
        }
        int mid = (l + r) / 2;
        return query(lx, rx, l, mid, k << 1)  + query(lx, rx, mid + 1, r, k << 1 | 1);
    }

    Matrix<int> query(int l, int r) {
        return query(l, r, 0, sz - 1, 1);
    }

    void update(int l, int r, Matrix<int> val) {
        update(l, r, 0, sz - 1, 1, val);
    }
};
signed main() {
    fast();
    int t = 1;
    // cin >> t;
    while (t--) {
        int n, q; cin >> n >> q;
        vector<int>a(n);
        for(int i = 0; i < n; i++){
            cin >> a[i];
        }
        Seg seg;
        seg.build(a);
        Matrix<int> Trans(2,2);
        Trans.arr[0][0] = 1;
        Trans.arr[0][1] = 1;
        Trans.arr[1][0] = 1;
        Trans.arr[1][1] = 0;
        while(q--){
            int ty; cin >> ty;
            if (ty == 1){
                int l,r,x; cin >> l >> r >> x;
                l--,r--;
                Matrix<int> shift = power(Trans, x);
                seg.update(l, r, shift);
            }
            else {
                int l,r;cin >> l >> r;
                l--,r--;
                cout << seg.query(l,r)[0][0] << endl;
            }
        }

    }
    return 0;
}
