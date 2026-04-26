#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

void solve() {
    int n;
    if (!(cin >> n)) return;

    // cost[i][0] = cost when removed with 0 neighbors
    // cost[i][1] = cost when removed with 1 neighbor
    // cost[i][2] = cost when removed with 2 neighbors
    vector<vector<long long>> cost(n + 1, vector<long long>(3));
    for (int i = 1; i <= n; i++) {
        cin >> cost[i][0] >> cost[i][1] >> cost[i][2];
    }

    // dp[i][j] stores the min cost to remove all stones strictly in the subsegment [i, j]
    vector<vector<long long>> dp(n + 2, vector<long long>(n + 2, 0));

    // Interval DP: Build answers for smaller lengths first
    for (int len = 1; len <= n; len++) {
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;

            // Determine how many neighbors are CURRENTLY present immediately outside [i, j].
            // Because everything inside [i, j] will be removed first, the absolute LAST
            // stone removed in this interval will only touch these stable outer bounds.
            int outside_neighbors = 0;
            if (i > 1) outside_neighbors++;
            if (j < n) outside_neighbors++;

            dp[i][j] = INF;

            // Try picking each stone 'k' to be the absolute LAST stone removed in [i, j]
            for (int k = i; k <= j; k++) {
                long long current_cost = dp[i][k - 1] + dp[k + 1][j] + cost[k][outside_neighbors];
                dp[i][j] = min(dp[i][j], current_cost);
            }
        }
    }

    // The overall minimum cost to remove the entire interval from 1 to N
    cout << dp[1][n] << "\n";
}

int main() {
    // Fast I/O for Competitive Programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
