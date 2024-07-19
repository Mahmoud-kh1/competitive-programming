#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct XorBasis {
    int d, sz;
    vector<ll> basis;

    XorBasis(int n) {
        sz = 0;
        d = n + 1;
        basis = vector<ll>(d);
    }

    void insert(int mask) {
        for (int i = d - 1; i >= 0; i--) {
            if ((mask >> i) & 1) {
                if (!basis[i]) {
                    basis[i] = mask;
                    sz++;
                    return;
                }
                mask ^= basis[i];
            }
        }
    }

    bool check(int mask) {
        for (int i = d - 1; i >= 0; i--) {
            if ((mask >> i) & 1) {
                if (!basis[i]) {
                    return 1;
                }
                mask ^= basis[i];
            }
        }
        return 0;
    }

    ll minXor() {
        ll result = LLONG_MAX;
        for (int i = d - 1; i >= 0; i--) {
            if (basis[i] != 0 && (result ^ basis[i]) < result) {
                result ^= basis[i];
            }
        }
        return result == LLONG_MAX ? 0 : result; // If no basis vectors, return 0
    }
ll maxXor() {
        ll result = 0;
        for (int i = d - 1; i >= 0; i--) {
            if ((result ^ basis[i]) > result) {
                result ^= basis[i];
            }
        }
        return result;
    }
};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n; // Number of elements
    cin >> n;
    vector<int> elements(n);
    for (int i = 0; i < n; i++) {
        cin >> elements[i];
    }
  
    XorBasis xb(32); // Assuming 32-bit integers          or 64 if long long
    for (int x : elements) {
        xb.insert(x);
    }

    cout << "Minimum XOR subset value: " << xb.minXor() << endl;
  cout << "max" << xb.maxXor <<endl;

    return 0;
}
