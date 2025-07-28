#include <bits/stdc++.h>
using namespace std;

const int M = 31;
struct Node {
    int ch[2]{};
    int sz = 0;
    int mnIndex = 1e6;
    int isEnd = 0;

    int& operator[](int x) {
        return ch[x];
    }
};

struct Trie {
    vector<Node> trie;

    Trie() { init(); }

    void init() {
        trie.clear();
        trie.emplace_back();
    }

    int newNode() {
        trie.emplace_back();
        return (int)trie.size() - 1;
    }

    void update(int x, int in, int cur = 0, int i = M - 1) {
        if (i == -1) {
            trie[cur].isEnd++;
            trie[cur].sz++;
            trie[cur].mnIndex = min(trie[cur].mnIndex, in);
            return;
        }

        int bit = (x >> i) & 1;
        if (trie[cur][bit] == 0) trie[cur][bit] = newNode();

        update(x, in, trie[cur][bit], i - 1);
        trie[cur].sz++;
        trie[cur].mnIndex = min(trie[cur].mnIndex, trie[trie[cur][bit]].mnIndex);
    }

    int getAns(int xo, int L) {
        int cur = 0;
        int ans = 1e9;

        for (int i = M - 1; i >= 0; i--) {
            int bitx = (xo >> i) & 1;
            int bitL = (L >> i) & 1;

            if (bitL == 0 && bitx == 0) {
                if (trie[cur][1]) {
                    ans = min(ans, trie[trie[cur][1]].mnIndex);
                }
            } else if (bitx == 1 && bitL == 0) {
                if (trie[cur][0]) {
                    ans = min(ans, trie[trie[cur][0]].mnIndex);
                }
            }

            if (trie[cur][bitL ^ bitx]) {
                cur = trie[cur][bitL ^ bitx];
            } else {
                break;
            }
        }
        if (trie[cur].isEnd) ans = min(ans, trie[cur].mnIndex);
        return ans;
    }
};

void fastIO() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
}

int main() {
    fastIO();
    int n, x;
    cin >> n >> x;

    Trie trie;
    trie.update(0, 0);

    vector<int> a(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        a[i] ^= a[i - 1];
    }

    int L = 1, R = 0;
    for (int i = 1; i <= n; i++) {
        int left = trie.getAns(a[i], x);
        if (left != 1e9) {
            if (i - left > R - L + 1) {
                L = left + 1;
                R = i;
            } else if (i - left == R - L + 1 && left + 1 < L) {
                L = left + 1;
                R = i;
            }
        }
        trie.update(a[i], i);
    }

    cout << L << " " << R - L + 1 << endl;
    return 0;
}
