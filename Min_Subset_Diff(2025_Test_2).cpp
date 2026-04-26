#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

using namespace std;

void solve() {
    int n;
    // Read the number of cargoes
    if (!(cin >> n)) return;

    vector<int> l(n);
    long long total_sum = 0;

    for (int i = 0; i < n; i++) {
        cin >> l[i];
        total_sum += l[i];
    }

    // Edge case: No cargoes
    if (n == 0) {
        cout << 0 << "\n";
        return;
    }

    // We want to find a subset of cargoes whose sum is as close to total_sum / 2 as possible.
    long long target = total_sum / 2;

    // dp[j] will be true if a subset with sum 'j' is achievable.
    // We only need to compute up to 'target' because any subset larger than target
    // means the OTHER subset is smaller than target.
    vector<bool> dp(target + 1, false);

    // A sum of 0 is always achievable (by picking no cargoes)
    dp[0] = true;

    // Standard 0/1 Knapsack 1D DP transition
    for (int i = 0; i < n; i++) {
        int weight = l[i];

        // We traverse backwards from 'target' down to 'weight'.
        // Traversing backwards ensures we don't accidentally use the same cargo twice.
        for (long long j = target; j >= weight; j--) {
            // If we could make a sum of (j - weight) previously,
            // we can now make a sum of 'j' by adding the current cargo.
            if (dp[j - weight]) {
                dp[j] = true;
            }
        }
    }

    // Find the largest achievable sum for Compartment A that is <= target
    long long weight_A = 0;
    for (long long j = target; j >= 0; j--) {
        if (dp[j]) {
            weight_A = j;
            break;
        }
    }

    // Compartment B gets whatever is left over
    long long weight_B = total_sum - weight_A;

    // The absolute difference between the two compartments
    cout << abs(weight_B - weight_A) << "\n";
}

int main() {
    // Fast I/O for Competitive Programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Uncomment the loop below if the platform provides multiple test cases (T)
    // int t; if (cin >> t) { while (t--) solve(); }
    solve();

    return 0;
}
