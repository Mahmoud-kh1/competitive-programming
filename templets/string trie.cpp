struct Node {
    map<char, int> ch;
    int sz = 0;
    int isEnd = 0;
    int &operator[](char x) {
        return ch[x];
    }
};

struct Trie {
    vector<Node> trie;

    void init() {
        trie.clear();
        trie.emplace_back();
    }

    Trie() { init(); }

    int newNode() {
        trie.emplace_back();
        return (int)trie.size() - 1;
    }

    void update(string s, int op) {
        int cur = 0;
        for (auto I : s) {
            if (trie[trie[cur][I]].sz == 0) trie[cur][I] = newNode();
            cur = trie[cur][I];
            trie[cur].sz+= op;
        }
        trie[cur].isEnd += op;
    }

    long long getAns(string s) {
          int ans = 0;
          int cur = 0;
          for(auto i : s){
              if (trie[trie[cur][i]].sz < 2) break;
              ans++;
              cur = trie[cur][i];
          }
          return ans;
    }
};
