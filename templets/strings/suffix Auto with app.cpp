#include <bits/stdc++.h>
using namespace std;
#define int long long
const char nl = '\n';
const int inf = 4e18;

struct state {
    map<int, int> nxt, primes; // Transitions DAG (nxt) and auxiliary mappings
    int len = 0, link = -1;    // Length of longest substring in class; Suffix Link parent
    int id = 0;
    bool is_clone = false;     // True if node was created by splitting a transition
    int first_pos = 0;         // 0-based index of the end position of the first occurrence
    long long cnt = 0;         // Number of times substrings in this state appear in s
};

struct SAM {
    vector<state> st;
    vector<int> ans;
    vector<vector<int>> adj;   // Suffix Link Tree adjacency list (parent -> children)
    vector<int> prefixNode;    // prefixNode[i] = SAM state of prefix s[0...i]
    vector<int> nodePrefix;    // nodePrefix[u] = prefix length responsible for creating state u
    vector<vector<int>> up;    // up[u][lg] = 2^lg-th ancestor in the Suffix Link Tree
    vector<int> in, out, val;  // Euler Tour in/out timestamps and node mapping
    int sz = 0, last = 0, lgn = 0, timer = 0;
    string s;

    // ==========================================
    // 1. CONSTRUCTORS & INITIALIZATION
    // ==========================================

    SAM() {}
    SAM(int n) { init(n); }
    SAM(const string& _s) {
        init(max(1LL, 2 * (int)_s.size() + 5));
        build(_s);
    }

    /*
     * @purpose: Allocates memory and resets root state (0) and helper data structures.
     * @logic: A Suffix Automaton on string of length N has at most 2*N - 1 states.
     * @time: O(N) | @space: O(N log N) for binary lifting table.
     */
    void init(int n) {
        st.clear();
        st.resize(2 * n + 5);
        ans.resize(2 * n + 5);
        nodePrefix.resize(2 * n + 5);
        lgn = __lg(max(1LL, 2 * n));
        prefixNode.resize(2 * n + 5);
        up.assign(2 * n + 1, vector<int>(lgn + 2, -1));
        in.resize(2 * n + 5);
        out.resize(2 * n + 5);
        val.resize(2 * n + 5);
        sz = 1;
        st[0].len = 0;
        st[0].link = -1;
        st[0].is_clone = false;
        last = 0;
        timer = 0;
        s = "";
    }

    /*
     * @purpose: Builds the entire Suffix Automaton from a string and initializes queries.
     * @usage: SAM sa; sa.build("abracadabra");
     * @logic: Feeds characters one by one into extend(), then calls perp() to prep trees.
     * @time: O(N log Sigma) where Sigma is alphabet size | @space: O(N log N).
     */
    void build(const string& _s) {
        s = _s;
        for (int i = 0; i < (int)s.size(); i++) {
            extend(s[i], i);
        }
        perp();
    }

