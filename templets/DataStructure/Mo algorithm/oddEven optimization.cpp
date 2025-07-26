#include <bits/stdc++.h>
using namespace std;

int block_size;

struct Query {
    int l, r, idx;
    // we order by (l/block, then r ascending on even blocks, descending on odd)
    bool operator<(const Query &o) const {
        auto b1 = l / block_size;
        auto b2 = o.l / block_size;
        if (b1 != b2) return b1 < b2;
        if (b1 & 1)       return r > o.r;
        else              return r < o.r;
    }
};

static const int MAXV = 2'000'000;
long long freq[MAXV];

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    vector<Query> qq(q);
    for (int i = 0; i < q; i++){
        cin >> qq[i].l >> qq[i].r;
        qq[i].l--;  // make 0‑based
        qq[i].r--;
        qq[i].idx = i;
    }

    block_size = static_cast<int>(sqrt(n)) + 1;
    sort(qq.begin(), qq.end());

    long long curr_ans = 0;
    auto add = [&](int i){
        long long x = a[i];
        // remove old contribution
        curr_ans -= freq[x] * freq[x] * x;
        freq[x]++;
        // add new
        curr_ans += freq[x] * freq[x] * x;
    };
    auto remove = [&](int i){
        long long x = a[i];
        curr_ans -= freq[x] * freq[x] * x;
        freq[x]--;
        curr_ans += freq[x] * freq[x] * x;
    };

    vector<long long> answer(q);
    int L = 0, R = -1;

    for (auto &qu : qq) {
        int l = qu.l, r = qu.r, idx = qu.idx;
        while (L > l)    add(--L);
        while (R < r)    add(++R);
        while (L < l)    remove(L++);
        while (R > r)    remove(R--);
        answer[idx] = curr_ans;
    }

    for (auto x : answer) {
        cout << x << "\n";
    }
    return 0;
}
