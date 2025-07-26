
template<typename T>
struct OrderedMultiset {
    using pii = pair<T, int>;
    tree<pii, null_type, less<pii>, rb_tree_tag, tree_order_statistics_node_update> t;
    int index = 0;
 
    void insert(T x) {
        t.insert({x, index++});
    }
 
    void erase(T x) {
        auto it = t.lower_bound({x, 0});
        if (it != t.end() && it->first == x) {
            t.erase(it);
        }
    }
 
    int order_of_key(T x) {
        return t.order_of_key({x, 0});
    }
 
    T find_by_order(int k) {
        if (k >= (int) t.size()) return -1;
        return t.find_by_order(k)->first;
    }
 
    int size() {
        return t.size();
    }
 
    void clear() {
        t.clear();
        index = 0;
    }
 
    // Adding iterator support so that we can iterate over elements
    auto begin() const { return t.begin(); }
    auto end() const { return t.end(); }
};
