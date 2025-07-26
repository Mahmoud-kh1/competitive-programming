#include <vector>
using namespace std;

struct Partial3D {
    int n, m, p;
    vector<vector<vector<long long>>> diff;

    // Initialize with dimensions (x, y, z)
    Partial3D(int x, int y, int z) : n(x), m(y), p(z), 
        diff(x+2, vector<vector<long long>>(y+2, vector<long long>(z+2, 0))) {}

    // Add value to cuboid [x1, x2] x [y1, y2] x [z1, z2] (1-indexed inclusive)
    void update(int x1, int x2, int y1, int y2, int z1, int z2, long long val) {
        diff[x1][y1][z1] += val;
        diff[x2+1][y1][z1] -= val;
        diff[x1][y2+1][z1] -= val;
        diff[x1][y1][z2+1] -= val;
        diff[x1][y2+1][z2+1] += val;
        diff[x2+1][y1][z2+1] += val;
        diff[x2+1][y2+1][z1] += val;
        diff[x2+1][y2+1][z2+1] -= val;
    }

    // Convert difference array to final 3D array
    vector<vector<vector<long long>>> finalize() {
        vector<vector<vector<long long>>> arr(n+1, 
            vector<vector<long long>>(m+1, vector<long long>(p+1, 0)));
        
        // Apply inclusion-exclusion
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                for (int k = 1; k <= p; k++) {
                    arr[i][j][k] = diff[i][j][k]
                        + arr[i-1][j][k] 
                        + arr[i][j-1][k]
                        + arr[i][j][k-1]
                        - arr[i-1][j-1][k]
                        - arr[i-1][j][k-1]
                        - arr[i][j-1][k-1]
                        + arr[i-1][j-1][k-1];
                }
            }
        }
        return arr;
    }
};
