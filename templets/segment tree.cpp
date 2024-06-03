#include <bits/stdc++.h>
using namespace std;

#define int long long
int combine(int a, int b){
	return a + b;
}
struct segTree{
	std::vector<long long> tree;
	int sz;
	int buffer = 0;
	void build (int n, vector<int>&a){
		int old = n;
		if (old & (old - 1) == 0) sz = old;
		else {
			sz = (1  << (__lg(old) + 1));
		}
		tree.resize(sz << 1);
		for (int g = 0; g < old; g++){
			tree[g + sz] = a[g];
		}
		// buffer
		for (int g = old + sz; g < (sz << 1); g++) tree[g] = buffer;
		for (int g = sz - 1; g >= 1; g--) tree[g] = combine (tree[g << 1] , tree[g << 1 | 1]);
	}
   int query(int ql ,int qr , int k, int sl, int sr){
   	 // if s is in q totally 
   	  if (sl >= ql && sr <= qr) return tree[k];
   	  if (ql > sr || sl > qr) return 0;
   	  int mid = (sl + sr) / 2;
   	  return  combine( query(ql, qr, k << 1, sl, mid) , query(ql, qr, k << 1 | 1, mid + 1, sr));

   }
   void update(int k, int val){
         tree[k] = val;
         k/=2;
         while(k != 0){
            tree[k] = combine(tree[k << 1] , tree[k << 1 | 1]);
            k/=2;
         }
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
    int t = 1; //cin >> t;
  Z:  while(t--){
         int n, m; cin >> n >> m;
         vector<int>a(n);
         for (auto &i : a) cin >> i;
         segTree sg;
         sg.build( n, a);
         while(m--){
         	int type; cin >> type;
         	if (type == 1){
         		int in, va; cin >> in >> va; // zero base
         		int k = in + sg.sz;
         		sg.update(k, va);
         	}
         	else {
         		int l, r; cin >> l >> r; // zero base
         		cout << sg.query(l, r -1,1, 0, sg.sz - 1) << endl;
      }

 }
         
     
 }      
     return 0;
        
}
