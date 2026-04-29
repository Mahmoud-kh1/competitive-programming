#include <bits/stdc++.h>

using namespace std;

#define int long long

ostream &operator<<(ostream &os, const __int128 & value){
    char buffer[64];
    char *pos = end(buffer) - 1;
    *pos = '\0';
    __int128 tmp = value < 0 ? -value : value;
    do{
        --pos;
        *pos = tmp % 10 + '0';
        tmp /= 10;
    }while(tmp != 0);
    if(value < 0){
        --pos;
        *pos = '-';
    }
    return os << pos;
}
istream &operator>>(istream &is, __int128 &value) {
    string s;
    is >> s;

    value = 0;
    int sign = 1;
    int i = 0;

    if (s[0] == '-') {
        sign = -1;
        i = 1;
    }

    for (; i < (int)s.size(); i++) {
        value = value * 10 + (s[i] - '0');
    }

    value *= sign;
    return is;
}
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while (t--) {
       
       // use cin, cout  as it's 
        
    }
    

    return 0;
}
