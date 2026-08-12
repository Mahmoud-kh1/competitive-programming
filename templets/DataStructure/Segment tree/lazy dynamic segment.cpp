struct Vertex {
    ll left, right;
    ll sum = 0;
    ll lazy = 0;
    Vertex *left_child = nullptr, *right_child = nullptr;

    Vertex(ll lb, ll rb) {
        left = lb;
        right = rb;
    }

    void extend() {
        if (!left_child && left + 1 < right) {
            ll t = (left + right) / 2;
            left_child = new Vertex(left, t);
            right_child = new Vertex(t, right);
        }
    }

    void add(ll lq, ll rq, ll x) {
        if (lq <= left && right <= rq) {
            sum += x * (right - left);
            lazy += x;
            return;
        }
        if (max(left, lq) >= min(right, rq))
            return;

        extend();
        if (lazy != 0 && left_child) {
            left_child->sum += lazy * (left_child->right - left_child->left);
            left_child->lazy += lazy;
            right_child->sum += lazy * (right_child->right - right_child->left);
            right_child->lazy += lazy;
            lazy = 0;
        }

        if (left_child) {
            left_child->add(lq, rq, x);
            right_child->add(lq, rq, x);
            sum = left_child->sum + right_child->sum;
        }
    }

    ll get_sum(ll lq, ll rq) {
        if (lq <= left && right <= rq)
            return sum;
        if (max(left, lq) >= min(right, rq))
            return 0;

        extend();
        if (lazy != 0 && left_child) {
            left_child->sum += lazy * (left_child->right - left_child->left);
            left_child->lazy += lazy;
            right_child->sum += lazy * (right_child->right - right_child->left);
            right_child->lazy += lazy;
            lazy = 0;
        }

        if (left_child) {
            return left_child->get_sum(lq, rq) + right_child->get_sum(lq, rq);
        }
        return 0;
    }
};
