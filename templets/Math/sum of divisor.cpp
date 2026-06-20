__int128 sumRangeDivisors(__int128 x) {
    __int128 ans = 0, left = 1, right;
    for (; left <= x; left = right + 1) {
        right = x / (x / left);
        ans += (x / left) * (left + right) * (right - left + 1) / 2;
    }
    return ans;
}
void solve() {
    ll l,r;
    cin>>l>>r;
    __int128 ans = sumRangeDivisors(r) - sumRangeDivisors(l-1);
    ll tot = (ll)(ans);
    cout<<tot<<endl;
}
