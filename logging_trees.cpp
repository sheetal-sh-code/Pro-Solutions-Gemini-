#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

void solve() {
    int n, m;
    // Assuming standard competitive programming input:
    // Read N (road length) and M (total number of trees)
    if (!(cin >> n >> m)) return;

    // A map to automatically group tree locations by their lengths.
    // 'greater<int>' ensures we process lengths in strictly descending order.
    // We only need to store the minimum (leftmost) and maximum (rightmost)
    // positions for each tree length.
    map<int, pair<int, int>, greater<int>> groups;

    for (int i = 0; i < m; ++i) {
        int pos, length;
        // Assuming input gives the position and length of each tree
        cin >> pos >> length;

        if (groups.find(length) == groups.end()) {
            groups[length] = {pos, pos};
        } else {
            groups[length].first = min(groups[length].first, pos);
            groups[length].second = max(groups[length].second, pos);
        }
    }

    // DP State Initialization
    // pos0 / pos1: The ending position of the robot after the previous group
    // cost0 / cost1: The cumulative travel time to end up at pos0 / pos1
    long long pos0 = 0, pos1 = 0;
    long long cost0 = 0, cost1 = 0;

    for (auto const& [len, bounds] : groups) {
        long long L = bounds.first;  // Leftmost tree of this length
        long long R = bounds.second; // Rightmost tree of this length

        // Option 0: To end up at L, we MUST visit R first.
        // We calculate the cost coming from either 'pos0' or 'pos1'.
        long long next_cost0 = min(cost0 + abs(pos0 - R), cost1 + abs(pos1 - R)) + (R - L);

        // Option 1: To end up at R, we MUST visit L first.
        long long next_cost1 = min(cost0 + abs(pos0 - L), cost1 + abs(pos1 - L)) + (R - L);

        // Advance the state for the next length group
        pos0 = L;
        pos1 = R;
        cost0 = next_cost0;
        cost1 = next_cost1;
    }

    // Final movement from the last visited tree to the ending point 'N'
    long long final_move_time = min(cost0 + abs(n - pos0), cost1 + abs(n - pos1));

    // Total time = (Time spent moving) + (1 minute to cut/load each of the M trees)
    long long total_time = final_move_time + m;

    cout << total_time << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
