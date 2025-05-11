#include <iostream>
#include <vector>
#include <stack>
#include <functional>
using namespace std;
using ll = long long;

// Generic stack that maintains the combined value using a binary associative operation
template <typename T>
struct SlideStack {
    using Func = function<T(T, T)>;

    stack<pair<T, T>> st;
    Func combine;
    T identity;

    SlideStack(Func combine_op, T id) : combine(combine_op), identity(id) {}

    void push(T x) {
        T agg = st.empty() ? x : combine(x, st.top().second);
        st.emplace(x, agg);
    }

    void pop() {
        st.pop();
    }

    T get() const {
        return st.empty() ? identity : st.top().second;
    }

    bool empty() const {
        return st.empty();
    }

    void clear() {
        while (!st.empty()) st.pop();
    }
};

// Two stacks to simulate a queue with O(1) get-aggregate
template <typename T>
struct SlideQueue {
    SlideStack<T> in_stack, out_stack;
    using Func = function<T(T, T)>;
    Func combine;
    T identity;

    SlideQueue(Func combine_op, T id)
            : in_stack(combine_op, id), out_stack(combine_op, id),
              combine(combine_op), identity(id) {}

    void push(T x) {
        in_stack.push(x);
    }

    void pop() {
        if (out_stack.empty()) {
            while (!in_stack.empty()) {
                T val = in_stack.st.top().first;
                in_stack.pop();
                out_stack.push(val);
            }
        }
        out_stack.pop();
    }

    T get() const {
        return combine(in_stack.get(), out_stack.get());
    }

    int size() const {
        return in_stack.st.size() + out_stack.st.size();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    ll x, a, b, c;
    cin >> n >> k >> x >> a >> b >> c;

    

    // for expample minimum   
    auto opeartion = [](ll a, ll b) { return min(a, b); };
  // here dont forget to change 1e9 + 7 to the iddenty we solve if max make it negetive , if or make 0, if xor make it 0 , if and make it -1 and like this for gcd make it 0 and like this 
    SlideQueue<ll> window(opeartion, 1e9  + 7);

    ll ans = 0;
     //  time is O(n) * time of opeartoin itslef like min (1) , gcd log 
  for (int i = 0; i < n; ++i) {
        window.push(x);
        if (window.size() > k) window.pop();
        if (window.size() == k) {
            ans ^= window.get();
        }

        // Generate next value
        x = (a * x + b) % c;
    }

    cout << ans << '\n';
    return 0;
}

