using ll = long long;
 
struct LinearDiophantine {
    ll A, B, C;
    ll g;
    ll x0, y0;
 
    LinearDiophantine(ll a, ll b, ll c)
            : A(a), B(b), C(c)
    {
        g = extGcd(llabs(A), llabs(B), x0, y0);
        if (A < 0) x0 = -x0;
        if (B < 0) y0 = -y0;
    }
 
    static ll extGcd(ll a, ll b, ll &x, ll &y) {
        if (b == 0) {
            x = 1; y = 0;
            return a;
        }
        ll x1, y1;
        ll d = extGcd(b, a % b, x1, y1);
        x = y1;
        y = x1 - (a / b) * y1;
        return d;
    }
 
    bool hasSolution() const {
        return (C % g) == 0;
    }
 
    pair<ll,ll> getOneSolution() const {
        if (!hasSolution()) return {0,0};
        ll scale = C / g;
        return { x0 * scale, y0 * scale };
    }
 
    pair<ll,ll> getGeneralSolution(ll t) const {
        auto [xp, yp] = getOneSolution();
        ll A_ = A/g, B_ = B/g;
        return { xp + B_*t, yp - A_*t };
    }
 
    ll countStrictPositiveSolutions() const {
        if (!hasSolution()) return 0;
        auto [xp, yp] = getOneSolution();
        ll A_ = A/g, B_ = B/g;
        long double t_lo =  ( - (long double)xp ) / B_;
        long double t_hi =    (  (long double)yp ) / A_;
        ll t_min = (ll)floor(t_lo) + 1;
        ll t_max = (ll)ceil(t_hi)  - 1;
        if (t_max < t_min) return 0;
        return t_max - t_min + 1;
    }
 
    ll countNonNegativeSolutions() const {
        if (!hasSolution()) return 0;
        auto [xp, yp] = getOneSolution();
        ll A_ = A/g, B_ = B/g;
        long double t_lo =  ( - (long double)xp ) / B_;
        long double t_hi =    (  (long double)yp ) / A_;
        ll t_min = (ll)ceil(t_lo);
        ll t_max = (ll)floor(t_hi);
        if (t_max < t_min) return 0;
        return t_max - t_min + 1;
    }
 
    vector<pair<ll,ll>> getAllStrictPositiveSolutions() const {
        vector<pair<ll,ll>> sol;
        if (!hasSolution()) return sol;
        auto [xp, yp] = getOneSolution();
        ll A_ = A/g, B_ = B/g;
        long double t_lo =  ( - (long double)xp ) / B_;
        long double t_hi =    (  (long double)yp ) / A_;
        ll t_min = (ll)floor(t_lo) + 1;
        ll t_max = (ll)ceil(t_hi)  - 1;
        for (ll t = t_min; t <= t_max; ++t)
            sol.push_back(getGeneralSolution(t));
        return sol;
    }
}; A,B   C   A * x + B * y = C
