#include <bits/stdc++.h>
using namespace std;

class ManhattanDataStructure {
private:
    multiset<long long> setU, setV;
public:
    void insertPoint(long long x, long long y) {
        setU.insert(x + y);
        setV.insert(x - y);
    }
    bool deletePoint(long long x, long long y) {
        auto itU = setU.find(x + y);
        auto itV = setV.find(x - y);
        if (itU == setU.end() || itV == setV.end()) return false;
        setU.erase(itU);
        setV.erase(itV);
        return true;
    }
    long long queryMaxManhattan() {
        if(setU.empty() || setV.empty()) return 0;
        return max(*setU.rbegin() - *setU.begin(), *setV.rbegin() - *setV.begin());
    }
};

int main() {
    ManhattanDataStructure ds;

    // 1. Empty / Single point
    assert(ds.queryMaxManhattan() == 0);
    ds.insertPoint(5, 5);
    assert(ds.queryMaxManhattan() == 0);

    // 2. Standard distance
    ds.insertPoint(2, 1);
    // |5-2| + |5-1| = 3 + 4 = 7
    assert(ds.queryMaxManhattan() == 7); 

    // 3. Negative coordinates
    ds.insertPoint(-1, -2);
    // Max distance is between (5,5) and (-1,-2): |5 - (-1)| + |5 - (-2)| = 6 + 7 = 13
    assert(ds.queryMaxManhattan() == 13);

    // 4. Duplicate points
    ds.insertPoint(-1, -2);
    ds.deletePoint(-1, -2); 
    // One copy remains, max should still be 13
    assert(ds.queryMaxManhattan() == 13);

    // 5. Final deletion
    ds.deletePoint(-1, -2);
    // Now max is back to (5,5) and (2,1) -> 7
    assert(ds.queryMaxManhattan() == 7);

    cout << "All assertions passed!" << endl;
    return 0;
}
