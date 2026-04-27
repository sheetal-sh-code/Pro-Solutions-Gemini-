
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

const int MOD = 1e9 + 7;

// Memoization table: memo[idx][current_sum][is_tight]
// idx: up to 105 digits (since A < 10^100)
// current_sum: up to 1005
// is_tight: 0 or 1
int memo[105][1005][2];
string A;
int target_S;

// The Digit DP Recursive Function
int dp(int idx, int current_sum, bool is_tight) {
    // Base Case 1: If our sum exceeds the target, prune this branch
    if (current_sum > target_S) {
        return 0;
    }

    // Base Case 2: If we have placed all digits, check if the sum perfectly matches S
    if (idx == A.length()) {
        return (current_sum == target_S) ? 1 : 0;
    }

    // Return memoized result if already computed
    if (memo[idx][current_sum][is_tight] != -1) {
        return memo[idx][current_sum][is_tight];
    }

    // Determine the maximum digit we can place at this index.
    // If we are 'tight' (bound by the prefix of A), we cannot exceed A's digit here.
    // Otherwise, we are 'loose' and can place anything from 0 to 9.
    int limit = is_tight ? (A[idx] - '0') : 9;
    long long ways = 0;

    // Try placing all valid digits
    for (int d = 0; d <= limit; d++) {
        // We remain 'tight' ONLY if we were already tight AND we picked the maximum allowed digit
        bool next_tight = is_tight && (d == limit);

        ways = (ways + dp(idx + 1, current_sum + d, next_tight)) % MOD;
    }

    // Save and return the result
    return memo[idx][current_sum][is_tight] = ways;
}

void solve() {
    // Read string A and integer target_S
    if (!(cin >> A >> target_S)) return;

    // Edge Case / Pruning:
    // The maximum possible digit sum for a number of length L is 9 * L.
    // If target_S is greater than this, no valid number can exist.
    if (target_S > 9 * A.length()) {
        cout << 0 << "\n";
        return;
    }

    // Initialize memoization table with -1
    memset(memo, -1, sizeof(memo));

    // Start DP at index 0, current_sum = 0, and is_tight = true
    cout << dp(0, 0, true) << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
