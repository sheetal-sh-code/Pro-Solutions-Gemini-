
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure to represent our tiles as 2D points
struct Point {
    long long w, h;
};

// Segment Tree Node for Range Additions and Global Max Query
struct Node {
    int mx, lazy;
};

vector<Node> tree;

// Pushes lazy updates down to the children
void push_down(int node) {
    if (tree[node].lazy != 0) {
        tree[node * 2].mx += tree[node].lazy;
        tree[node * 2].lazy += tree[node].lazy;
        tree[node * 2 + 1].mx += tree[node].lazy;
        tree[node * 2 + 1].lazy += tree[node].lazy;
        tree[node].lazy = 0;
    }
}

// Range Update function
void update(int node, int l, int r, int ql, int qr, int val) {
    if (ql <= l && r <= qr) {
        tree[node].mx += val;
        tree[node].lazy += val;
        return;
    }
    push_down(node);
    int mid = l + (r - l) / 2;
    if (ql <= mid) update(node * 2, l, mid, ql, qr, val);
    if (qr > mid) update(node * 2 + 1, mid + 1, r, ql, qr, val);

    tree[node].mx = max(tree[node * 2].mx, tree[node * 2 + 1].mx);
}

// Checks if there exists any D x D square that covers at least K points
bool check(long long D, int K, const vector<Point>& pts) {
    int n = pts.size();

    // Step 1: Coordinate Compression for Y-axis (Heights)
    vector<long long> y_vals;
    y_vals.reserve(2 * n);
    for (int i = 0; i < n; i++) {
        y_vals.push_back(pts[i].h);
        y_vals.push_back(pts[i].h + D);
    }
    sort(y_vals.begin(), y_vals.end());
    y_vals.erase(unique(y_vals.begin(), y_vals.end()), y_vals.end());

    int M = y_vals.size();
    tree.assign(4 * M + 1, {0, 0}); // Reset Segment tree

    auto get_idx = [&](long long val) {
        return lower_bound(y_vals.begin(), y_vals.end(), val) - y_vals.begin() + 1;
    };

    // Step 2: Line Sweep using a Sliding Window on the X-axis (Widths)
    int L = 0;
    for (int R = 0; R < n; R++) {
        // If the window is too wide (> D), remove the leftmost points
        while (pts[R].w - pts[L].w > D) {
            int ql = get_idx(pts[L].h);
            int qr = get_idx(pts[L].h + D);
            update(1, 1, M, ql, qr, -1);
            L++;
        }

        // Add the current point into the active window
        int ql = get_idx(pts[R].h);
        int qr = get_idx(pts[R].h + D);
        update(1, 1, M, ql, qr, 1);

        // If the maximum overlapping intervals >= K, D is achievable!
        if (tree[1].mx >= K) return true;
    }
    return false;
}

void solve() {
    int n, k;
    if (!(cin >> n >> k)) return;

    vector<Point> pts(n);
    long long min_w = 2e18, max_w = -2e18;
    long long min_h = 2e18, max_h = -2e18;

    for (int i = 0; i < n; i++) {
        cin >> pts[i].w >> pts[i].h;
        min_w = min(min_w, pts[i].w);
        max_w = max(max_w, pts[i].w);
        min_h = min(min_h, pts[i].h);
        max_h = max(max_h, pts[i].h);
    }

    // Edge case: Selecting 1 or 0 tiles has a max difference of 0
    if (k <= 1) {
        cout << 0 << "\n";
        return;
    }

    // Sort by Width (X-axis) for the Sliding Window
    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        if (a.w != b.w) return a.w < b.w;
        return a.h < b.h;
    });

    // Step 3: Binary Search on the Answer
    long long low = 0;
    long long high = max(max_w - min_w, max_h - min_h);
    long long ans = high;

    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (check(mid, k, pts)) {
            ans = mid;
            high = mid - 1; // Try to find a tighter/smaller maximum difference
        } else {
            low = mid + 1; // Increase allowed difference
        }
    }

    cout << ans << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
