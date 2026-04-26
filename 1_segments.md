# Competitive Programming Solution: Points on an Axis-Parallel Path

## 1. Complete Optimized C++ Code

```cpp
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

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
```

## 2. Beginner-Friendly Step-by-Step Explanation

Imagine you and I are sitting at a whiteboard, looking at this problem for the very first time.

### Phase 1: Visualizing the Problem
Imagine a giant piece of graph paper.
1.  **The Points ($N$):** You take a red pen and draw $N$ distinct dots on the graph paper. These are your "target" points.
2.  **The Path ($M$):** You take a blue marker and start drawing a path connecting $M$ points. The rule is that you can only draw straight horizontal lines (left/right) or straight vertical lines (up/down). No diagonals allowed.

**The Goal:** You need to count exactly how many of your red dots got covered by the blue marker.

### Phase 2: The "Brute Force" Method (And why it fails)
When you first see a problem, it's good to think of the simplest, most obvious solution.

* **The Naive Idea:** For every single red dot (all $N$ of them), let's look at every single blue line segment (all $M-1$ of them) and check: "Hey, is this dot sitting on this line segment?"
* **The Problem (Time Limit Exceeded):** In competitive programming, $N$ and $M$ can be huge numbers, like $100,000$. If you check every dot against every line, you do $100,000 \times 100,000 = 10,000,000,000$ checks. A computer can generally only do about $100,000,000$ simple operations in 1 second. Your code would take way too long and fail.

We need a shortcut. We need to avoid checking lines that are nowhere near our dots.

### Phase 3: The Smart Approach

#### Step 3.1: The "Filing Cabinet" Concept (Hash Maps)
If a red dot is located at `Y = 5` (height of 5), does it make sense to check horizontal blue lines that are at `Y = 2` or `Y = 10`? Absolutely not!

So, we organize our blue lines into "filing cabinets" based on their coordinates. We use a data structure called a **Hash Map** (`unordered_map` in C++) to do this.
* **Horizontal Cabinet:** The labels on the drawers are the `Y` coordinates. If we have a horizontal line segment from `X=1` to `X=4` at height `Y=5`, we open the `Y=5` drawer and put the range `[1, 4]` inside.
* **Vertical Cabinet:** The labels on the drawers are the `X` coordinates. If we have a vertical line from `Y=2` to `Y=8` at position `X=3`, we open the `X=3` drawer and put the range `[2, 8]` inside.

#### Step 3.2: The Overlap Problem (Merging Intervals)
What if the blue path goes right, then left, then right again on the exact same line? You might end up with segments in your drawer that overlap, like `[1, 5]` and `[3, 7]`.

If a red dot is at `X=4`, and you check both of those overlapping segments, you might accidentally count the dot twice, or it just makes searching messy and slow.
* **The Fix:** Before we do any searching, we take every drawer, sort the ranges inside, and "smush" (merge) overlapping ones together. `[1, 5]` and `[3, 7]` just become one big clean block: `[1, 7]`.

#### Step 3.3: The Fast Search (Binary Search)
Now we have clean, non-overlapping blocks of lines in our drawers. Let's say we have a red dot at `X=8, Y=5`.
1.  We instantly open the horizontal drawer labeled `Y=5`.
2.  Inside, we see a list of sorted, clean line segments.
3.  Instead of checking them one by one (which is slow), we use **Binary Search**. It's like finding a word in a dictionary: you open the book to the middle, see if you went too far or not far enough, and cut your search in half every time. This allows us to instantly find out if `X=8` falls inside any of the line segments.

### Phase 4: Code Walkthrough for Newbies

#### 1. The Setup and Input
```cpp
    int n, m;
    cin >> n >> m; // Read the number of points (N) and path corners (M)

    // Create lists (vectors) to store the red dots
    vector<int> x_pts(n), y_pts(n);
    for (int i = 0; i < n; i++) cin >> x_pts[i];
    for (int i = 0; i < n; i++) cin >> y_pts[i];
```
* **`vector<int>`**: Think of this as a dynamically sizing array or a list that holds integers.

#### 2. Creating the Filing Cabinets
```cpp
    // unordered_map maps an integer (the drawer label) to a list of pairs (the line segments)
    unordered_map<int, vector<pair<int, int>>> horiz_segments;

    for (int i = 0; i < m - 1; i++) {
        // If Y stays the same, it's a horizontal line!
        if (y_path[i] == y_path[i+1]) {
            int min_x = min(x_path[i], x_path[i+1]);
            int max_x = max(x_path[i], x_path[i+1]);
            // Put the range [min_x, max_x] into the Y drawer
            horiz_segments[y_path[i]].push_back({min_x, max_x});
        }
    }
```
* **`pair<int, int>`**: Just a neat way to bundle two numbers together, like a `[start, end]` of a line.
* **`min()` and `max()`**: A path can go backwards (e.g., from `X=5` to `X=1`). We always want our segments to be listed as `[Smallest, Largest]` so `[1, 5]`.

#### 3. Smushing (Merging) the Overlaps
```cpp
    // For every drawer in our horizontal cabinet, run the merge function
    for (auto& pair : horiz_segments) merge_intervals(pair.second);
```
* **How `merge_intervals` works**: It sorts the ranges by their starting number. If the *start* of the next range is smaller than or equal to the *end* of the current range, they touch! It stretches the end of the current range to encompass both.

#### 4. The Final Count (Answering the Queries)
```cpp
    int points_on_path = 0;

    for (int i = 0; i < n; i++) {
        int px = x_pts[i]; // Current red dot's X
        int py = y_pts[i]; // Current red dot's Y
        bool found = false;

        // 1. Check if the Y drawer exists. If it does, do a fast binary search.
        if (horiz_segments.count(py) && is_in_intervals(px, horiz_segments[py])) {
            found = true;
        }

        // If we found it, count it!
        if (found) points_on_path++;
    }
```
* **`horiz_segments.count(py)`**: This quickly checks "Does drawer number `py` even exist?". If there are no horizontal lines at height `py`, it returns 0 (false), saving us from searching an empty drawer.
* **`is_in_intervals`**: This is our custom Binary Search function. It rapidly flips through the sorted line segments to see if our dot's coordinate falls between the `start` and `end` of any of them.
