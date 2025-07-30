#include <bits/stdc++.h>

using namespace std;

void fileIO(void) {
#ifndef ONLINE_JUDGE

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

#endif
}
void fastIO(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
}

typedef long long ll;
#define int ll
#define double long double

const ll is_query = -(1LL<<62);
struct line {
    double m, b;
    mutable function<const line*()> succ;
    bool operator<(const line& rhs) const {
        if (rhs.b != is_query) return m < rhs.m;
        const line* s = succ();
        if (!s) return 0;
        double x = rhs.m;
        return b - s->b < (s->m - m) * x;
    }
};

struct dynamic_hull : public multiset<line> {
    const ll inf = LLONG_MAX;
    bool bad(iterator y) {
        auto z = next(y);
        if (y == begin()) {
            if (z == end()) return 0;
            return y->m == z->m && y->b <= z->b;
        }
        auto x = prev(y);
        if (z == end()) return y->m == x->m && y->b <= x->b;

        /* compare two lines by slope, make sure denominator is not 0 */
        double v1 = (x->b - y->b);
        if (y->m == x->m) v1 = x->b > y->b ? inf : -inf;
        else v1 /= (y->m - x->m);
        double v2 = (y->b - z->b);
        if (z->m == y->m) v2 = y->b > z->b ? inf : -inf;
        else v2 /= (z->m - y->m);
        return v1 >= v2;
    }
    void insert_line(double m, double b) {
        auto y = insert({ m, b });
        y->succ = [=] { return next(y) == end() ? 0 : &*next(y); };
        if (bad(y)) { erase(y); return; }
        while (next(y) != end() && bad(next(y))) erase(next(y));
        while (y != begin() && bad(prev(y))) erase(prev(y));
    }
    double eval(double x) {
        auto l = *lower_bound((line) { x, is_query });
        return l.m * x + l.b;
    }
};

struct pt {
    int x, y;
};
void solve(int tc) {
    int n , q;
    cin >> n >> q;
    vector<pt> points(n);

    dynamic_hull mx, mn;
    int mnx = LLONG_MAX, mxx = LLONG_MIN;
    for (int i = 0 ;i < n;i++) {
        cin >> points[i].x >> points[i].y;
        mx.insert_line(points[i].x, points[i].y);
        mn.insert_line(-points[i].x, -points[i].y);
        mnx = min(mnx, points[i].x);
        mxx = max(mxx, points[i].x);
    }

    for (int i = 0 ;i < q;i++){
        int a , b , c;
        cin >> a >> b >> c;

        if(b == 0) {
            cout << max(abs(mxx + c / (double)a), abs(mnx + c / (double)a)) << "\n";
            continue;
        }

        double ev1 = mx.eval((double)a/b) * b + c;
        double ev2 = -mn.eval((double)a/b) * b + c;

        double ans = max(abs(ev1), abs(ev2)) / sqrtl(a*a + b*b);
        cout << ans << "\n";
    }
}

signed main() {

    fastIO(); fileIO();
    cout << setprecision(10) << fixed;
    int t = 1; //cin >> t;

    for (int i = 1; i <= t; ++i)
        solve(i);

    return 0;
}
