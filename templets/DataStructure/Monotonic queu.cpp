#include <bits/stdc++.h>
using namespace std;
#define int long long
#define ld long double
const int oo = 2e9;
/// max 
struct Queue {
    struct Node {
        int mx = -oo, mn = oo, val;
        Node(): val(0) {}
        Node(int x): mx(x), mn(x), val(x) {}
    };

    stack<Node> a, b;

    int size() { return a.size() + b.size(); }

    void mrg(Node &l, Node &r) {
        l.mn = min(l.mn, r.mn);
        l.mx = max(l.mx, r.mx);
    }

    void push(int val) {
        auto nd = Node(val);
        if(!a.empty()) mrg(nd, a.top());
        a.push(nd);
    }

    void move() {
        while(!a.empty()) {
            auto nd = Node(a.top().val);
            if(!b.empty()) mrg(nd, b.top());
            b.push(nd), a.pop();
        }
    }

    Node get() {
        Node res;
        if(!b.empty()) mrg(res, b.top());
        if(!a.empty()) mrg(res, a.top());
        return res;
    }

    void pop() {
        if(b.empty()) move();
        if(!b.empty()) b.pop();
    }
};

/// MIN 
const int oo = 2e9;

struct MinQueue {
    struct Node {
        int mn = oo;
        int val;
        Node() : val(0) {}
        Node(int x) : mn(x), val(x) {}
    };

    stack<Node> a, b;

    int size() const { return int(a.size() + b.size()); }

    // Merge r into l by taking minimum
    static void mrg(Node &l, const Node &r) {
        l.mn = min(l.mn, r.mn);
    }

    // Push new element at the back
    void push(int v) {
        Node nd(v);
        if (!a.empty()) mrg(nd, a.top());
        a.push(nd);
    }

    // Move all elements from a to b (reversing order)
    void move() {
        while (!a.empty()) {
            Node nd(a.top().val);
            if (!b.empty()) mrg(nd, b.top());
            b.push(nd);
            a.pop();
        }
    }

    // Pop element from the front
    void pop() {
        if (b.empty()) move();
        if (!b.empty()) b.pop();
    }

    // Get current minimum
    int get_min() const {
        int res = oo;
        if (!b.empty()) res = min(res, b.top().mn);
        if (!a.empty()) res = min(res, a.top().mn);
        return res;
    }
};

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int tt = 1;
//    cin >> tt;
    while(tt--){
        int n, m, w; cin >> n >> m >> w;
        int a[n + 1]{}, pre[n + 1]{};
        for(int i = 1; i <= n; i++){
            cin >> a[i];
            pre[i] = pre[i - 1] + a[i];
        }
        auto sum =[&](int l, int r){
            return pre[r] - pre[l - 1];
        };
        vector<vector<int>>dp(n + 2, vector<int>(3, 0));
        for(int i = 0; i <= m; i++){
            Queue mono;
            for(int j = n; j >= 1; j--){
                if(i >= 2){
                    if(j + w + 1 <= n + 1) mono.push(dp[j + w + 1][(i - 2) % 3] + pre[j + w]);
                    while(mono.size() >= w) mono.pop();
                    dp[j][i % 3] = max(dp[j][i % 3], mono.get().mx - pre[j - 1]);
                }
                dp[j][i % 3] = max(dp[j][i % 3], dp[j + 1][i % 3]);
                if(i && j + w  <= n + 1){
                   dp[j][i % 3] = max(dp[j][i % 3], dp[j + w][(i - 1) % 3] + pre[j + w - 1] - pre[j - 1]);
                }
            }
        }
        cout << dp[1][m % 3];





    }

    return 0;
}
