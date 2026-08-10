#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct PalindromicTree {
    // len: length of the maximum palindromic suffix at this node
    // link: suffix link (points to the longest proper palindromic suffix of this node)
    // cnt: how many distinct palindromes end at the first occurrence of this node
    // oc: frequency of this palindrome in the entire string (requires calling calc_occurrences() first)
    // st, en: start and end indices of the FIRST time this palindrome appeared in the string
    struct node {
        int len, st, en, link, cnt, oc;
        map<char, int> nxt; // nxt[c] points to the palindrome formed by adding 'c' to both ends
    };

    vector<node> t;       // Nodes (1-based indexing). 1 = odd root, 2 = even root
    int sz;               // Current number of nodes (highest node index)
    int last;             // Node index of the longest palindromic suffix of the current prefix
    string s;             // The string built so far

    // char_node[i] = node index of the longest palindromic suffix ending at index i
    vector<int> char_node; 

    // History stack for rolling back states (useful for DFS on trees or backtracking)
    // Stores: {type (0=existing, 1=new), previous_last, previous_cur, char_added}
    stack<array<int, 4>> st; 

    PalindromicTree() {
        init();
    }

    // Initialize/Reset the tree. CRITICAL for multi-testcase problems.
    void init() {
        t.assign(3, {});
        sz = 2; last = 2;
        
        // Node 1: Root for odd-length palindromes
        t[1].len = -1; t[1].link = 1; 
        
        // Node 2: Root for even-length palindromes
        t[2].len = 0;  t[2].link = 1; 
        
        s = "";
        char_node.clear();
        while(!st.empty()) st.pop();
    }

    // Pushes a new character into the tree.
    // Returns 1 if a NEW distinct palindrome was created, 0 if it already existed.
    int push(char ch) {
        s.push_back(ch);
        int cur = last, curlen = 0, pos = s.length() - 1;

        // 1. Find the longest palindromic suffix that can be expanded with 'ch'
        // We traverse suffix links until we find a node where the character preceding 
        // its match is equal to our new character 'ch'.
        while (true) {
            curlen = t[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) break;
            cur = t[cur].link;
        }
        
        // 2. If the expanded palindrome already exists in our tree
        if (t[cur].nxt[ch]) {
            st.push({0, last, 0, 0});
            last = t[cur].nxt[ch];
            t[last].oc++;
            char_node.push_back(last);
            return 0; // No new distinct palindrome created
        }

        // 3. Otherwise, create a new node for this newly discovered palindrome
        st.push({1, last, cur, (int) ch});
        sz++;
        last = sz;
        t.push_back({});
        char_node.push_back(last);

        t[sz].oc = 1;
        t[sz].len = t[cur].len + 2;
        t[cur].nxt[ch] = sz;
        t[sz].en = pos;
        t[sz].st = pos - t[sz].len + 1;

        // 4. Edge case: If it's a palindrome of length 1, its suffix link is the even root (Node 2)
        if (t[sz].len == 1) {
            t[sz].link = 2;
            t[sz].cnt = 1;
            return 1;
        }

        // 5. Find the suffix link for the new node.
        // We continue searching down the suffix links of 'cur' to find the next 
        // largest palindrome that can be expanded by 'ch'.
        while (true) {
            cur = t[cur].link;
            curlen = t[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) {
                t[sz].link = t[cur].nxt[ch];
                break;
            }
        }

        // 6. Number of palindromes ending here is 1 (the new one) + whatever ends at its suffix link
        t[sz].cnt = 1 + t[t[sz].link].cnt;
        return 1;
    }

    // Rolls back the last pushed character. O(1) time.
    void pop() {
        if (st.empty()) return;

        auto &curr = st.top();
        if (curr[0] == 0) { // Removed a character that didn't create a new node
            t[last].oc--;
            last = curr[1];
            char_node.pop_back();
        } else { // Removed a character that created a new node
            last = curr[1];
            t[curr[2]].nxt[(char) curr[3]] = 0;
            t.pop_back();
            char_node.pop_back();
            sz--;
        }
        st.pop();
        s.pop_back();
    }

    // Build tree from an entire string at once
    void build(const string& str) {
        for (char c : str) {
            push(c);
        }
    }

    // Propagates occurrences down the suffix link tree.
    // MUST BE CALLED AFTER ALL PUSHES ARE DONE if you need absolute frequencies!
    // A palindrome's occurrences include the occurrences of all palindromes it is a prefix/suffix of.
    void calc_occurrences() {
        for (int i = sz; i >= 3; i--) {
            t[t[i].link].oc += t[i].oc;
        }
    }

    // Total number of palindromes (counting duplicates) in the built string
    ll calc_total_pals() {
        ll ans = 0;
        for (int i = 0; i < s.length(); ++i) {
            // Using char_node avoids needing calc_occurrences if we just sum ends
            ans += t[char_node[i]].cnt; 
        }
        return ans;
    }

    // Number of unique palindromes in the string
    int calc_dist_pals() {
        return sz - 2;
    }

    // ========================================================================
    // USEFUL ADDITIONS FOR COMMON PROBLEMS
    // ========================================================================

    // Extracts the string representing the palindrome at a specific node
    string get_pal(int node_idx) {
        if (node_idx <= 2) return "";
        return s.substr(t[node_idx].st, t[node_idx].len);
    }

    // Classic APIO 2014 Problem: "Palindromes"
    // Finds the maximum value of (Length of Palindrome * Its Frequency)
    // Remember to call calc_occurrences() before running this!
    ll get_max_pal_score() {
        ll max_score = 0;
        for (int i = 3; i <= sz; i++) {
            max_score = max(max_score, 1LL * t[i].len * t[i].oc);
        }
        return max_score;
    }

    // Returns a vector of all unique palindromic substrings found
    vector<string> get_all_distinct_pals() {
        vector<string> res;
        for (int i = 3; i <= sz; i++) {
            res.push_back(get_pal(i));
        }
        return res;
    }
};
