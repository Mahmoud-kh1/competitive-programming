#include <bits/stdc++.h>
using namespace std;

#define int long long
void fast() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
}

const int M = 3, N = 1e6 + 7;
int bases[M], mods[M], powers[M][N];

static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Node {
    char val;
    uint64_t pri;
    int sz;
    bool rev;
    Node *l, *r;
    array<int, M> forw, back;
    Node(char v) : val(v), pri(rng()), sz(1), rev(false), l(nullptr), r(nullptr) {
        for (int i = 0; i < M; i++) {
            forw[i] = v;
            back[i] = v;
        }
    }
};

void push(Node* t) {
    if (t && t->rev) {
        t->rev = false;
        swap(t->l, t->r);
        swap(t->forw, t->back);
        if (t->l) t->l->rev = !t->l->rev;
        if (t->r) t->r->rev = !t->r->rev;
    }
}

void pull(Node* t) {
    if (!t) return;
    push(t);
    if (t->l) push(t->l);
    if (t->r) push(t->r);
    int left_sz = (t->l ? t->l->sz : 0);
    int right_sz = (t->r ? t->r->sz : 0);
    t->sz = 1 + left_sz + right_sz;
    for (int i = 0; i < M; i++) {
        if (t->l) {
            t->forw[i] = t->l->forw[i];
            t->forw[i] = (1LL * t->forw[i] * bases[i] + t->val) % mods[i];
        } else {
            t->forw[i] = t->val;
        }
        if (t->r) {
            t->forw[i] = (1LL * t->forw[i] * powers[i][right_sz] + t->r->forw[i]) % mods[i];
        } else {
            t->forw[i] = (1LL * t->forw[i] * powers[i][right_sz]) % mods[i];
        }

        if (t->r) {
            t->back[i] = t->r->back[i];
            t->back[i] = (1LL * t->back[i] * bases[i] + t->val) % mods[i];
        } else {
            t->back[i] = t->val;
        }
        if (t->l) {
            t->back[i] = (1LL * t->back[i] * powers[i][left_sz] + t->l->back[i]) % mods[i];
        } else {
            t->back[i] = (1LL * t->back[i] * powers[i][left_sz]) % mods[i];
        }
    }
}

pair<Node*, Node*> split(Node* t, int k) {
    if (!t) return {nullptr, nullptr};
    push(t);
    int left_sz = (t->l ? t->l->sz : 0);
    if (k <= left_sz) {
        auto [L, R] = split(t->l, k);
        t->l = R;
        pull(t);
        return {L, t};
    } else {
        auto [L, R] = split(t->r, k - left_sz - 1);
        t->r = L;
        pull(t);
        return {t, R};
    }
}

Node* merge(Node* A, Node* B) {
    if (!A || !B) return A ? A : B;
    if (A->pri > B->pri) {
        push(A);
        A->r = merge(A->r, B);
        pull(A);
        return A;
    } else {
        push(B);
        B->l = merge(A, B->l);
        pull(B);
        return B;
    }
}

struct ImplicitTreap {
    Node* root = nullptr;

    void build(const string& s) {
        root = nullptr;
        for (char c : s) {
            root = merge(root, new Node(c));
        }
    }

    void reverse(int l, int r) {
        auto [A, BC] = split(root, l);
        auto [B, C] = split(BC, r - l + 1);
        if (B) {
            B->rev = !B->rev;
        }
        root = merge(A, merge(B, C));
    }

    array<int, M> get_hash(int l, int r) {
        if (l > r) {
            array<int, M> res;
            for (int i = 0; i < M; i++) res[i] = 0;
            return res;
        }
        auto [A, BC] = split(root, l);
        auto [B, C] = split(BC, r - l + 1);
        array<int, M> res;
        if (B) {
            res = B->forw;
        } else {
            for (int i = 0; i < M; i++) res[i] = 0;
        }
        root = merge(A, merge(B, C));
        return res;
    }

    void collect(Node* t, string& out) {
        if (!t) return;
        push(t);
        collect(t->l, out);
        out.push_back(t->val);
        collect(t->r, out);
    }

    string toString() {
        string s;
        s.reserve(root ? root->sz : 0);
        collect(root, s);
        return s;
    }
};

set<array<int, M>> perid;
ImplicitTreap treap;
vector<int>spf(N);
int getPeriod(int l, int r){
    int Len = r - l + 1;
    int ans = Len;
    while(Len > 1){
        int p = spf[Len];
        if(treap.get_hash(l , r - ans / p) == treap.get_hash(l + ans / p, r)){
            ans /= p;
        }
        Len /= p;
    }
    return ans;
}
void solve() {
    int n, q;
    cin >> n >> q;
    string s;
    cin >> s;

    treap.build(s);

    while (q--) {
        int L, R;
        cin >> L >> R;
        L--; R--;
        int p = getPeriod(L, R);
        auto period_hash = treap.get_hash(L, L + p - 1);
        if (perid.find(period_hash) == perid.end()) {
            perid.insert(period_hash);
            treap.reverse(L, R);
        }
    }

    cout << treap.toString() << endl;
}

signed main() {
    fast();
    auto rnd = [&](int a, int b) {
        return a + rng() % (b - a + 1);
    };
    auto checkPrime = [&](int x) {
        if (x < 2) return false;
        for (long long i = 2; i * i <= x; ++i)
            if (x % i == 0)
                return false;
        return true;
    };

    for (int i = 0; i < M; ++i) {
        bases[i] = rnd(128, 1280);
        mods[i] = rnd(max(bases[i] + 5, (int)1e9 + 20), (int)2e9);
        while (!checkPrime(mods[i]))
            mods[i]--;
        powers[i][0] = 1;
        for (int j = 1; j < N; ++j) {
            powers[i][j] = (1LL * powers[i][j-1] * bases[i]) % mods[i];
        }
    }
    for(int i = 2; i < N; i++){
        if(spf[i] == 0){
            for(int j = i; j < N; j+= i){
                if(spf[j] == 0) spf[j] = i;
            }
        }
    }
    int test_cases = 1;
    while (test_cases--) {
        solve();
    }
    return 0;
}
