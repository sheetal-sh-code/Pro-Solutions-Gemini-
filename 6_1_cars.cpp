#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void solve() {
    int n;
    // Read the number of cars
    if (!(cin >> n)) return;

    vector<long long> x(n), y(n);
    for (int i = 0; i < n; i++) {
        cin >> x[i] >> y[i];
    }

    // If there is only one car, 0 drives are required.
    if (n == 1) {
        cout << 0 << "\n";
        return;
    }

    // Safely calculate mathematical modulo for potentially negative coordinates
    auto get_parity = [](long long val) {
        return ((val % 2) + 2) % 2;
    };

    // Step 1: The Parity Check
    // All cars must have the same initial (x + y) parity.
    long long required_parity = get_parity(x[0] + y[0]);
    for (int i = 1; i < n; i++) {
        if (get_parity(x[i] + y[i]) != required_parity) {
            cout << -1 << "\n";
            return;
        }
    }

    // Step 2: 45-Degree Coordinate Transformation (u = x+y, v = x-y)
    long long u_min = x[0] + y[0], u_max = x[0] + y[0];
    long long v_min = x[0] - y[0], v_max = x[0] - y[0];

    for (int i = 1; i < n; i++) {
        long long u = x[i] + y[i];
        long long v = x[i] - y[i];

        u_min = min(u_min, u);
        u_max = max(u_max, u);
        v_min = min(v_min, v);
        v_max = max(v_max, v);
    }

    // Step 3: Find the maximum radial distance needed in either axis
    // (Integer ceiling of half the maximum spread)
    long long R_u = (u_max - u_min + 1) / 2;
    long long R_v = (v_max - v_min + 1) / 2;
    long long R = max(R_u, R_v);

    // Step 4: Binary Search for the minimum drives K
    long long low = 0, high = 2e9; // 2e9 safely bounds sqrt(4 * 10^17)
    long long ans = high;

    while (low <= high) {
        long long mid = low + (high - low) / 2;
        // Total distance a car can cover in 'mid' drives is sum of 1..mid
        long long max_reach = mid * (mid + 1) / 2;

        if (max_reach >= R) {
            ans = mid;
            high = mid - 1; // Try to find a smaller valid K
        } else {
            low = mid + 1; // K is too small, increase it
        }
    }

    cout << ans << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Uncomment the loop below if multiple test cases exist in the platform
    // int t; cin >> t; while (t--) solve();

    solve();

    return 0;
}
