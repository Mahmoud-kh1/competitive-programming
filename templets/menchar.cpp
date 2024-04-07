
// proceess the string and answer if (range l, r ) is palindorime or not 
vector<int> menacher(string &s) {
    string arr;
    for (int i = 0; i < s.size(); i++) {
        arr.push_back('#');
        arr.push_back(s[i]);
    }
    arr.push_back('#');
    vector<int> dp(arr.size());
    int left = 0;
    int right = 0;
    int lg_max = 0;
    int idx = 0;
    for (int i = 0; i < arr.size();) {
        while (left > 0 && right < arr.size() - 1 &&
               arr[left - 1] == arr[right + 1]) {
            left--;
            right++;
        }
        dp[i] = right - left + 1;
        if (lg_max < dp[i]) {
            lg_max = dp[i];
            idx = i;
        }
        int new_center = right + (i % 2 == 0 ? 1 : 0);
        for (int j = i + 1; j <= right; j++) {
            dp[j] = min(dp[i - (j - i)], 2 * (right - j) + 1);
            if (lg_max < dp[i]) {lg_max = dp[i]; idx = i; }
            if (j + dp[i - (j - i)] / 2 == right) {new_center = j; break; }
        }
        i = new_center;
        right = i + dp[i] / 2;
        left = i - dp[i] / 2;
    }
    return dp;
}
bool is_palindrome(int l , int r , vector<int> &dp){
    l *= 2; r *= 2; r += 2;
    return dp[(l + r) / 2] == (r - l + 1);
}
signed main() {
    HOKSHA();
    int t = 1;
    cin >> t;
    Z:
    while (t--) {
        string s; cin >> s;
      // proceses the string 
        vector<int>a = menacher(s);
        if (!is_palindrome(0, (int)s.size() - 1, a)){
            cout << "YES" <<endl <<  1 << endl << s << endl;
            goto Z;
        }
        for (int i = 0; i <(int)s.size() - 1; i++){
            if (!is_palindrome(0 ,i,a)&& !is_palindrome(i + 1, (int)s.size() - 1, a)){
                cout << "YES"  << endl << 2 << endl;
                cout << s.substr(0 , i + 1) << ' ' << s.substr(i + 1) << endl;
                goto Z;
            }
 
        }
        cout << "NO" << endl;
 
    }
    return 0;
}
