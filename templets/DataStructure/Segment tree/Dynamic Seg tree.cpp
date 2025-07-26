#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// --------- Dynamic Segment Tree Template ---------
struct Node {
    Node *left = nullptr, *right = nullptr;
    ll val = 0;      // aggregate value (e.g. sum) over this segment

    Node(ll v = 0): val(v) {}
};

struct DynamicSegTree {
    ll L, R;         // supported domain [L..R]
    Node* root;

    // constructor: specify the full index range (can be negative!)
    DynamicSegTree(ll _L, ll _R): L(_L), R(_R) {
        root = new Node();
    }

    // internal point‐update: add `delta` at position `x`
    void update(Node* &node, ll l, ll r, ll x, ll delta) {
        if (!node) node = new Node();
        if (l == r) {
            node->val += delta;
            return;
        }
        ll mid = l + ((r - l) >> 1);
        if (x <= mid) update(node->left,  l,    mid, x, delta);
        else          update(node->right, mid+1, r, x, delta);
        ll s = 0;
        if (node->left)  s += node->left->val;
        if (node->right) s += node->right->val;
        node->val = s;
    }

    // public wrapper for point‐update
    // adds `delta` at position `x`
    void update(ll x, ll delta) {
        if (x < L || x > R) {
            // out of range
            return;
        }
        update(root, L, R, x, delta);
    }

    // internal range‐query: sum over [ql..qr]
    ll query(Node* node, ll l, ll r, ll ql, ll qr) {
        if (!node || qr < l || r < ql) return 0;  // identity for sum
        if (ql <= l && r <= qr) return node->val;
        ll mid = l + ((r - l) >> 1);
        return query(node->left,  l,    mid, ql, qr)
             + query(node->right, mid+1, r,   ql, qr);
    }

    // public wrapper for range‐query
    ll query(ll ql, ll qr) {
        if (qr < L || ql > R) return 0;
        ql = max(ql, L);
        qr = min(qr, R);
        return query(root, L, R, ql, qr);
    }
};
// --------------------------------------------------

// Example usage
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Suppose you need to support positions from -1e9 to +1e9
    DynamicSegTree st(-1000000000LL, +1000000000LL);

    // Example: process some operations
    // Let's do:
    //   +5 at position -7
    //   +3 at position 10
    //   query sum on [-10..0] → should see 5
    //   query sum on [0..20]  → should see 3

    st.update(-7, 5);
    st.update(10, 3);

    cout << "Sum[-10..0] = " << st.query(-10, 0) << "\n";  // outputs 5
    cout << "Sum[0..20]  = " << st.query(0, 20)  << "\n";  // outputs 3

    // You can freely update or query any x in [-1e9..1e9].
    return 0;
}
