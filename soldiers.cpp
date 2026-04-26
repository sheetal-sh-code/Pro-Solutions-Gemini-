#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

// Maximum possible sum = 500 nodes * 100 soldiers = 50000
const int MAX_SUM = 50005;

// dp[u][s] is true if the subtree rooted at 'u' can be balanced to have exactly sum 's'
bitset<MAX_SUM> dp[505];
vector<int> adj[505];
int val[505];

void dfs(int u) {
    // Base Case: Leaf Node
    if (adj[u].empty()) {
        // A leaf can be reduced to any value between 0 and its original value
        for (int i = 0; i <= val[u]; ++i) {
            dp[u].set(i);
        }
        return;
    }

    // common bitset will store the INTERSECTION of achievable sums of all children
    bitset<MAX_SUM> common;
    common.set(); // Initialize all to 1 (true)

    for (int v : adj[u]) {
        dfs(v);
        common &= dp[v]; // Bitwise AND to find sums achievable by ALL children
    }

    int k = adj[u].size();
    bitset<MAX_SUM> valid_bases;

    // For every uniform sum 'c' that all children can achieve,
    // the total sum contributed by children is k * c.
    for (int c = 0; c < MAX_SUM; ++c) {
        if (common.test(c)) {
            if (k * c < MAX_SUM) {
                valid_bases.set(k * c);
            }
        }
    }

    // Now, add the parent's soldiers.
    // We can keep anywhere from 0 to val[u] soldiers from the current unit.
    dp[u].reset();
    for (int x = 0; x <= val[u]; ++x) {
        // Shifting valid_bases by x perfectly simulates adding 'x' to all valid base sums!
        dp[u] |= (valid_bases << x);
    }
}

void solve() {
    int n;
    if (!(cin >> n)) return;

    int root = -1;
    for (int i = 0; i < n; ++i) {
        int p, v;
        cin >> p >> v;
        val[i] = v;

        // Assuming -1 indicates the root of the tree (standard CP format)
        if (p == -1 || p == i) {
            root = i;
        } else {
            adj[p].push_back(i);
        }
    }

    if (root == -1) root = 0; // Fallback just in case

    dfs(root);

    // Find the maximum valid balanced sum for the entire tree
    int max_remaining = 0;
    for (int i = MAX_SUM - 1; i >= 0; --i) {
        if (dp[root].test(i)) {
            max_remaining = i;
            break;
        }
    }

    cout << max_remaining << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
