#include <bits/stdc++.h>
using namespace std;
 
struct BIT {
    vector<long long> tree;
    int n;
    BIT(int n): n(n), tree(n + 2) {}
 
    void add(int i, long long v) {
        while (i <= n) {
            tree[i] += v;
            i += i & -i;
        }
    }
 
    long long sum(int i) {
        long long res = 0;
        while (i > 0) {
            res += tree[i];
            i -= i & -i;
        }
        return res;
    }
 
    // lower_bound for the k-th element (by frequency)
    int kth(long long k) {
        int x = 0;
        for (int i = 20; i >= 0; --i) {
            if (x + (1 << i) <= n && tree[x + (1 << i)] < k) {
                k -= tree[x + (1 << i)];
                x += (1 << i);
            }
        }
        return x + 1;
    }
};
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    vector<int> all;
    for (int &x : a) {
        cin >> x;
        all.push_back(x);
    }
 
    // Coordinate compression
    sort(all.begin(), all.end());
    all.erase(unique(all.begin(), all.end()), all.end());
    int m = all.size();
 
    auto get = [&](int x) {
        return lower_bound(all.begin(), all.end(), x) - all.begin() + 1;
    };
 
    BIT bitFreq(m), bitSum(m);
 
    auto insert = [&](int x) {
        int idx = get(x);
        bitFreq.add(idx, 1);
        bitSum.add(idx, x);
    };
 
    auto erase = [&](int x) {
        int idx = get(x);
        bitFreq.add(idx, -1);
        bitSum.add(idx, -x);
    };
 
    for (int i = 0; i < k; ++i) insert(a[i]);
 
    auto get_cost = [&]() {
        int midPos = (k + 1) / 2;
        int midIdx = bitFreq.kth(midPos);
        int median = all[midIdx - 1];
 
        long long leftFreq = bitFreq.sum(midIdx - 1);
        long long leftSum = bitSum.sum(midIdx - 1);
 
        long long rightFreq = bitFreq.sum(m) - bitFreq.sum(midIdx);
        long long rightSum = bitSum.sum(m) - bitSum.sum(midIdx);
 
        long long cost = median * leftFreq - leftSum + rightSum - median * rightFreq;
        return cost;
    };
 
    cout << get_cost();
 
    for (int i = k; i < n; ++i) {
        erase(a[i - k]);
        insert(a[i]);
        cout << " " << get_cost();
    }
 
    cout << "\n";
}
