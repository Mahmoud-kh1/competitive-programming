
#include <bits/stdc++.h>
using namespace std;
const int mod = 998244353;
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
                    product.arr[i][j] = (product.arr[i][j] + arr[i][k] * b.arr[k][j] % mod) % mod;
                }
            }
        }
        return product;
    }
    vector<T>& operator [](int i){
        return arr[i];
    }
};
Matrix<int> trans(202, 202); 
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
struct Aho {
    static const int A = 26;
 
    struct Node {
        int nxt[A];
        int link = 0;
        int isEnd = 0;
        int near = -1; 
        vector<int>endId;
        Node() {
            memset(nxt, 0, sizeof(nxt));
        }
    };
 
    vector<Node> trie;
    vector<vector<int>> all;
    vector<vector<int>>slTree;
    vector<int>matched;
    vector<int>bfsOrder;
    
    Aho() {
        trie.reserve(5e5 + 5);
        trie.emplace_back();
    }
 
    int newNode() {
        trie.emplace_back();
        return (int)trie.size() - 1;
    }
 
    int addString(const string &s, int id) {
        int v = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (!trie[v].nxt[c])
                trie[v].nxt[c] = newNode();
 
            v = trie[v].nxt[c];
        }
        trie[v].isEnd = 1; 
        trie[v].endId.push_back(id);
        return v;
    }
 
    void build() {
        queue<int> q;
 
        for (int c = 0; c < A; c++) {
            int u = trie[0].nxt[c];
 
            if (u) {
                trie[u].link = 0;
                q.push(u);
                bfsOrder.push_back(u);
            }
        }
 
        while (!q.empty()) {
            int v = q.front();
            q.pop();
 
            int fail = trie[v].link;
 
            if (trie[fail].isEnd)
                trie[v].near = fail;
            else
                trie[v].near = trie[fail].near;
 
            for (int c = 0; c < A; c++) {
                int u = trie[v].nxt[c];
 
                if (u) {
                    trie[u].link = trie[fail].nxt[c];
                    q.push(u);
                    bfsOrder.push_back(u);
                } else {
                    trie[v].nxt[c] = trie[fail].nxt[c];
                }
            }
        }
        slTree.assign(trie.size(), {});
        for (int v = 1; v < (int)trie.size(); v++) { 
            slTree[trie[v].link].push_back(v);
        }
 
        matched.assign(trie.size(), 0);
    }
 
    int advance(int state, char c) {
        return trie[state].nxt[c - 'a'];
    }
 
    // all[u] is we are on node u what's the pattern we match ? 
    void compute_all() {
        int sz = trie.size();
        all.resize(sz);
        for (int i = 0; i < sz; i++) {
            for (int v = i; v != -1; v = trie[v].near) {
                for (auto x : trie[v].endId) {
                    all[i].push_back(x);
                }
            }
        }
    } 
    
    // you can dfs of the link tree 
    void dfs(int u) {
        for (auto v : slTree[u]) {
            if (trie[u].isEnd) trie[v].isEnd = 1;
            dfs(v);
        }
    }
    
    void scan(string &s) {
        int v = 0;
        for (auto ch : s) {
            v = advance(v, ch);
            matched[v]++;
        }
    }
    
};
 
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n >> k;
    Aho aho;
    for (int i = 0 ; i < k; i++) {
        string s; cin >> s;
        aho.addString(s, i);
    }
    aho.build();
    aho.dfs(0);
    int sz = aho.trie.size();
    Matrix<int> mat(sz, sz); 
    
    for (int u = 0; u < sz; u++) {
        if (aho.trie[u].isEnd) continue;
        for (int c = 0; c < 26; c++) {
            int nxt = aho.advance(u, ('a' + c));
            if (aho.trie[nxt].isEnd) continue;
            mat[u][nxt]++;
        }
    }
    mat = power(mat, n);
    Matrix<int>bases(1, sz);
    bases[0][0] = 1;
    bases = bases * mat;
    int sum = 0;
    for (int u = 0; u < sz; u++) {
        if (aho.trie[u].isEnd) continue;
        sum += bases[0][u];
        sum %= mod;
    }
    cout << sum << endl;
    
    
    
    
    
   
 
    return 0;
}
