#include <bits/stdc++.h>
using namespace std;
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template <class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template <class T>
using ordered_multiset = tree<T, null_type, less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;
// #pragma GCC target("avx,avx2,popcnt,lzcnt")
#pragma GCC optimize("O3,unroll-loops,fast-math")
#ifdef LOCAL
#include "debug.h"
#else
#define debug(...)
#endif
#define endl '\n'
#define int int64_t
const long long mod = 1e9 + 7, MaxN = 10000005, INF = 2e9;
struct Segment_Tree{
	int sz = 1;
	vector<int>Max, Min;
	Segment_Tree(int N){
		while(sz < N)sz <<= 1;
		Max.resize(sz << 1, -INF);
		Min.resize(sz << 1, INF);
	}
	#define left node * 2 + 1
	#define right node * 2 + 2
	void update(int idx, int val, int node, int l, int r){
		if(l == r){
			Min[node] = Max[node] = val;
			return;
		}
		int mid = l + r >> 1;
		if(idx <= mid)update(idx, val, left, l, mid);
		else update(idx, val, right, mid + 1, r);
		Min[node] = min(Min[left], Min[right]);
		Max[node] = max(Max[left], Max[right]);
	}
	void update(int idx, int val){
		update(idx, val, 0, 0, sz - 1);
	}
	int getless(int node, int l, int r, int lq, int rq, int x){
		if(l > rq || r < lq || Min[node] >= x)return 0;
		if(l == r){
			return (Min[node] < x ? l : 0);
		}
		int mid = l + r >> 1;
		if(Min[left] < x){
			int ret = getless(left, l, mid, lq, rq, x);
			if(ret)return ret;
		}
		if(Min[right] < x){
			int ret = getless(right, mid + 1, r, lq, rq, x);
			return ret;
		}
		return 0;
	}
	int getless(int lq, int rq, int x){
		return getless(0, 0, sz - 1, lq, rq, x);
	}

	int getlarger(int node, int l, int r, int lq, int rq, int x){
		if(l > rq || r < lq || Max[node] <= x)return 0;
		if(l == r){
			return (Max[node] > x ? l : 0);
		}
		int mid = l + r >> 1;
		if(Max[left] > x){
			int ret = getlarger(left, l, mid, lq, rq, x);
			if(ret)return ret;
		}
		if(Max[right] > x){
			int ret = getlarger(right, mid + 1, r, lq, rq, x);
			if(ret)return ret;
		}
		return 0;
	}
	int getlarger(int lq, int rq, int x){
		return getlarger(0, 0, sz - 1, lq, rq, x);
	}
	int query(int lq, int rq, int node, int l, int r){
		if(l > rq || r < lq)return INF;
		if(l >= lq && r <= rq)return Min[node];
		int mid = l + r >> 1;
		return min(query(lq, rq, left, l, mid), query(lq, rq, right, mid + 1, r));
	}
	int query(int lq, int rq){
		return query(lq, rq, 0, 0, sz - 1);
	}
	#undef left
	#undef right	
};

void Solve(int Tc)
{
	int N;cin >> N;
	vector<int>a(N + 1);
	Segment_Tree seg(N + 1);
	for(int i = 1;i <= N;i++){
		cin >> a[i];
		seg.update(i, a[i]);
	}
	vector<array<int, 3>>v;
	for(int i = 1;i <= N;i++){
		int x = seg.getlarger(i, N, a[i]);
		if(!x)continue;
		int y = seg.getless(x, N, a[i]);
		if(!y)continue;
		v.push_back({i, x, y});
	}
	sort(v.begin(), v.end());
	vector<int>habd(N + 1, INF);
	for(auto [x, y, z] : v){
		habd[x] = z;
	}
	Segment_Tree st(N + 1);
	for(int i = 1;i <= N;i++){
		st.update(i, habd[i]);
	}
	int res = N * (N + 1) / 2;
	for(int i = 1;i <= N;i++){
		int x = st.query(i, N);
		if(x == INF)continue;
		res -= (N - x + 1);
	}
	cout << res << endl;
}
signed main()
{
	// freopen("promote.in","r",stdin);
	// freopen("promote.out","w",stdout);
#ifdef LOCAL
	FileRedirect("test");
	Timer timer;
#endif
	ios::sync_with_stdio(0);
	cin.tie(nullptr);
	cout.tie(nullptr);	
	int Tc = 1;
	// cin >> Tc;
	for (int T = 1; T <= Tc; T++)
	{
		Solve(T);
	}
}
/*
Thoughts:

*/
