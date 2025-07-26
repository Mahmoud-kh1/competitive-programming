struct VeniceSet {
    multiset<long long> ms;
    long long delta = 0; // global offset

    void insert(long long x) {
        ms.insert(x - delta);  // store adjusted value
    }

    void remove(long long x) {
        auto it = ms.find(x - delta);
        if (it != ms.end()) ms.erase(it);
    }

    void add_all(long long v) {
        delta += v;  // logically add v to all elements
    }

    long long get_min() {
        if (ms.empty()) return LLONG_MAX;
        return *ms.begin() + delta;
    }

    long long get_max() {
        if (ms.empty()) return LLONG_MIN;
        return *ms.rbegin() + delta;
    }

    int size() const {
        return ms.size();
    }
};
