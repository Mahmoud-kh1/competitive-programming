#include <vector>
using namespace std;

struct Prefix3D {
    int n, m, p;
    vector<vector<vector<long long>>> pre;

    // Initialize with dimensions (x, y, z)
    Prefix3D(int x, int y, int z) : n(x), m(y), p(z), 
        pre(x+1, vector<vector<long long>>(y+1, vector<long long>(z+1, 0))) {}

    // Build prefix sum from 3D array (1-indexed)
    void build(const vector<vector<vector<long long>>> &arr) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                for (int k = 1; k <= p; k++) {
                    pre[i][j][k] = arr[i][j][k]
                        + pre[i-1][j][k] 
                        + pre[i][j-1][k]
                        + pre[i][j][k-1]
                        - pre[i-1][j-1][k]
                        - pre[i-1][j][k-1]
                        - pre[i][j-1][k-1]
                        + pre[i-1][j-1][k-1];
                }
            }
        }
    }

    // Query sum in cuboid [x1, x2] x [y1, y2] x [z1, z2] (1-indexed inclusive)
    long long query(int x1, int x2, int y1, int y2, int z1, int z2) {
        return pre[x2][y2][z2]
            - pre[x1-1][y2][z2] 
            - pre[x2][y1-1][z2]
            - pre[x2][y2][z1-1]
            + pre[x1-1][y1-1][z2]
            + pre[x1-1][y2][z1-1]
            + pre[x2][y1-1][z1-1]
            - pre[x1-1][y1-1][z1-1];
    }
};
