
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void solve() {
    int n, m;
    // Read sizes of array A and B
    if (!(cin >> n >> m)) return;

    vector<long long> a(n), b(m);
    vector<long long> candidates;

    // D can be 0 (meaning all elements get score 2)
    candidates.push_back(0);

    for (int i = 0; i < n; i++) {
        cin >> a[i];
        candidates.push_back(a[i]);
    }
    for (int i = 0; i < m; i++) {
        cin >> b[i];
        candidates.push_back(b[i]);
    }

    // Sort arrays to enable O(log N) binary search lookups
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    // Coordinate Compression: Remove duplicate candidate values
    // to avoid redundant calculations
    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());

    long long max_diff = -1e18;
    long long best_d = 0;
    long long max_score_a = -1e18; // Secondary tie-breaker

    for (long long d : candidates) {
        // upper_bound gives the number of elements strictly less than or equal to 'd'
        long long count_a = upper_bound(a.begin(), a.end(), d) - a.begin();
        long long count_b = upper_bound(b.begin(), b.end(), d) - b.begin();

        // Calculate scores based on the rule
        // elements <= D get 1 point, elements > D get 2 points
        long long score_a = count_a * 1 + (n - count_a) * 2;
        long long score_b = count_b * 1 + (m - count_b) * 2;

        long long diff = score_a - score_b;

        // If we find a strictly better difference, update.
        // If the difference is the same, pick the one that gives A a higher total score.
        if (diff > max_diff || (diff == max_diff && score_a > max_score_a)) {
            max_diff = diff;
            best_d = d;
            max_score_a = score_a;
        }
    }

    cout << best_d << "\n";
}

int main() {
    // Fast I/O for Competitive Programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
