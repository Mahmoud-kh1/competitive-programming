// update character and for given pattern calc how much it appear in range [l ,r ]
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 1e5 + 4;
bitset<N>bt[26];
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s; cin >> s;
    int n = s.size();
    for(int i = 0; i < n; i++){
        bt[s[i] - 'a'][i] = 1;
    }
    int q; cin >> q;
    while(q--){
        int ty; cin >> ty;
        if (ty == 1){
            int idx; cin >> idx;
            idx--;
            char c; cin >> c;
            bt[s[idx] - 'a'][idx] = 0;
            bt[c - 'a'][idx] = 1;
            s[idx] = c;
        }
        else {
            bitset<N>ans;
            ans.flip();
            int l, r; cin >> l >> r;
            l--, r--;
            string y; cin >> y;
            if (r - l + 1  < y.size()){
                cout << 0 << endl;
                continue;
            }
            for(int i = 0; i < y.size(); i++){
                ans &= bt[y[i] - 'a'] >> i;
            }
            cout << (ans >> l).count() - (ans >> (r - y.size() + 2)).count() << endl;

        }
    }




    return 0;
}