    /*
     * @purpose: Online SAM extension — adds character `c` at 0-based index `idx`.
     * @logic: Creates a new state for prefix s[0...idx]. Updates suffix link transitions.
     *         If a transition conflict occurs, splits the target node into a clone state
     *         so that `len` invariant (len(p) + 1 == len(q)) holds.
     * @time: O(log Sigma) amortized per character.
     */
    void extend(int c, int idx = -1) {
        if (idx == -1) idx = (int)s.size();
        int cur = sz++;
        st[cur].len = st[last].len + 1;
        st[cur].first_pos = st[cur].len - 1;
        st[cur].is_clone = false;
        st[cur].cnt = 1;

        int p = last;
        prefixNode[idx] = cur;
        nodePrefix[cur] = (idx + 1);

        while (p != -1 && st[p].nxt.count(c) == 0) {
            st[p].nxt[c] = cur;
            p = st[p].link;
        }
        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].nxt[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = sz++;
                st[clone].len = st[p].len + 1;
                st[clone].nxt = st[q].nxt;
                st[clone].link = st[q].link;
                st[clone].id = st[q].id;
                st[clone].is_clone = true;
                st[clone].cnt = 0;
                st[clone].first_pos = st[q].first_pos;

                while (p != -1 && st[p].nxt.count(c) && st[p].nxt[c] == q) {
                    st[p].nxt[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    /*
     * @purpose: Performs DFS on the Suffix Link Tree to compute Euler Tour timers & binary lifting.
     * @logic: Subtree of a state u in Suffix Link tree contains all states whose substrings
     *         have u as a suffix. `in`/`out` timers allow O(1) subtree checks; `up` table enables jumps.
     * @time: O(N log N) | @space: O(N log N).
     */
    void dfs(int u, int parent) {
        in[u] = timer++;
        val[in[u]] = nodePrefix[u];
        up[u][0] = parent;
        for (int lg = 1; lg <= lgn; lg++) {
            if (up[u][lg - 1] != -1) up[u][lg] = up[up[u][lg - 1]][lg - 1];
            else up[u][lg] = -1;
        }
        for (auto v : adj[u]) {
            dfs(v, u);
        }
        out[u] = timer - 1;
    }

    /*
     * @purpose: Prepares auxiliary trees after all characters are added.
     * @logic: Builds `adj` (Suffix Link Tree), runs `dfs(0, -1)`, and computes `cnt` frequencies.
     * @time: O(N log N).
     */
    void perp() {
        adj.assign(sz, vector<int>());
        for (int i = 1; i < sz; i++) {
            if (st[i].link != -1) {
                adj[st[i].link].push_back(i);
            }
        }
        timer = 0;
        dfs(0, -1);
        build_occurrences();
    }

    /*
     * @purpose: Finds the exact SAM state representing substring s[l...r] (0-indexed).
     * @usage: int state_id = sa.getIdSubstring(l, r);
     * @logic: Starts at `prefixNode[r]` (state of prefix s[0...r]) and jumps UP the Suffix Link
     *         Tree using binary lifting while ancestor length >= wanted length (r - l + 1).
     * @time: O(log N) per query | @space: O(1).
     */
    int getIdSubstring(int l, int r) {
        int node = prefixNode[r];
        int wantedLen = r - l + 1;
        for (int lg = lgn; lg >= 0; lg--) {
            if (up[node][lg] != -1 && st[up[node][lg]].len >= wantedLen) {
                node = up[node][lg];
            }
        }
        return node;
    }

    // ==========================================
    // 2. CORE SAM HELPER ALGORITHMS
    // ==========================================

    /*
     * @purpose: Returns SAM states sorted topologically by decreasing maximum length (`len`).
     * @logic: Uses Counting Sort on state lengths. In SAM, if u = link(v), then len(u) < len(v).
     *         Thus, decreasing length order is a valid reverse topological sort of Suffix Link Tree.
     * @time: O(S) where S is number of states | @space: O(S).
     */
    vector<int> order_desc() const {
        int mx = 0;
        for (int i = 0; i < sz; i++) mx = max(mx, st[i].len);
        vector<int> cnt(mx + 1, 0), ord(sz);
        for (int i = 0; i < sz; i++) cnt[st[i].len]++;
        for (int i = 1; i <= mx; i++) cnt[i] += cnt[i - 1];
        for (int i = sz - 1; i >= 0; i--) ord[--cnt[st[i].len]] = i;
        reverse(ord.begin(), ord.end());
        return ord;
    }

    /*
     * @purpose: Computes exact number of occurrences (`cnt`) in `s` for every SAM state.
     * @logic: Initial non-clone nodes appear once. Propagates counts from children to parents
     *         in Suffix Link Tree by processing states in decreasing order of length.
     * @time: O(S) | @space: O(1).
     */
    void build_occurrences() {
        for (int i = 0; i < sz; i++) {
            st[i].cnt = (i && !st[i].is_clone);
        }
        for (int v : order_desc()) {
            if (st[v].link != -1) {
                st[st[v].link].cnt += st[v].cnt;
            }
        }
    }

    /*
     * @purpose: Finds the state reached by walking from root along characters of string `t`.
     * @usage: int v = sa.go_state("abc"); if (v == -1) cout << "Not present";
     * @logic: Traverses transition map `nxt` character by character.
     * @time: O(|t| log Sigma) | @space: O(1).
     */
    int go_state(const string& t) const {
        int v = 0;
        for (char c : t) {
            auto it = st[v].nxt.find(c);
            if (it == st[v].nxt.end()) return -1;
            v = it->second;
        }
        return v;
    }

    // ==========================================
    // 3. ADVANCED STRING APPLICATIONS (24 APIs)
    // ==========================================

    /*
     * @purpose: Checks if string `t` is a substring of `s`.
     * @usage: bool ok = sa.contains("pattern");
     * @logic: True if and only if go_state(t) does not return -1.
     * @time: O(|t| log Sigma) | @space: O(1).
     */
    bool contains(const string& t) const {
        return go_state(t) != -1;
    }

    /*
     * @purpose: Counts total number of distinct substrings in `s`.
     * @usage: long long total = sa.count_distinct_substrings();
     * @logic: Each SAM state `u` represents distinct substrings of lengths from
     *         `len(link(u)) + 1` up to `len(u)`. Summing `(len(u) - len(link(u)))` gives total.
     * @time: O(S) | @space: O(1).
     */
    long long count_distinct_substrings() const {
        long long ans = 0;
        for (int i = 1; i < sz; i++) {
            ans += st[i].len - st[st[i].link].len;
        }
        return ans;
    }

    /*
     * @purpose: Calculates the sum of lengths of all distinct substrings in `s`.
     * @usage: long long sum_len = sa.total_length_distinct_substrings();
     * @logic: For each state, sums arithmetic progression [len(link)+1 ... len].
     *         Sum of integers from L to R is (R - L + 1) * (L + R) / 2.
     * @time: O(S) | @space: O(1).
     */
    long long total_length_distinct_substrings() const {
        long long ans = 0;
        for (int i = 1; i < sz; i++) {
            long long l = st[st[i].link].len + 1;
            long long r = st[i].len;
            ans += (r - l + 1) * (l + r) / 2;
        }
        return ans;
    }

    /*
     * @purpose: Counts how many distinct substrings exist for each length 1...N.
     * @usage: vector<long long> count_by_len = sa.distinct_by_length();
     * @logic: Uses a difference array over range [link.len + 1, u.len] for each state u,
     *         then computes prefix sums to get count per length.
     * @time: O(S + N) | @space: O(N).
     */
    vector<long long> distinct_by_length() const {
        int n = s.size();
        vector<long long> diff(n + 2, 0), ans(n + 1, 0);
        for (int i = 1; i < sz; i++) {
            int l = st[st[i].link].len + 1;
            int r = st[i].len;
            diff[l]++;
            diff[r + 1]--;
        }
        for (int i = 1; i <= n; i++) {
            diff[i] += diff[i - 1];
            ans[i] = diff[i];
        }
        return ans;
    }

    /*
     * @purpose: Finds the k-th lexicographically smallest distinct substring (1-indexed).
     * @usage: string kth = sa.kth_substring(5);
     * @logic: DP on transition DAG (`dp[v]` = total paths/substrings reachable from state v).
     *         Then greedily walks lexicographical transitions from root 0 while decrementing k.
     * @time: O(S * Sigma) | @space: O(S) for DP table.
     */
    string kth_substring(long long k) const {
        const long long INF = 4e18;
        vector<long long> dp(sz, -1);

        function<long long(int)> dfs_kth = [&](int v) -> long long {
            if (dp[v] != -1) return dp[v];
            long long res = 0;
            for (int c = 0; c < 26; c++) {
                char ch = 'a' + c;
                auto it = st[v].nxt.find(ch);
                if (it != st[v].nxt.end()) {
                    res += 1 + dfs_kth(it->second);
                    res = min(res, INF);
                }
            }
            return dp[v] = res;
        };

        dfs_kth(0);
        if (k <= 0 || k > dp[0]) return "";

        string ans;
        int v = 0;
        while (k > 0) {
            for (int c = 0; c < 26; c++) {
                char ch = 'a' + c;
                auto it = st[v].nxt.find(ch);
                if (it == st[v].nxt.end()) continue;
                int u = it->second;
                long long block = 1 + dp[u];
                if (k <= block) {
                    ans.push_back(ch);
                    k--;
                    if (k == 0) return ans;
                    v = u;
                    break;
                }
                k -= block;
            }
        }
        return ans;
    }
    // non disttinct 
    string getKth(int k) {
        vector<int>dp(sz + 1, - 1);
        function<int(int)>solve = [&](int u) {
            int &ret = dp[u];
            if (~ret) return ret;
            ret = st[u].cnt;
            for (auto [c, nxt] : st[u].nxt) {
                ret += solve(nxt);
            }
            return ret;
        };
        solve(0); 
        int cur = 0;
        string ans = "";
        while (k > 0) {
            for (auto [c, nxt] : st[cur].nxt) {
                if (k > dp[nxt]) {
                    k-= dp[nxt];
                    continue;
                }
                ans+= c;
                k-= st[nxt].cnt; 
                cur = nxt;
                break;
            }
        }
        return ans;
    }

    /*
     * @purpose: Counts total number of times pattern `t` appears in `s`.
     * @usage: long long freq = sa.count_occurrences("aba");
     * @logic: Walks `t` to find its state v, then returns precomputed `st[v].cnt`.
     * @time: O(|t| log Sigma) | @space: O(1).
     */
    long long count_occurrences(const string& t) const {
        int v = go_state(t);
        return v == -1 ? 0 : st[v].cnt;
    }

    /*
     * @purpose: Finds 0-based starting position of the FIRST occurrence of pattern `t`.
     * @usage: int first_idx = sa.first_occurrence("ana");
     * @logic: State v records `first_pos` (end index of first occurrence).
     *         Start position is `first_pos - |t| + 1`.
     * @time: O(|t| log Sigma) | @space: O(1).
     */
    int first_occurrence(const string& t) const {
        int v = go_state(t);
        if (v == -1) return -1;
        return st[v].first_pos - (int)t.size() + 1;
    }

    /*
     * @purpose: Internal DFS helper to collect all start indices from Suffix Link subtree.
     * @logic: Original occurrences correspond to non-clone states in subtree of state v.
     * @time: O(Number of occurrences) | @space: O(Recursion stack).
     */
    void report_all(int v, int len, vector<int>& res) const {
        if (!st[v].is_clone) {
            res.push_back(st[v].first_pos - len + 1);
        }
        for (int u : adj[v]) {
            report_all(u, len, res);
        }
    }

    /*
     * @purpose: Returns a sorted vector of ALL 0-based starting positions of pattern `t`.
     * @usage: vector<int> pos_list = sa.all_occurrences("ab");
     * @logic: Finds state v of `t`, traverses its Suffix Link Tree subtree via `report_all`,
     *         and sorts the collected indices.
     * @time: O(|t| + occ log(occ)) where occ is number of occurrences | @space: O(occ).
     */
    vector<int> all_occurrences(const string& t) const {
        int v = go_state(t);
        if (v == -1) return {};
        vector<int> res;
        report_all(v, (int)t.size(), res);
        sort(res.begin(), res.end());
        return res;
    }

    /*
     * @purpose: Finds the longest substring of `s` that appears at least `k` times.
     * @usage: string sub = sa.longest_substring_occurring_at_least(3);
     * @logic: Scans all SAM states where `st[i].cnt >= k` and selects the one with max `len`.
     * @time: O(S) | @space: O(S) for output string.
     */
    string longest_substring_occurring_at_least(long long k) const {
        int best = 0, pos = -1;
        for (int i = 1; i < sz; i++) {
            if (st[i].cnt >= k && st[i].len > best) {
                best = st[i].len;
                pos = st[i].first_pos;
            }
        }
        if (!best) return "";
        return s.substr(pos - best + 1, best);
    }

    /*
     * @purpose: Finds the longest substring that appears at least twice in `s`.
     * @usage: string repeated = sa.longest_repeated_substring();
     * @logic: Special case of `longest_substring_occurring_at_least` with k = 2.
     * @time: O(S) | @space: O(S).
     */
    string longest_repeated_substring() const {
        return longest_substring_occurring_at_least(2);
    }

    /*
     * @purpose: Maximizes (substring_length * frequency) across all substrings.
     * @usage: long long max_val = sa.max_repeat_value();
     * @logic: For each state `i`, evaluates `st[i].len * st[i].cnt` and takes the maximum.
     * @time: O(S) | @space: O(1).
     */
    long long max_repeat_value() const {
        long long ans = 0;
        for (int i = 1; i < sz; i++) {
            ans = max(ans, st[i].len * st[i].cnt);
        }
        return ans;
    }

    /*
     * @purpose: For each substring length 1...N, finds the maximum occurrence frequency.
     * @usage: vector<long long> max_occ = sa.max_occurrences_by_length();
     * @logic: Each state `i` provides count `st[i].cnt` valid for lengths in `[link.len + 1, u.len]`.
     *         Uses a priority queue to maintain maximum available frequency as we sweep lengths 1..N.
     * @time: O(S log S + N log S) | @space: O(N + S).
     */
    vector<long long> max_occurrences_by_length() const {
        int n = s.size();
        vector<vector<pair<int, long long>>> add(n + 2);
        vector<long long> ans(n + 1, 0);
        for (int i = 1; i < sz; i++) {
            int l = st[st[i].link].len + 1;
            int r = st[i].len;
            add[l].push_back({r, st[i].cnt});
        }
        priority_queue<pair<long long, int>> pq;
        for (int len = 1; len <= n; len++) {
            for (auto& [r, c] : add[len]) pq.push({c, r});
            while (!pq.empty() && pq.top().second < len) pq.pop();
            ans[len] = pq.empty() ? 0 : pq.top().first;
        }
        return ans;
    }

    /*
     * @purpose: Finds the longest substring appearing at least twice without overlapping.
     * @usage: string non_ovlp = sa.longest_non_overlapping_repeat();
     * @logic: Computes minimum (`mn`) and maximum (`mx`) start index for every SAM state
     *         by propagating up Suffix Link Tree. Valid non-overlapping length is
     *         min(state.len, mx[i] - mn[i]).
     * @time: O(S) | @space: O(S).
     */
    string longest_non_overlapping_repeat() const {
        vector<int> mn(sz, 1e9), mx(sz, -1e9);
        for (int i = 1; i < sz; i++) {
            if (!st[i].is_clone) {
                mn[i] = st[i].first_pos;
                mx[i] = st[i].first_pos;
            }
        }
        for (int v : order_desc()) {
            if (st[v].link != -1) {
                int p = st[v].link;
                mn[p] = min(mn[p], mn[v]);
                mx[p] = max(mx[p], mx[v]);
            }
        }
        int best = 0, state = -1;
        for (int i = 1; i < sz; i++) {
            int can = min(st[i].len, mx[i] - mn[i]);
            if (can > best) {
                best = can;
                state = i;
            }
        }
        if (!best) return "";
        return s.substr(st[state].first_pos - best + 1, best);
    }

    /*
     * @purpose: Finds the SHORTEST string (using first `k` lowercase letters) absent from `s`.
     * @usage: string missing = sa.shortest_absent(26); // using a-z
     * @logic: DP on SAM transition DAG (`dp[u]` = shortest absent string starting from state u).
     *         If transition for char 'a'+c is missing, dp = 1. Then reconstructs path.
     * @time: O(S * k) | @space: O(S).
     */
    string shortest_absent(int k) const {
        vector<int> dp(sz, -1);
        function<int(int)> dfs_absent = [&](int v) -> int {
            if (dp[v] != -1) return dp[v];
            int res = 1e9;
            for (int c = 0; c < k; c++) {
                char ch = 'a' + c;
                auto it = st[v].nxt.find(ch);
                if (it == st[v].nxt.end()) {
                    res = 1;
                    break;
                }
                res = min(res, 1 + dfs_absent(it->second));
            }
            return dp[v] = res;
        };

        dfs_absent(0);
        string ans;
        int v = 0;
        while (true) {
            if (dp[v] == 1) {
                for (int c = 0; c < k; c++) {
                    char ch = 'a' + c;
                    if (!st[v].nxt.count(ch)) {
                        ans.push_back(ch);
                        return ans;
                    }
                }
            }
            for (int c = 0; c < k; c++) {
                char ch = 'a' + c;
                auto it = st[v].nxt.find(ch);
                if (it != st[v].nxt.end() && dp[v] == 1 + dp[it->second]) {
                    ans.push_back(ch);
                    v = it->second;
                    break;
                }
            }
        }
    }

    /*
     * @purpose: Finds the lexicographically smallest cyclic rotation of string `str`.
     * @usage: string min_shift = SAM::minimal_cyclic_shift("baca");
     * @logic: Builds SAM on `str + str`, then greedily walks the smallest available
     *         lexicographical transition character N times from root 0.
     * @time: O(N log Sigma) | @space: O(N log N).
     */
    static string minimal_cyclic_shift(const string& str) {
        SAM sa(str + str);
        string ans;
        int v = 0, n = str.size();
        for (int i = 0; i < n; i++) {
            for (int c = 0; c < 26; c++) {
                char ch = 'a' + c;
                auto it = sa.st[v].nxt.find(ch);
                if (it != sa.st[v].nxt.end()) {
                    ans.push_back(ch);
                    v = it->second;
                    break;
                }
            }
        }
        return ans;
    }

    /*
     * @purpose: Computes the Longest Common Substring (LCS) of two strings A and B.
     * @usage: string common = SAM::LCS("abcdef", "zbcdex"); // returns "bcde"
     * @logic: Builds SAM on A. Streams B character by character through A's SAM.
     *         If transition exists, len++. Otherwise, jumps suffix link until transition exists.
     * @time: O(|A| + |B|) | @space: O(|A| log |A|).
     */
    static string LCS(const string& A, const string& B) {
        SAM sa(A);
        int v = 0, len = 0, best = 0, bestpos = 0;
        for (int i = 0; i < (int)B.size(); i++) {
            char c = B[i];
            while (v && sa.st[v].nxt.count(c) == 0) {
                v = sa.st[v].link;
                len = sa.st[v].len;
            }
            if (sa.st[v].nxt.count(c)) {
                v = sa.st[v].nxt[c];
                len++;
            } else {
                v = 0;
                len = 0;
            }
            if (len > best) {
                best = len;
                bestpos = i;
            }
        }
        return B.substr(bestpos - best + 1, best);
    }

    /*
     * @purpose: LCS of two strings returning {matching_string, posA, posB} (0-indexed starts).
     * @usage: auto [sub, iA, iB] = SAM::LCS_with_pos(A, B);
     * @logic: Same streaming algorithm as LCS, but also tracks `st[v].first_pos` in A.
     * @time: O(|A| + |B|) | @space: O(|A| log |A|).
     */
    static tuple<string, int, int> LCS_with_pos(const string& A, const string& B) {
        SAM sa(A);
        int v = 0, len = 0, best = 0, posA = 0, posB = 0;
        for (int i = 0; i < (int)B.size(); i++) {
            char c = B[i];
            while (v && sa.st[v].nxt.count(c) == 0) {
                v = sa.st[v].link;
                len = sa.st[v].len;
            }
            if (sa.st[v].nxt.count(c)) {
                v = sa.st[v].nxt[c];
                len++;
            } else {
                v = 0;
                len = 0;
            }
            if (len > best) {
                best = len;
                posB = i - best + 1;
                posA = sa.st[v].first_pos - best + 1;
            }
        }
        return {B.substr(posB, best), posA, posB};
    }

    /*
     * @purpose: Counts total number of DISTINCT substrings present in BOTH A and B.
     * @usage: long long common_distinct = SAM::count_common_distinct(A, B);
     * @logic: Builds SAM on A, streams B to find max matching length `best[v]` per state.
     *         Propagates `best[v]` UP Suffix Link Tree, then sums overlapping length ranges.
     * @time: O(|A| + |B|) | @space: O(|A| log |A|).
     */
    static long long count_common_distinct(const string& A, const string& B) {
        SAM sa(A);
        int S = sa.sz;
        vector<int> best(S, 0);
        int v = 0, len = 0;
        for (char c : B) {
            while (v && sa.st[v].nxt.count(c) == 0) {
                v = sa.st[v].link;
                len = sa.st[v].len;
            }
            if (sa.st[v].nxt.count(c)) {
                v = sa.st[v].nxt[c];
                len++;
            } else {
                v = 0;
                len = 0;
            }
            best[v] = max(best[v], len);
        }
        for (int x : sa.order_desc()) {
            if (sa.st[x].link != -1) {
                int p = sa.st[x].link;
                best[p] = max(best[p], min(best[x], sa.st[p].len));
            }
        }
        long long ans = 0;
        for (int i = 1; i < S; i++) {
            int low = sa.st[sa.st[i].link].len;
            ans += max(0LL, (long long)best[i] - low);
        }
        return ans;
    }

    /*
     * @purpose: Finds the Longest Common Substring across K strings.
     * @usage: string lcs_multi = SAM::LCS_many({"abac", "abde", "abfe"});
     * @logic: Builds SAM on SHORTEST string. Streams every other string through it,
     *         intersecting maximum matching lengths for each SAM state across all strings.
     * @time: O(sum(|s_i|) + K * |s_0|) | @space: O(|s_min| log |s_min|).
     */
    static string LCS_many(vector<string> ss) {
        if (ss.empty()) return "";
        int id = 0;
        for (int i = 1; i < (int)ss.size(); i++) {
            if (ss[i].size() < ss[id].size()) id = i;
        }
        swap(ss[0], ss[id]);
        SAM sa(ss[0]);
        int S = sa.sz;
        vector<int> common(S);
        for (int i = 0; i < S; i++) common[i] = sa.st[i].len;

        for (int idx = 1; idx < (int)ss.size(); idx++) {
            vector<int> cur(S, 0);
            int v = 0, len = 0;
            for (char c : ss[idx]) {
                while (v && sa.st[v].nxt.count(c) == 0) {
                    v = sa.st[v].link;
                    len = sa.st[v].len;
                }
                if (sa.st[v].nxt.count(c)) {
                    v = sa.st[v].nxt[c];
                    len++;
                } else {
                    v = 0;
                    len = 0;
                }
                cur[v] = max(cur[v], len);
            }
            for (int x : sa.order_desc()) {
                if (sa.st[x].link != -1) {
                    int p = sa.st[x].link;
                    cur[p] = max(cur[p], min(cur[x], sa.st[p].len));
                }
            }
            for (int i = 0; i < S; i++) common[i] = min(common[i], cur[i]);
        }
        int best = 0, state = 0;
        for (int i = 1; i < S; i++) {
            if (common[i] > best) {
                best = common[i];
                state = i;
            }
        }
        if (!best) return "";
        return ss[0].substr(sa.st[state].first_pos - best + 1, best);
    }

    /*
     * @purpose: Counts total number of DISTINCT substrings common to ALL K strings.
     * @usage: long long common_multi = SAM::count_common_many({"abac", "abde", "abfe"});
     * @logic: Same intersection array `common` as `LCS_many`, then sums valid intervals
     *         (common[i] - link.len) over all states where common[i] > link.len.
     * @time: O(sum(|s_i|) + K * |s_0|) | @space: O(|s_min| log |s_min|).
     */
    static long long count_common_many(vector<string> ss) {
        if (ss.empty()) return 0;
        int id = 0;
        for (int i = 1; i < (int)ss.size(); i++) {
            if (ss[i].size() < ss[id].size()) id = i;
        }
        swap(ss[0], ss[id]);
        SAM sa(ss[0]);
        int S = sa.sz;
        vector<int> common(S);
        for (int i = 0; i < S; i++) common[i] = sa.st[i].len;

        for (int idx = 1; idx < (int)ss.size(); idx++) {
            vector<int> cur(S, 0);
            int v = 0, len = 0;
            for (char c : ss[idx]) {
                while (v && sa.st[v].nxt.count(c) == 0) {
                    v = sa.st[v].link;
                    len = sa.st[v].len;
                }
                if (sa.st[v].nxt.count(c)) {
                    v = sa.st[v].nxt[c];
                    len++;
                } else {
                    v = 0;
                    len = 0;
                }
                cur[v] = max(cur[v], len);
            }
            for (int x : sa.order_desc()) {
                if (sa.st[x].link != -1) {
                    int p = sa.st[x].link;
                    cur[p] = max(cur[p], min(cur[x], sa.st[p].len));
                }
            }
            for (int i = 0; i < S; i++) common[i] = min(common[i], cur[i]);
        }
        long long ans = 0;
        for (int i = 1; i < S; i++) {
            int low = sa.st[sa.st[i].link].len;
            ans += max(0LL, (long long)common[i] - low);
        }
        return ans;
    }
};

signed main() {
     string s; cin >> s;
    int k; cin >> k;
    SAM st(s);
    cout << st.getKth(k) << endl;
}
