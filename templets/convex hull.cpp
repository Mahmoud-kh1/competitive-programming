#include <bits/stdc++.h>
using namespace std;

using ll = long long;
struct Pt { int x, y; };

static const double EPS = 1e-9;

ll cross(const Pt &a, const Pt &b, const Pt &c) {
    return ll(b.x - a.x)*(c.y - a.y)
         - ll(b.y - a.y)*(c.x - a.x);
}

vector<Pt> convex_hull(vector<Pt> a) {
    int n = a.size();
    if (n <= 1) return a;
    sort(a.begin(), a.end(), [](const Pt &A, const Pt &B){
        return A.x < B.x || (A.x == B.x && A.y < B.y);
    });
    vector<Pt> H;
    for (int i = 0; i < n; ++i) {
        while (H.size() >= 2 && cross(H[H.size()-2], H.back(), a[i]) <= 0)
            H.pop_back();
        H.push_back(a[i]);
    }
    int lower = H.size();
    for (int i = n - 2; i >= 0; --i) {
        while ((int)H.size() >= lower + 1 && cross(H[H.size()-2], H.back(), a[i]) <= 0)
            H.pop_back();
        H.push_back(a[i]);
    }
    H.pop_back();
    return H;
}

bool onSegment(const Pt &a, const Pt &b, const Pt &p) {
    if (cross(a,b,p)!=0) return false;
    return min(a.x,b.x) <= p.x && p.x <= max(a.x,b.x)
        && min(a.y,b.y) <= p.y && p.y <= max(a.y,b.y);
}

bool inPolygon(const vector<Pt> &poly, const Pt &p) {
    bool inside = false;
    int n = poly.size();
    for (int i = 0, j = n-1; i < n; j = i++) {
        const Pt &A = poly[i], &B = poly[j];
        if (onSegment(A,B,p)) return true;
        bool yi = (A.y > p.y), yj = (B.y > p.y);
        if (yi != yj) {
            double xint = double(B.x - A.x)*(p.y - A.y)/(B.y - A.y) + A.x;
            if (xint > p.x) inside = !inside;
        }
    }
    return inside;
}

double areaPolygon(const vector<Pt> &P) {
    int n = P.size();
    ll sum = 0;
    for (int i = 0; i < n; i++) {
        int j = (i+1)%n;
        sum += ll(P[i].x)*P[j].y - ll(P[j].x)*P[i].y;
    }
    return fabs(sum) * 0.5;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    struct K { vector<Pt> hull; double area; bool dead; };
    vector<K> kingdoms;

    while (true) {
        int N;
        if (!(cin >> N) || N < 0) break;
        vector<Pt> pts(N);
        for (int i = 0; i < N; i++) {
            cin >> pts[i].x >> pts[i].y;
        }
        vector<Pt> H = convex_hull(pts);
        double A = areaPolygon(H);
        kingdoms.push_back(K{H, A, false});
    }

    Pt m;
    while (cin >> m.x >> m.y) {
        for (int i = 0; i < (int)kingdoms.size(); ++i) {
            if (!kingdoms[i].dead && inPolygon(kingdoms[i].hull, m)) {
                kingdoms[i].dead = true;
                break;
            }
        }
    }

    double ans = 0;
    for (int i = 0; i < (int)kingdoms.size(); ++i)
        if (kingdoms[i].dead)
            ans += kingdoms[i].area;

    cout << fixed << setprecision(2) << ans << "\n";
    return 0;
}
