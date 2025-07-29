#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <climits>
using namespace std;

class XORBasis {
private:
    int LOG_VAL;
    vector<long long> basis;
    int sz;
    bool reduced;

public:
    XORBasis(int log = 64) : LOG_VAL(log), sz(0), reduced(false) {
        basis.assign(LOG_VAL, 0);
    }

    XORBasis(const XORBasis& other) : LOG_VAL(other.LOG_VAL), sz(other.sz), reduced(other.reduced) {
        basis = other.basis;
    }

    void insert(long long x) {
        reduced = false;
        for (int i = LOG_VAL-1; i >= 0; i--) {
            if ((x >> i) & 1) {
                if (!basis[i]) {
                    basis[i] = x;
                    sz++;
                    return;
                }
                x ^= basis[i];
            }
        }
    }

    void orthogonalize() {
        if (reduced) return;
        for (int i = LOG_VAL-1; i >= 0; i--) {
            if (basis[i]) {
                for (int j = i-1; j >= 0; j--) {
                    if ((basis[i] >> j) & 1) {
                        basis[i] ^= basis[j];
                    }
                }
            }
        }
        reduced = true;
    }
  // check if it can be created
    bool represent(long long x) {
        for (int i = LOG_VAL-1; i >= 0; i--) {
            if ((x >> i) & 1) {
                if (!basis[i]) return false;
                x ^= basis[i];
            }
        }
        return true;
    }

    long long get_max() {
        long long res = 0;
        for (int i = LOG_VAL-1; i >= 0; i--) {
            if ((res ^ basis[i]) > res) {
                res ^= basis[i];
            }
        }
        return res;
    }

    long long get_min() {
        for (int i = 0; i < LOG_VAL; i++) {
            if (basis[i]) return basis[i];
        }
        return 0;
    }

    int size() {
        return sz;
    }
    // get the kth distinct xor 
    long long kth(long long k) {
        if (!reduced) orthogonalize();
        vector<long long> b;
        for (int i = 0; i < LOG_VAL; i++) {
            if (basis[i]) b.push_back(basis[i]);
        }
        if (k >= (1LL << b.size())) return -1;
        long long ans = 0;
        for (int i = 0; i < b.size(); i++) {
            if (k & (1LL << i)) {
                ans ^= b[i];
            }
        }
        return ans;
    }

    void merge(const XORBasis& other) {
        for (int i = 0; i < LOG_VAL; i++) {
            if (other.basis[i]) {
                insert(other.basis[i]);
            }
        }
    }

    vector<long long> get_basis() {
        vector<long long> res;
        for (int i = 0; i < LOG_VAL; i++) {
            if (basis[i]) res.push_back(basis[i]);
        }
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr); 
   // get the kth  smallest xors and print them 
    int n;
    long long k;
    cin >> n >> k;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    XORBasis basis(64);
    for (long long x : a) {
        basis.insert(x);
    }

    int r = basis.size();
    basis.orthogonalize();

    if (n - r >= 60) {
        for (int i = 0; i < k; i++) {
            cout << 0;
            if (i < k-1) {
                cout << ' ';
            }
        }
        cout << endl;
        return 0;
    }

    long long M = (1LL << (n - r));
    long long d = (k-1) / M + 1;

    vector<long long> basis_vectors = basis.get_basis();

    priority_queue<long long, vector<long long>, greater<long long>> pq;
    unordered_set<long long> seen;
    vector<long long> distinct;
    pq.push(0);
    seen.insert(0);

    while (!pq.empty() && (long long)distinct.size() < d) {
        long long u = pq.top();
        pq.pop();
        distinct.push_back(u);
        for (long long x : basis_vectors) {
            long long v = u ^ x;
            if (seen.find(v) == seen.end()) {
                seen.insert(v);
                pq.push(v);
            }
        }
    }

    long long total = 0;
    for (int i = 0; i < distinct.size(); i++) {
        long long times = M;
        if (i == (int)distinct.size() - 1) {
            times = k - total;
        }
        for (int j = 0; j < times; j++) {
            cout << distinct[i];
            total++;
            if (total < k) {
                cout << ' ';
            }
        }
    }
    cout << endl;

    return 0;
}
