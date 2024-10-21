int merge (int g, int b){
	return min(g, b);
}
vector<vector<int>>sp;
void build (const vector<int>& a){
	int sz = a.size();
	sp = vector<vector<int>>(__lg(sz) + 5, vector<int>(sz));
    sp[0] = a;
    for (int i = 1; i < sp.size(); i++){
    	for (int j = 0; j + (1 << i) - 1 < sz; j++){
    		sp[i][j] = merge(sp[i - 1][j] , sp[i - 1][j + (1 << (i - 1))]);
    	}
    }
}
int query1(int l, int r){
	int res = // dpend on merge operation 
	int len = r - l + 1;
	for (int i = 0; l <= r; i++){
		if (len & (1 << i)){
			res = merge(res, sp[i][l]);
			l += (1 << i);
		}
	}
}
// time dpend on merge operatioin log for gcd ..... and o(1) for min max 
// query like [min , max , and, or , gcd] 
// because you take range from l and range from r there is a part will be computed twice ...  for above query it doesn't matter like min (a, a ) = a but in sum or xor the number of element is matter
int  query_for_overlaped(int l, int r){
    int maxL = __lg(r - l + 1);
	int res  = sp[maxL][l]; 
	res = merge(res, sp[maxL][r - (1 << maxL) + 1]);
	return res;
}
