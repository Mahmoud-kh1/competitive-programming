#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
#pragma GCC optimize("O3,unroll-loops,fast-math")
#ifdef LOCAL
#include "debug.h"
#else
#define debug(...)
#endif
#define endl '\n'
#define int int64_t
#define debug(...)
const long long mod = 1e9 + 7, MaxN = 1e5 + 5, INF = 2e18;
//mtnsa4 t8yarha
struct Segment_Tree_Gcd
{
	int sz = 1;
	vector<int> tree;
	Segment_Tree_Gcd(int N)
	{
		while (sz < N)
			sz <<= 1;
		tree.resize(sz * 2);
	}
#define left node * 2 + 1
#define right node * 2 + 2
	void update(int idx, int val, int node, int l, int r)
	{
		if (l == r)
			return void(tree[node] = val);
		int mid = l + r >> 1;
		if (idx <= mid)
			update(idx, val, left, l, mid);
		else
			update(idx, val, right, mid + 1, r);
		tree[node] = __gcd(tree[left], tree[right]);
	}
	void update(int idx, int val)
	{
		update(idx, val, 0, 0, sz - 1);
	}

	int query(int lq, int rq, int node, int l, int r)
	{
		if (l > rq || r < lq)
			return 0;
		if (l >= lq && r <= rq)
			return tree[node];
		int mid = l + r >> 1;
		return __gcd(query(lq, rq, left, l, mid), query(lq, rq, right, mid + 1, r));
	}
	int query(int lq, int rq)
	{
		return query(lq, rq, 0, 0, sz - 1);
	}
#undef left
#undef right
};
struct Segment_Tree
{
	int sz = 1;
	vector<int> tree, lazy;
	Segment_Tree(int N)
	{
		while (sz < N)
			sz <<= 1;
		tree.resize(sz * 2);
		lazy.resize(sz * 2);
	}
#define left node * 2 + 1
#define right node * 2 + 2
	void apply(int node, int l, int r, int val)
	{
		tree[node] += (r - l + 1) * val;
		lazy[node] += val;
	}
	void propagate(int node, int l, int r)
	{
		if (l == r)
			return;
		int mid = l + r >> 1;
		apply(left, l, mid, lazy[node]);
		apply(right, mid + 1, r, lazy[node]);
		lazy[node] = 0;
	}
	void update(int lq, int rq, int val, int node, int l, int r)
	{
		propagate(node, l, r);
		if (l > rq || r < lq)
			return;
		if (l >= lq && r <= rq)
		{
			apply(node, l, r, val);
			return;
		}
		int mid = l + r >> 1;
		update(lq, rq, val, left, l, mid);
		update(lq, rq, val, right, mid + 1, r);
		tree[node] = tree[left] + tree[right];
	}
	void update(int lq, int rq, int val)
	{
		update(lq, rq, val, 0, 0, sz - 1);
	}

