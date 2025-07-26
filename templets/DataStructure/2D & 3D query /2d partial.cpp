#include <vector>
using namespace std;

struct Partial2D {
    int n, m;
    vector<vector<long long>> diff;

    // Initialize with dimensions (rows, columns)
    Partial2D(int x, int y) : n(x), m(y), 
        diff(x+2, vector<long long>(y+2, 0)) {}

    // Add value to rectangle [x1, x2] x [y1, y2] (1-indexed inclusive)
    void update(int x1, int x2, int y1, int y2, long long val) {
        diff[x1][y1] += val;
        diff[x1][y2+1] -= val;
        diff[x2+1][y1] -= val;
        diff[x2+1][y2+1] += val;
    }

    // Convert difference array to final 2D array
    vector<vector<long long>> finalize() {
        vector<vector<long long>> arr(n+1, vector<long long>(m+1, 0));
        
        // Compute 2D prefix sum
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                arr[i][j] = diff[i][j]
                    + arr[i-1][j]
                    + arr[i][j-1]
                    - arr[i-1][j-1];
            }
        }
        return arr;
    }
};
