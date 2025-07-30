const int INFi = 2e9;
const ll INF = 2e18;
const int LG = 20;
 
struct SegTree {
    vector<int> t;
    int n;
 
    void build(int sz) {
        n = sz;
        t.resize(n * 4);
    }
 
    vector<pair<int, int>> updates;
 
    void upd(int v, int l, int r, int lq, int rq, int x) {
        if (lq >= r || l >= rq) return;
        if (lq <= l && r <= rq) {
            if (t[v] < x) {
                updates.emplace_back(v, t[v]);
                t[v] = x;
            }
            return;
        }
        upd(v << 1, l, (l + r) >> 1, lq, rq, x);
        upd(v << 1 | 1, (l + r) >> 1, r, lq, rq, x);
        if (t[v] < min(t[v << 1], t[v << 1 | 1])) {
            updates.emplace_back(v, t[v]);
            t[v] = min(t[v << 1], t[v << 1 | 1]);
        }
    }
 
    void upd(int lq, int rq, int x) {
        upd(1, 0, n, lq, rq, x);
    }
 
    int get(int v, int l, int r, int lq, int rq) {
        if (lq >= r || l >= rq) return INFi;
        if (lq <= l && r <= rq) return t[v];
        int res = get(v << 1, l, (l + r) >> 1, lq, rq);
        ckmin(res, get(v << 1 | 1, (l + r) >> 1, r, lq, rq));
        return max(res, t[v]);
    }
 
    int get(int lq, int rq) {
        return get(1, 0, n, lq, rq);
    }
 
    int Version() {
        return updates.size();
    }
 
    void Back(int v) {
        while ((int)updates.size() > v) {
            auto [i, x] = updates.back();
            updates.pop_back();
            t[i] = x;
        }
    }
} st;
 
const int N = 2e5 +  500;
 
vi T[N * 4];
vector<ar<int, 3>> who;
vector<ar<int, 4>> qs;
 
void Add(int v, int l, int r, int lq, int rq, int i) {
    if (l >= rq || lq >= r) return;
    if (lq <= l && r <= rq) {
        T[v].push_back(i);
        return;
    }
    Add(v << 1, l, (l + r) >> 1, lq, rq, i);
    Add(v << 1 | 1, (l + r) >> 1, r, lq, rq, i);
}
 
vi ans;
 
void dfs(int v, int l, int r) {
    int ver = st.Version();
    for(auto &i : T[v]) {
        st.upd(who[i][0], who[i][1], who[i][2]);
    }
    if (l + 1 == r && qs[l][0] == 2) {
        ans.push_back(st.get(qs[l][1], qs[l][2]));
    }
    if (l + 1 != r) {
        dfs(v << 1, l, (l + r) >> 1);
        dfs(v << 1 | 1, (l + r) >> 1, r);
    }
 
    st.Back(ver);
}
