/* Priyansh Agarwal*/

#include<bits/stdc++.h>

#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;
// make it less_equal to make it multiset 
typedef tree<int, null_type, less_equal<int>, rb_tree_tag, tree_order_statistics_node_update> pbds; // find_by_order, order_of_key

int main() {
	pbds A; // declaration

	// Inserting elements - 1st query
	A.insert(1);
	A.insert(10);
	A.insert(2);
	A.insert(7);
	A.insert(2);  // ordered set only contains unique values

	// A contains
	cout << "A = ";
	for (auto i : A)
		cout << i << " ";
	cout << endl;
	cout << endl;

	// finding kth element - 4th query
	cout << "0th element: " << *A.find_by_order(0) << endl;
	cout << "1st element: " << *A.find_by_order(1) << endl;
	cout << "2nd element: " << *A.find_by_order(2) << endl;
	cout << "3rd element: " << *A.find_by_order(3) << endl;
	cout << endl;

	// finding number of elements smaller than X - 3rd query
	cout << "No. of elems smaller than 6: " << A.order_of_key(6) << endl; // 2
	cout << "No. of elems smaller than 11: " << A.order_of_key(11) << endl; // 4
	cout << "No. of elems smaller than 1: " << A.order_of_key(1) << endl; // 0
	cout << endl;

	// lower bound -> Lower Bound of X = first element >= X in the set
	cout << "Lower Bound of 6: " << *A.lower_bound(6) << endl;
	cout << "Lower Bound of 2: " << *A.lower_bound(2) << endl;
	cout << endl;

	// Upper bound -> Upper Bound of X = first element > X in the set
	cout << "Upper Bound of 6: " << *A.upper_bound(6) << endl;
	cout << "Upper Bound of 1: " << *A.upper_bound(1) << endl;
	cout << endl;

	// // Remove elements - 2nd query
  // to erasse x 
	 auto it = s.find_by_order(s.order_of_key(X));
        if (it != s.end()) {
            s.erase(it);
        }

	// A contains
	cout << "A = ";
	for (auto i : A)
		cout << i << " ";
	cout << endl;

}



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