	int query(int lq, int rq, int node, int l, int r)
	{
		propagate(node, l, r);
		if (l > rq || r < lq)
			return 0;
		if (l >= lq && r <= rq)
			return tree[node];
		int mid = l + r >> 1;
		return query(lq, rq, left, l, mid) + query(lq, rq, right, mid + 1, r);
	}
	int query(int lq, int rq)
	{
		return query(lq, rq, 0, 0, sz - 1);
	}
#undef left
#undef right
};
Segment_Tree_Gcd seg_gcd(MaxN);
Segment_Tree seg(MaxN);
const int Log = 20;
vector<int>End(MaxN);
vector<int> heavy(MaxN), head(MaxN), pos(MaxN), a(MaxN), depth(MaxN);
vector<vector<int>> parent(Log, vector<int>(MaxN)), adj(MaxN);
int dfs(int node, int par)
{
	parent[0][node] = par;
	int sz = 1, Max = 0;
	for (auto child : adj[node])
	{
		if (child == par)
			continue;
		depth[child] = depth[node] + 1;
		int cur = dfs(child, node);
		sz += cur;
		if (cur > Max)
			heavy[node] = child, Max = cur;
	}
	return sz;
}
int cur = 0;
void decompose(int H, int node, int par)
{
	head[node] = H;
	pos[node] = ++cur;
	seg_gcd.update(pos[node], a[node]);
	if (heavy[node])
		decompose(H, heavy[node], node);
	else End[node] = 1;
	for (auto child : adj[node])
	{
		if (child == par || child == heavy[node])
			continue;
		decompose(child, child, node);
	}
}
void init(int N)
{
	dfs(1, 0);
	decompose(1, 1, 0);
	for (int i = 1; i < Log; i++)
	{
		for (int u = 1; u <= N; u++)
		{
			parent[i][u] = parent[i - 1][parent[i - 1][u]];
		}
	}
	for(int i = 1;i <= N;i++){
		seg.update(i, i, 2);
	}
}
int get_kth(int u, int k)
{
	for (int i = 0; i < Log; i++)
	{
		if (k >> i & 1)
			u = parent[i][u];
	}
	return u;
}
int Lca(int u, int v)
{
	if (depth[u] < depth[v])
		swap(u, v);
	u = get_kth(u, depth[u] - depth[v]);
	if (u == v)
		return u;
	for (int i = Log - 1; i >= 0; i--)
	{
		if (parent[i][u] != parent[i][v])
		{
			u = parent[i][u], v = parent[i][v];
		}
	}
	return parent[0][u];
}
int query(int u, int v)
{
	int ret = 0;
	while (head[u] != head[v])
	{
		if (depth[head[u]] > depth[head[v]])
			swap(u, v);
		int c = seg_gcd.query(pos[head[v]] + 1, pos[v]);
		int yy = seg.query(pos[head[v]], pos[head[v]]);
		c = __gcd(c, yy);
		ret = __gcd(ret, c);
		v = parent[0][head[v]];
	}
	if (depth[u] > depth[v])
		swap(u, v);
	int c = seg_gcd.query(pos[u] + 1, pos[v]);
	int yy = seg.query(pos[u], pos[u]);
	// debug(c, yy, u, v, pos[u], pos[v]);
	c = __gcd(c, yy);
	ret = __gcd(ret, c);
	return ret;
}
int Add(int a, int b)
{
	return (a + b) % mod;
}
int Mul(int a, int b)
{
	return (a * b) % mod;
}
struct Fib
{
	struct Matrix
	{
		int N, M;
		vector<vector<int>> matrix;
		Matrix() {};
		Matrix(int _N, int _M)
		{
			N = _N, M = _M;
			matrix = vector<vector<int>>(N, vector<int>(M));
		}
		vector<int> &operator[](int idx)
		{
			assert(idx < N);
			return matrix[idx];
		}
		friend Matrix operator*(Matrix a, Matrix b)
		{
			Matrix result(a.N, b.M);
			assert(a.M == b.N);
			for (int i = 0; i < a.N; i++)
			{
				for (int j = 0; j < b.M; j++)
				{
					for (int k = 0; k < a.M; k++)
					{
						result[i][j] = Add(result[i][j], Mul(a[i][k], b[k][j]));
					}
				}
			}
			return result;
		}
	};
	Matrix Identity(int N)
	{
		Matrix matrix(N, N);
		for (int i = 0; i < N; i++)
			matrix[i][i] = 1;
		return matrix;
	}
	Matrix power(Matrix a, int b)
	{
		if (!b)
			return Identity(a.N);
		if (b & 1)
			return a * power(a, b - 1);
		auto res = power(a, b >> 1);
		return res * res;
	}
	int FIB(int N){
		Matrix tran(2, 2);
		tran[0][1] = tran[1][0] = tran[1][1] = 1;
		Matrix fib(2, 2);
		fib[0][1] = 1;
		auto res = fib * power(tran, N);
		return res[0][0];
	}
};
void Update(int u, int v, int val){
	int lca = Lca(u, v);
	bool lca_done = 0;
	for(auto x : vector<int>{u, v}){
		if(u == v && lca_done)break;
		while(pos[head[x]] > pos[lca]){
			seg.update(pos[head[x]], pos[x], val);
			x = parent[0][head[x]];
		}
		seg.update(pos[lca] + (lca_done), pos[x], val);
		int L = seg.query(pos[lca] - 1, pos[lca] - 1);
		int R = seg.query(pos[lca], pos[lca]);
		debug(lca, head[lca], lca_done, L, R);
		if(!lca_done && lca != head[lca]){
			seg_gcd.update(pos[lca], R - L);
		}
		int q = query(u, v);
		debug(q, u, v, head[lca], lca);
		lca_done = 1;
	}
	
	if(!End[u]){
		int L = seg.query(pos[u], pos[u]);
		int R = seg.query(pos[u] + 1, pos[u] + 1);
		seg_gcd.update(pos[u] + 1, R - L);
	}
	if(!End[v] && u != v){
		int L = seg.query(pos[v], pos[v]);
		int R = seg.query(pos[v] + 1, pos[v] + 1);
		seg_gcd.update(pos[v] + 1, R - L);
	}
	debug(u, v);
	debug(query(u, v));
}
void Solve(int Tc)
{
	int N, Q;
	cin >> N >> Q;
	for(int i = 2;i <= N;i++){
		int p;cin >> p;
		adj[i].push_back(p);
		adj[p].push_back(i);
	}	
	init(N);
	Fib fib;
	while(Q--){
		int tp, u, v;
		cin >> tp >> u >> v;
		if(tp == 1){
			int x;cin >> x;
			Update(u, v, x);
		}
		else{
			int idx = abs(query(u, v));
			debug(idx);
			int res = fib.FIB(idx);
			cout << res << endl;
		}
	}
}
signed main()
{
	// freopen("exam.in","r",stdin);
	// freopen("mootube.out","w",stdout);
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
		Solve(T);
}
/*
Thoughts:

*/
