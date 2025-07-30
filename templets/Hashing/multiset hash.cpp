#include <bits/stdc++.h>
using namespace std;

random_device rd;
mt19937 mt(rd());

int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(mt); }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n , q; cin >> n >> q;
    map<int,long long>val1, val2;
    long long a[n], b[n], pre1a[n + 1] {}, pre2a[n + 1]{}, pre1b[n + 1]{}, pre2b[n +  1]{};
    for(int i = 0;i < n ;i++){
        cin >> a[i];
        if(val1.find(a[i]) == val1.end()) val1[a[i]] = rnd(1, 2e11);
        if(val2.find(a[i]) == val2.end()) val2[a[i]] = rnd(1, 2e11);
    }
    for(int i = 0;i < n ;i++){
        cin >> b[i];
        if(val1.find(b[i]) == val1.end()) val1[b[i]] = rnd(1, 2e11);
        if(val2.find(b[i]) == val2.end()) val2[b[i]] = rnd(1, 2e11);
    }

    for(int i = 1; i <= n; i++){
        pre1a[i] += pre1a[i - 1] + val1[a[i - 1]];
        pre2a[i] += pre2a[i - 1] + val2[a[i - 1]];
        pre1b[i] += pre1b[i - 1] + val1[b[i - 1]];
        pre2b[i] += pre2b[i - 1] + val2[b[i - 1]];
    }
    while(q--){
        int l1, r1, l2, r2; cin >> l1 >> r1 >> l2 >> r2;
        if (r2 - l2 != r1  - l1)cout << "No" << endl;
       else  if (pre1a[r1] - pre1a[l1 - 1] == pre1b[r2] - pre1b[l2 - 1] && pre2a[r1] - pre2a[l1 - 1] == pre2b[r2] - pre2b[l2 - 1]){
            cout << "Yes" << endl;
        }
        else cout << "No" << endl;
    }




    return 0;
}
