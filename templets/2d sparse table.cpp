#include <bits/stdc++.h>
using namespace std;

static const int MOD = 1000000007;

/*
  2D Sparse‐Table Template for GCD over any axis‐aligned subrectangle.
  -------------------------------------------------------------------
  Precondition: n*m ≤ 10^6.  Grid A is 0‐indexed.
  Build: O(n·m·log n·log m) time and memory.
  Query(r1,c1,r2,c2): gcd over A[r1..r2][c1..c2] in O(1) by combining four
  precomputed 2^kx × 2^ky corners.

  Steps:
    1) Precompute lg2_row[i] = ⌊log2(i)⌋ for 1 ≤ i ≤ n, and lg2_col[j] = ⌊log2(j)⌋ for 1 ≤ j ≤ m.
    2) Allocate dp[kx][ky][i][j], where
         dp[kx][ky][i][j] = gcd of the submatrix whose top‐left corner is (i,j)
                              and whose size is (2^kx) × (2^ky).
       Valid for 0 ≤ i ≤ n−2^kx, 0 ≤ j ≤ m−2^ky.
    3) Build levels incrementally over kx, ky.
    4) To query any rectangle [r1..r2] × [c1..c2]:
         let dx = r2−r1+1, dy = c2−c1+1,
             kx = ⌊log2(dx)⌋, ky = ⌊log2(dy)⌋,
             i2 = r2 − 2^kx + 1, j2 = c2 − 2^ky + 1.
         The answer is
           gcd(
             gcd(dp[kx][ky][r1][c1], dp[kx][ky][i2][c1]),
             gcd(dp[kx][ky][r1][j2], dp[kx][ky][i2][j2])
           ).
*/

struct SparseTable2D {
    int n, m;
    vector<int> lg2_row, lg2_col;
    vector<vector<vector<vector<int>>>> dp;
    // dp[kx][ky][i][j]
    int combine(int a, int b) const {
        return gcd(a, b);
    }
    void build(const vector<vector<int>> &A) {
        n = (int)A.size();
        m = (int)A[0].size();

        // Precompute logs for rows and columns
        lg2_row.resize(n + 1);
        lg2_row[1] = 0;
        for (int i = 2; i <= n; i++) {
            lg2_row[i] = lg2_row[i >> 1] + 1;
        }
        lg2_col.resize(m + 1);
        lg2_col[1] = 0;
        for (int j = 2; j <= m; j++) {
            lg2_col[j] = lg2_col[j >> 1] + 1;
        }

        int Kx = lg2_row[n];
        int Ky = lg2_col[m];

        // Allocate dp with dimensions [Kx+1][Ky+1][n][m]
        dp.assign(Kx + 1,
                  vector<vector<vector<int>>>(Ky + 1,
                                              vector<vector<int>>(n, vector<int>(m))));

        // Base level kx=0, ky=0: submatrix 1×1 is just A[i][j]
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                dp[0][0][i][j] = A[i][j];
            }
        }

        // Build for ky > 0, kx = 0 (extending horizontally)
        for (int ky = 1; ky <= Ky; ky++) {
            int len_col = 1 << (ky - 1);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j + (1 << ky) - 1 < m; j++) {
                    dp[0][ky][i][j] = combine(
                            dp[0][ky - 1][i][j],
                            dp[0][ky - 1][i][j + len_col]
                    );
                }
            }
        }

        // Build for kx > 0
        for (int kx = 1; kx <= Kx; kx++) {
            int len_row = 1 << (kx - 1);
            // First, build ky = 0 (extending vertically)
            for (int i = 0; i + (1 << kx) - 1 < n; i++) {
                for (int j = 0; j < m; j++) {
                    dp[kx][0][i][j] = combine(
                            dp[kx - 1][0][i][j],
                            dp[kx - 1][0][i + len_row][j]
                    );
                }
            }
            // Then for ky > 0, combine four quadrants
            for (int ky = 1; ky <= Ky; ky++) {
                int len_col = 1 << (ky - 1);
                for (int i = 0; i + (1 << kx) - 1 < n; i++) {
                    for (int j = 0; j + (1 << ky) - 1 < m; j++) {
                        int g1 = dp[kx - 1][ky - 1][i][j];
                        int g2 = dp[kx - 1][ky - 1][i + len_row][j];
                        int g3 = dp[kx - 1][ky - 1][i][j + len_col];
                        int g4 = dp[kx - 1][ky - 1][i + len_row][j + len_col];
                        dp[kx][ky][i][j] = combine(
                                combine(g1, g2),
                                gcd(g3, g4)
                        );
                    }
                }
            }
        }
    }

    // Query gcd over [r1..r2] × [c1..c2], 0‐based inclusive.
    int query(int r1, int c1, int r2, int c2) const {
        int dx = r2 - r1 + 1;
        int dy = c2 - c1 + 1;
        int kx = lg2_row[dx];
        int ky = lg2_col[dy];

        int i2 = r2 - (1 << kx) + 1;
        int j2 = c2 - (1 << ky) + 1;

        int g1 = dp[kx][ky][r1][c1];
        int g2 = dp[kx][ky][i2][c1];
        int g3 = dp[kx][ky][r1][j2];
        int g4 = dp[kx][ky][i2][j2];

        return combine( combine(g1, g2), combine(g3, g4) );
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<vector<int>> A(n, vector<int>(m));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cin >> A[i][j];
            }
        }

        SparseTable2D st;
        st.build(A);

        int q;
        cin >> q;
        long long ans = 1;
        while (q--) {
            int r1, c1 , k;
            cin >> r1 >> c1;
            cin >> k;
            --r1; --c1;
            int g = st.query(r1, c1, r1 + k - 1, c1 + k -1);
            ans = (ans * g) % MOD;
        }
        cout << ans << "\n";
    }
    return 0;
}
