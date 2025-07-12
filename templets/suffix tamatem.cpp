#include <bits/stdc++.h>
using namespace std;
#define int long long

// Suffix Automaton Template with Full Spectrum of Applications
// Based on CP-Algorithms: https://cp-algorithms.com/string/suffix-automaton.html
// Alphabet size assumed lowercase a-z.

struct SuffixAutomaton {
    struct State {
        int len, link;
        map<char,int> next;       // transitions
        long long occ;            // number of endpos occurrences
        int first_pos;            // index of first appearance
        vector<int> inv_link;     // inverse suffix links
        State(): len(0), link(-1), occ(0), first_pos(-1) {}
    };

    vector<State> st;
    int last;

    // Constructor
    // Time: O(1)
    // Usage: SuffixAutomaton sa(maxlen);
    SuffixAutomaton(int maxlen = 0) {
        st.reserve(2 * maxlen);
        st.push_back(State());
        last = 0;
    }

    // extend: add character c at position pos
    // Time: amortized O(log|Σ|) with map, O(1) with array
    // Usage: for each char in string: sa.extend(c, index);
    void extend(char c, int pos) {
        int cur = st.size();
        st.push_back(State());
        st[cur].len = st[last].len + 1;
        st[cur].occ = 1;
        st[cur].first_pos = pos;
        int p = last;
        while (p != -1 && !st[p].next.count(c)) {
            st[p].next[c] = cur;
            p = st[p].link;
        }
        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = st.size();
                st.push_back(st[q]);
                st[clone].len = st[p].len + 1;
                st[clone].occ = 0;
                st[clone].first_pos = st[q].first_pos;
                while (p != -1 && st[p].next[c] == q) {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    // prepare: propagate occurrences and build inverse links
    // Time: O(n)
    // Usage: call after all extend() calls
    void prepare() {
        int sz = st.size();
        int max_len = 0;
        for (auto &s : st) max_len = max(max_len, s.len);
        vector<int> cnt(max_len + 1);
        for (auto &s : st) cnt[s.len]++;
        for (int i = 1; i <= max_len; ++i) cnt[i] += cnt[i - 1];
        vector<int> order(sz);
        for (int i = sz - 1; i >= 0; --i)
            order[--cnt[st[i].len]] = i;
        for (int v : order) {
            if (st[v].link != -1) {
                st[st[v].link].occ += st[v].occ;
                st[st[v].link].inv_link.push_back(v);
                int &fp = st[st[v].link].first_pos;
                fp = (fp == -1) ? st[v].first_pos : min(fp, st[v].first_pos);
            }
        }
    }

    // count_transitions: count total edges in automaton
    // Time: O(n * |Σ|)
    // Usage: long long edges = sa.count_transitions();
    long long count_transitions() const {
        long long tot = 0;
        for (auto &s : st) tot += s.next.size();
        return tot;
    }

    // is_substring: check if t is a substring of the original string
    // Time: O(|t| * log|Σ|)
    // Usage: bool ok = sa.is_substring(pattern);
    bool is_substring(const string &t) const {
        int v = 0;
        for (char c : t) {
            if (!st[v].next.count(c)) return false;
            v = st[v].next.at(c);
        }
        return true;
    }

    // count_distinct_substrings: number of distinct substrings
    // Time: O(n)
    // Usage: long long d = sa.count_distinct_substrings();
    long long count_distinct_substrings() const {
        long long res = 0;
        for (int v = 1; v < (int)st.size(); ++v)
            res += st[v].len - st[st[v].link].len;
        return res;
    }

    // total_length_of_distinct_substrings: sum of lengths of all distinct substrings
    // Time: O(n)
    // Usage: long long L = sa.total_length_of_distinct_substrings();
    long long total_length_of_distinct_substrings() const {
        long long ans = 0;
        for (int v = 1; v < (int)st.size(); ++v) {
            long long a = st[v].len;
            long long b = st[st[v].link].len;
            ans += (a*(a+1)/2) - (b*(b+1)/2);
        }
        return ans;
    }

    // kth_substring: returns k-th lexicographically smallest substring
    // Time: O(k * log|Σ|)
    // Usage: string s = sa.kth_substring(k);
    string kth_substring(long long k) const {
        int v = 0;
        string res;
        while (k > 0) {
            for (auto &p : st[v].next) {
                int u = p.second;
                long long cnt = st[u].len - st[v].len;
                if (cnt < k) {
                    k -= cnt;
                } else {
                    res.push_back(p.first);
                    --k;
                    v = u;
                    break;
                }
            }
        }
        return res;
    }

    // first_occurrence: index of first occurrence of t (or -1)
    // Time: O(|t| * log|Σ|)
    // Usage: int pos = sa.first_occurrence(t);
    int first_occurrence(const string &t) const {
        int v = 0;
        for (char c : t) {
            if (!st[v].next.count(c)) return -1;
            v = st[v].next.at(c);
        }
        return st[v].first_pos - (int)t.size() + 1;
    }

    // all_occurrences: list of starting indices of all occurrences of t
    // Time: O(|t| * log|Σ| + occ_count)
    // Usage: auto occs = sa.all_occurrences(t);
    vector<int> all_occurrences(const string &t) const {
        int v = 0;
        for (char c : t) {
            if (!st[v].next.count(c)) return {};
            v = st[v].next.at(c);
        }
        vector<int> res;
        function<void(int)> dfs = [&](int u) {
            if (st[u].occ > 0)
                res.push_back(st[u].first_pos - (int)t.size() + 1);
            for (int w : st[u].inv_link)
                dfs(w);
        };
        dfs(v);
        sort(res.begin(), res.end());
        res.erase(unique(res.begin(), res.end()), res.end());
        return res;
    }

    // shortest_non_appearing: finds shortest string not in the original
    // Time: O(n * |Σ|)
    // Usage: string s = sa.shortest_non_appearing();
    string shortest_non_appearing() const {
        queue<pair<int,string>> q;
        q.push({0, ""});
        while (!q.empty()) {
            auto [u, s] = q.front(); q.pop();
            for (char c = 'a'; c <= 'z'; ++c) {
                if (!st[u].next.count(c))
                    return s + c;
                int v = st[u].next.at(c);
                if ((int)s.size() + 1 < st[v].len)
                    q.push({v, s + c});
            }
        }
        return "";
    }

    // match_lengths: for each i, max length of substring ending at i in t that appears in original
    // Time: O(|t| * log|Σ|)
    // Usage: auto vec = sa.match_lengths(t);
    vector<int> match_lengths(const string &t) const {
        int v = 0, l = 0;
        vector<int> res(t.size());
        for (int i = 0; i < (int)t.size(); ++i) {
            char c = t[i];
            if (st[v].next.count(c)) {
                v = st[v].next.at(c);
                ++l;
            } else {
                while (v != -1 && !st[v].next.count(c))
                    v = st[v].link;
                if (v == -1) {
                    v = 0; l = 0;
                } else {
                    l = st[v].len + 1;
                    v = st[v].next.at(c);
                }
            }
            res[i] = l;
        }
        return res;
    }

    // match_from_start: for each pos i in b, length of longest substring b[i..] in a
    // Time: O((|a|+|b|) * log|Σ|)
    // Usage: auto vec = SuffixAutomaton::match_from_start(a,b);
    static vector<int> match_from_start(const string &a, const string &b) {
        string ra = a; reverse(ra.begin(), ra.end());
        string rb = b; reverse(rb.begin(), rb.end());
        SuffixAutomaton revSA(ra.size());
        for (int i = 0; i < (int)ra.size(); ++i) revSA.extend(ra[i], i);
        revSA.prepare();
        auto rev_ans = revSA.match_lengths(rb);
        int m = b.size();
        vector<int> ans(m);
        for (int i = 0; i < m; ++i) ans[i] = rev_ans[m - 1 - i];
        return ans;
    }

    // distinct_and_total: returns {distinct_count, sum_of_lengths}
    // Time: O(n)
    // Usage: auto [dc,tl] = sa.distinct_and_total();
    pair<long long,long long> distinct_and_total() const {
        long long count = 0, total = 0;
        for (int v = 1; v < (int)st.size(); ++v) {
            long long a = st[v].len;
            long long b = st[st[v].link].len;
            count += (a - b);
            total += (a*(a+1)/2) - (b*(b+1)/2);
        }
        return {count, total};
    }
};

// Sparse Table for Range Queries
template<typename T, class CMP = function<T(const T &, const T &)>>
class SparseTable {
public:
    int n;
    vector<vector<T>> sp;
    CMP func;

    // Constructor: builds table in O(n log n)
    // Usage: SparseTable st(vec, [](T a,T b){return min(a,b);});
    SparseTable(const vector<T> &a, const CMP &f) : func(f) {
        n = a.size();
        int max_log = 32 - __builtin_clz(n);
        sp.resize(max_log);
        sp[0] = a;
        for (int j = 1; j < max_log; ++j) {
            sp[j].resize(n - (1 << j) + 1);
            for (int i = 0; i + (1 << j) <= n; ++i) {
                sp[j][i] = func(sp[j-1][i], sp[j-1][i + (1 << (j-1))]);
            }
        }
    }

    // query: answers range [l..r] in O(1)
    // Usage: T v = st.query(l,r);
    T query(int l, int r) const {
        int lg = __lg(r - l + 1);
        return func(sp[lg][l], sp[lg][r - (1 << lg) + 1]);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) {
        int n,m,q; cin >> n >> m >> q;
        string a,b; cin >> a >> b;
        SuffixAutomaton sa(a.size());
        for (int i = 0; i < (int)a.size(); ++i) sa.extend(a[i], i);
        sa.prepare();

        auto ans = SuffixAutomaton::match_from_start(a,b);
        SparseTable<int> sp(ans, [](int x,int y){ return min(x,y); });
        while (q--) {
            int l,r; cin >> l >> r;
            l--; r--;
            int L = 1, R = r-l+1, best=0;
            while (L <= R) {
                int mid = (L+R)/2;
                if (sp.query(l, r-mid+1) >= mid) {
                    best = mid;
                    L = mid+1;
                } else R = mid-1;
            }
            cout << best << '\n';
        }
    }

    return 0;
}
