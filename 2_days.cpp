#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure to hold our item properties
struct Item {
    long long a, b;
};

void solve() {
    int n;
    long long k;
    // Read N and K. If EOF, return safely.
    if (!(cin >> n >> k)) return;

    vector<Item> items(n);
    long long sum_a = 0, sum_b = 0;

    // Read array A and compute worst-case initial sum
    for (int i = 0; i < n; i++) {
        cin >> items[i].a;
        sum_a += items[i].a;
    }

    // Read array B and compute worst-case daily inflow sum
    for (int i = 0; i < n; i++) {
        cin >> items[i].b;
        sum_b += items[i].b;
    }

    // Sort items in ascending order of their daily inflow (B)
    sort(items.begin(), items.end(), [](const Item& x, const Item& y) {
        return x.b < y.b;
    });

    // Initialize Space-Optimized 1D DP
    // dp[j] stores the maximum savings if we pick exactly j items to clear.
    vector<long long> dp(n + 1, -1e18);
    dp[0] = 0;

    // Populate the DP
    for (int i = 0; i < n; i++) {
        // Traverse backwards to safely update the 1D DP without using the same item twice
        for (int j = i + 1; j >= 1; j--) {
            if (dp[j - 1] != -1e18) {
                // The j-th item picked gets a multiplier of j
                long long pick_savings = dp[j - 1] + items[i].a + (long long)j * items[i].b;
                dp[j] = max(dp[j], pick_savings);
            }
        }
    }

    // Find the minimum days required
    int ans = -1;
    for (int m = 1; m <= n; m++) {
        // Final stock = (Total A + m * Total B) - Max Savings for choosing m items
        long long final_stock = sum_a + (long long)m * sum_b - dp[m];

        if (final_stock <= k) {
            ans = m;
            break;
        }
    }

    cout << ans << "\n";
}

int main() {
    // Fast I/O for Competitive Programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
