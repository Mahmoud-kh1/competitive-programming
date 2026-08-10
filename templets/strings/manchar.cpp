struct Manacher {
    vector<int> p[2];
    Manacher(string s) {
        int n = s.size();
        p[0].resize(n + 1);
        p[1].resize(n);
        for (int z = 0; z < 2; z++) {
            for (int i = 0, l = 0, r = 0; i < n; i++) {
                int t = r - i + !z;
                if (i < r) p[z][i] = min(t, p[z][l + t]);
                int L = i - p[z][i], R = i + p[z][i] - !z;
                while (L >= 1 && R + 1 < n && s[L - 1] == s[R + 1]) {
                    p[z][i]++; 
                    L--; 
                    R++;
                }
                if (R > r) { l = L; r = R; }
            }
        }
    }
    // longest plaindrome where center is center , odd for odd
   int GetLongest(int center, bool odd = true) {
        if (odd) {
            // Odd length: centered exactly at s[center]
            // p[1][center] is the radius. Total length = 2 * radius + 1
            return 2 * p[1][center] + 1;
        } else {
            // Even length: centered between s[center] and s[center + 1]
            // In this specific template, the even palindrome for that gap is stored at index center + 1
            // p[0][center + 1] is half the length. Total length = 2 * half_length
            return 2 * p[0][center + 1];
        }
    }
  
    bool isPalindrome(int l, int r) {
        int mid = (l + r + 1) / 2;
        int len = r - l + 1;
        return 2 * p[len % 2][mid] + len % 2 >= len;
    }
};
