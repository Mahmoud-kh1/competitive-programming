#include <bits/stdc++.h>
using namespace std;

#define int long long
struct SuffixArray {
    // suff is the suffix array with the empty suffix being suff[0]
    // lcp[i] holds the lcp between sa[i], sa[i - 1]
    // pos is rank of index i in suffix
    int n;
    vector<int> suff, lcp, pos, lg;
    
    // change 21 to log (n)
    vector<array<int, 21>> table;
    
    SuffixArray(string &s, int lim = 256) {
        n = s.size() + 1;
        int k = 0, a, b;
        vector<int> c(s.begin(), s.end() + 1), tmp(n), frq(max(n, lim));
        c.back() = 0;
        suff = lcp = pos = tmp, iota(suff.begin(), suff.end(), 0);
        for (int j = 0, p = 0; p < n; j = max(1ll, j * 2), lim = p) {
            p = j, iota(tmp.begin(), tmp.end(), n - j);
            for (int i = 0; i < n; i++)
                if (suff[i] >= j) tmp[p++] = suff[i] - j;
            fill(frq.begin(), frq.end(), 0);
            for (int i = 0; i < n; i++) frq[c[i]]++;
            for (int i = 1; i < lim; i++) frq[i] += frq[i - 1];
            for (int i = n; i--;) suff[--frq[c[tmp[i]]]] = tmp[i];
            swap(c, tmp), p = 1, c[suff[0]] = 0;
            for (int i = 1; i < n; i++) {
                a = suff[i - 1], b = suff[i];
                c[b] = tmp[a] == tmp[b] && tmp[a + j] == tmp[b + j] ? p - 1 : p++;
            }
        }
        for (int i = 1; i < n; i++) pos[suff[i]] = i;
        for (int i = 0, j; i < n - 1; lcp[pos[i++]] = k)
            for (k &&k--, j = suff[pos[i] - 1]; s[i + k] == s[j + k];
        k++) {}
        preLcp();
    }
 
    void preLcp() {
        lg.resize(n + 5);
        table.resize(n + 5);
        for (int i = 2; i < n + 5; ++i) lg[i] = lg[i / 2] + 1;
        for (int i = 0; i < n; ++i) table[i][0] = lcp[i];
        for (int j = 1; j <= lg[n]; ++j)
            for (int i = 0; i <= n - (1 << j); ++i)
                table[i][j] = min(table[i][j - 1], table[i + (1 << (j - 1))][j - 1]);
    }
 
    // pass the pos of the suffixes
    int queryLcp(int i, int j) {
        if (i == j) return n - suff[i] - 1;
        if (i > j) swap(i, j);
        i++;
        int len = lg[j - i + 1];
        return min(table[i][len], table[j - (1 << len) + 1][len]);
    }
    
    // freq[k] is how many distinct substring that occ exactly k times
    vector<int>getExactOcc(int sz) {
        vector<int>st, freq(sz + 5, 0);
        for (int i = 1; i <= sz + 1; i++) {
            int cur = (i == sz + 1 ? 0 : lcp[i]);
            while (!st.empty() && cur < lcp[st.back()]) {
                int h = lcp[st.back()];
                st.pop_back();
                int leftInd = (st.empty() ? -1 : st.back());
                int leftVal = (st.empty() ? 0 :lcp[st.back()]);
                int base = max(leftVal, cur);
                freq[i - leftInd]+= max(0ll, h - base);
            }
            st.push_back(i);
        }
        return freq;
    }
};

signed main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s; cin >> s;
    int n = s.size();
    SuffixArray sa(s); 
    
    
    vector<int>freq = sa.getExactOcc(n);
    int ans = n * (n + 1) / 2;
    for (int k = 1; k <= n; k++) {
        ans += freq[k] * k * (k - 1) / 2;
    }
    cout << ans;
    
    
    

    return 0;
}
