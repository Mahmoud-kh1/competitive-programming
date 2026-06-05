using int64 = long long;

// 计算 sum of floor((a * i + b) / m) from i 0 to n - 1 , if you want start with i =1 , then pass n = n , m = m, b = a + b, a = a
int64 floor_sum(int64 n, int64 m, int64 a, int64 b) {
    int64 ans = 0;

    while (true) {
        if (a >= m) {
            ans += (n - 1) * n * (a / m) / 2;
            a %= m;
        }
        if (b >= m) {
            ans += n * (b / m);
            b %= m;
        }

        int64 y_max = a * n + b;
        if (y_max < m) break;

        n = y_max / m;
        b = y_max % m;
        swap(a, m);
    }

    
