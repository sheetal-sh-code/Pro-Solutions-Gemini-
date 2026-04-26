#include <bits/stdc++.h>

using namespace std;

// Helper function to merge overlapping intervals
void merge_intervals(vector<pair<int, int>>& intervals) {
    if (intervals.empty()) return;

    // Sort intervals based on starting points
    sort(intervals.begin(), intervals.end());

    vector<pair<int, int>> merged;
    merged.push_back(intervals[0]);

    for (int i = 1; i < intervals.size(); i++) {
        // If current interval overlaps with the last merged interval, merge them
        if (intervals[i].first <= merged.back().second) {
            merged.back().second = max(merged.back().second, intervals[i].second);
        } else {
            // Otherwise, push it as a new distinct interval
            merged.push_back(intervals[i]);
        }
    }
    // Update original list with the merged disjoint intervals
    intervals = merged;
}

// Helper function to check if a value exists within a sorted, non-overlapping list of intervals
bool is_in_intervals(int val, const vector<pair<int, int>>& intervals) {
    if (intervals.empty()) return false;

    int low = 0, high = intervals.size() - 1;
    int best_idx = -1;

    // Custom Binary Search to find the last interval that starts <= val
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (intervals[mid].first <= val) {
            best_idx = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // If we found a valid starting interval, check if 'val' falls before its end
    if (best_idx != -1 && val <= intervals[best_idx].second) {
        return true;
    }

    return false;
}

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<int> x_pts(n), y_pts(n);
    for (int i = 0; i < n; i++) cin >> x_pts[i];
    for (int i = 0; i < n; i++) cin >> y_pts[i];

    vector<int> x_path(m), y_path(m);
    for (int i = 0; i < m; i++) cin >> x_path[i];
    for (int i = 0; i < m; i++) cin >> y_path[i];

    // Hash maps to group segments.
    // Key: Fixed coordinate (Y for horiz, X for vert) -> Value: List of [start, end] ranges
    unordered_map<int, vector<pair<int, int>>> horiz_segments;
    unordered_map<int, vector<pair<int, int>>> vert_segments;

    // Step 1: Parse the path into Horizontal and Vertical segments
    for (int i = 0; i < m - 1; i++) {
        if (y_path[i] == y_path[i+1]) { // Horizontal segment
            int min_x = min(x_path[i], x_path[i+1]);
            int max_x = max(x_path[i], x_path[i+1]);
            horiz_segments[y_path[i]].push_back({min_x, max_x});
        }
        else if (x_path[i] == x_path[i+1]) { // Vertical segment
            int min_y = min(y_path[i], y_path[i+1]);
            int max_y = max(y_path[i], y_path[i+1]);
            vert_segments[x_path[i]].push_back({min_y, max_y});
        }
    }

    // Step 2: Sort and merge overlapping intervals on the same axis lines
    for (auto& pair : horiz_segments) merge_intervals(pair.second);
    for (auto& pair : vert_segments) merge_intervals(pair.second);

    // Step 3: Query each of the N points
    int points_on_path = 0;
    for (int i = 0; i < n; i++) {
        int px = x_pts[i], py = y_pts[i];
        bool found = false;

        // Check if the point lies on any horizontal segment on the line Y = py
        if (horiz_segments.count(py) && is_in_intervals(px, horiz_segments[py])) {
            found = true;
        }
        // Check if the point lies on any vertical segment on the line X = px
        else if (vert_segments.count(px) && is_in_intervals(py, vert_segments[px])) {
            found = true;
        }

        if (found) points_on_path++;
    }

    cout << points_on_path << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
