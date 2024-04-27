#include <bits/stdc++.h>
using namespace std;
// what if i give you array sorted and for every i I want to chose form 0 to i -1  elment that make this value of this equation is maximum or minumm value = a[i] + a[j] - 2*a[i]a[j] that 
// can be calc with n * n easy but i want it to be nlogn that we use convx hull it represent every element as a straight line from the equation and for known x-point he calc the maximum value 
// know what is the constant for the element we will calc is a[i] and the term we want to maximize or minumize is a[j] - 2 * a[i]a[j] we make the a[i] is x point and - 2 * a[j] and C is a[j] 
// y = m x + c 
// sort the value about slop in this equation which is here - 2 * a[j] descending order and push them in convenx then the algorithm do his work .....

// this code not for the specfic equation above it's for forg3 in atcoder dp contest
// 





const int N = 105; // Adjusted array size
const int M = 299005, mod = 1e9 + 7; // Adjusted array size
long long n, m;
long long  dp[M];
long long  a[M];
#define int long long
class convex{
	std::deque<pair<int,int>> v;
public : 
	 bool bad(pair<int,int>a, pair<int,int>b, pair<int,int>c){
		int lfh = (b.second - a.second) * (b.first - c.first);
		int rhs = (c.second - b.second) * (a.first - b.first);
     // if you want to maximize make it >= 
		return lfh <= rhs;
	}
   void add(pair<int,int>a){
   	   while((int)v.size() >= 2 && bad(a,v.back(),v[(int)v.size() - 2])){
   	   	  v.pop_back();
   	   }
   	   v.push_back(a);
   }
   int query(int h){
     // in ternary search you can do what you want to maximize or minmize 
     int l = 0, r = v.size() - 1, ans = 0;
      while (l <= r){
            int lft = l + (r - l) / 3l;
            int right = r - (r - l ) / 3;
        // this is value for the problem code dont to chang about the equation 
            int vr = v[right].second + v[right].first * h;
            int vl = v[lft].second +  v[lft].first * h;
           // if you want max make it >
            if (vl < vr) ans = vl,  r = right - 1;
            else ans = vr , l = lft + 1;

      }
      return ans;
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
    while (t--) {
         cin >> n >> m;
         
         for (int i = 0; i < n; i++){
         	cin >> a[i];             
         }
         convex con; // make and object; 
         con.add({-2*a[0],a[0] * a[0]});
         for (int i = 1; i < n; i++){
         	dp[i] = a[i] * a[i] + m + con.query(a[i]);
         	con.add({-2*a[i], dp[i]+ a[i] * a[i]});
         	cout << dp[i] << endl;
         }
         cout << dp[n - 1];
           
        

    }
    return 0;
} 
