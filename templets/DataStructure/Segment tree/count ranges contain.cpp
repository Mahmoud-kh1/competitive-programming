
// for every range in input count how many range it contain and how many contain it
#include <bits/stdc++.h> 
using namespace std;
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
// make it less_equal to make it multiset
typedef tree<int, null_type, less_equal<int>, rb_tree_tag, tree_order_statistics_node_update> pbds; // find_by_order, order_of_key
void fast() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}
 
#define int long long
 
#define all(a) a.begin(), a.end()
 
signed main() {
    fast();
    int t = 1;
    while (t--) {
        pbds s;
        int n;
        cin >> n;
        vector<array<int,3>> v(n);
        for (int i = 0; i < n; i++) {
            cin >> v[i][0] >> v[i][1]; v[i][2] = i;
        }
        sort(all(v), [](auto &a, auto &b) {
            if (a[0] == b[0]) return a[1] > b[1];
            return a[0] < b[0];
        });
        int contain[n] = {0}, contained[n] = {0};
        map<int,int>freq;
        for (int i = 0; i < n; i++){
            int x = s.order_of_key(v[i][1]);
            contained[v[i][2]] = (int)s.size() - x;
                 s.insert(v[i][1]);
                 freq[v[i][1]] ++;
        }
        for (int i = 0; i < n; i++){
            auto it = s.find_by_order(s.order_of_key(v[i][1]));
            if (it != s.end()) {
                freq[v[i][1]]--;
                s.erase(it);
            }
            contain[v[i][2]] = s.order_of_key(v[i][1]) + freq[v[i][1]];
        }
        for(int i = 0; i < n; i++) cout << contain[i] << ' ';
        cout << endl;
        for (int i = 0; i < n; i++) cout << contained[i] << ' ';
    }
 
    return 0;
}
