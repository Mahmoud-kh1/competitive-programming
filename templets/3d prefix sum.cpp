#include <bits/stdc++.h>
using namespace std;

void M() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}

#define int long long

signed main() {
    M();

    int N;
    cin >> N;

    vector<vector<vector<int>>> A(N + 1, vector<vector<int>>(N + 1, vector<int>(N + 1)));
    vector<vector<vector<int>>> prefix(N + 1, vector<vector<int>>(N + 1, vector<int>(N + 1, 0)));
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            for (int k = 1; k <= N; k++) {
                cin >> A[i][j][k];
            }
        }
    }
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            for (int k = 1; k <= N; k++) {
                prefix[i][j][k] = A[i][j][k]
                                  + (i > 1 ? prefix[i-1][j][k] : 0)
                                  + (j > 1 ? prefix[i][j-1][k] : 0)
                                  + (k > 1 ? prefix[i][j][k-1] : 0)
                                  - (i > 1 && j > 1 ? prefix[i-1][j-1][k] : 0)
                                  - (i > 1 && k > 1 ? prefix[i-1][j][k-1] : 0)
                                  - (j > 1 && k > 1 ? prefix[i][j-1][k-1] : 0)
                                  + (i > 1 && j > 1 && k > 1 ? prefix[i-1][j-1][k-1] : 0);
            }
        }
    }

    int Q;
    cin >> Q;

    // Process each query
    while (Q--) {
        int Lx, Rx, Ly, Ry, Lz, Rz;
        cin >> Lx >> Rx >> Ly >> Ry >> Lz >> Rz;

        int result = prefix[Rx][Ry][Rz]
                     - (Lx > 1 ? prefix[Lx-1][Ry][Rz] : 0)
                     - (Ly > 1 ? prefix[Rx][Ly-1][Rz] : 0)
                     - (Lz > 1 ? prefix[Rx][Ry][Lz-1] : 0)
                     + (Lx > 1 && Ly > 1 ? prefix[Lx-1][Ly-1][Rz] : 0)
                     + (Lx > 1 && Lz > 1 ? prefix[Lx-1][Ry][Lz-1] : 0)
                     + (Ly > 1 && Lz > 1 ? prefix[Rx][Ly-1][Lz-1] : 0)
                     - (Lx > 1 && Ly > 1 && Lz > 1 ? prefix[Lx-1][Ly-1][Lz-1] : 0);

        cout << result << endl;
    }

    return 0;
}
