#define all(a) a.begin(), a.end()
struct Seg{
	int sz;
	vector<int> tree;
	void build(vector<int>&a){
		int old = a.size();
		if (old & (old - 1)) sz = (1 << (__lg(old) + 1));
		else sz = old;
		tree.resize(sz * 2, 0);
	}
	  void update(int k, int sl, int sr, int kl, int kr, int value) {
        if (sl > kr || sr < kl) return;
        if (sl == sr) {
            tree[k] = value;
            return;
        }
        int mid = (sl + sr) / 2;
        update(k << 1, sl, mid, kl, kr, value);
        update(k << 1 | 1, mid + 1, sr, kl, kr, value);
        tree[k] = tree[k << 1] + tree[k << 1 | 1];
    }
	int query(int k, int sl, int sr, int kl, int kr){
		if(kl > kr) return 0;
		 if (kl > sr || sl > kr) return 0;
		if (sl >= kl && sr <= kr) return tree[k];
		if(min(sr, kr) < max(sl, kl)) return 0;
		int mid = (sl + sr) / 2;
		return query(k << 1 , sl, mid, kl, kr) + query(k << 1 | 1, mid + 1,sr, kl, kr);
	}
};
 
signed main() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
 
#ifndef ONLINE_JUDGE
    freopen("cin.txt", "r", stdin);
    freopen("cout.txt", "w", stdout);
#endif
                    
   int t = 1; 
   while(t--){
    int n, q; cin >> n >> q;
    vector<int>a(n); for (int i = 0; i < n; i++) cin >> a[i];
    Seg seg;
    seg.build(a);
 
    vector<pair<int,pair<int,int>>>cur(q);
    vector<int>ans(q);
     for (int i = 0; i < q; i++) cin >> cur[i].second.first >> cur[i].first, cur[i].second.second = i;
    sort(all(cur));
    map<int,int>last;
    int in = 0;
    // for (auto i : cur) cout << i.first << ' ' << i.second << endl;
    for (int i = 0; i < n; i++) {
            if (last.find(a[i]) != last.end()) {
                seg.update(1, 0, seg.sz - 1, last[a[i]], last[a[i]], 0);
            }
            seg.update(1, 0, seg.sz - 1, i, i, 1);
            last[a[i]] = i;
 
            while (in < q && i + 1 == cur[in].first) {
                ans[cur[in].second.second] = seg.query(1, 0, seg.sz - 1, cur[in].second.first - 1, cur[in].first - 1);
                in++;
            }
        }
        for (int i = 0; i < q; i++) cout << ans[i] << endl;
   }  
 
   
 
 
 
    return 0;
   	
   
}
