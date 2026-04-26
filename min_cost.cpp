#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int n;
vector<long long> a;
vector<vector<int>> adj;
vector<long long> sum_a; // sum_a[i] = sum of node values in the subtree of i
vector<long long> res;   // res[i] = total distance sum if tree is rooted at i
long long total_a = 0;   // sum of all a_i in the entire tree

// First Pass: Post-order DFS
// Calculates the total sum of 'a' for each subtree, and computes the answer
// as if node 1 is the absolute root of the tree.
void dfs1(int u, int p, long long depth) {
    sum_a[u] = a[u];
    // Add this node's contribution to the root's (node 1) total answer
    res[1] += depth * a[u];

    for (int v : adj[u]) {
        if (v != p) {
            dfs1(v, u, depth + 1);
            sum_a[u] += sum_a[v]; // Build the subtree sum bottom-up
        }
    }
}

// Second Pass: Pre-order DFS
// Transitions the root from parent 'u' to child 'v'
void dfs2(int u, int p) {
    for (int v : adj[u]) {
        if (v != p) {
            // Re-rooting Transition Math:
            // When we move from 'u' to 'v':
            // 1. Everything inside v's subtree gets 1 step CLOSER. (Subtract sum_a[v])
            // 2. Everything outside v's subtree gets 1 step FURTHER. (Add total_a - sum_a[v])
            res[v] = res[u] - sum_a[v] + (total_a - sum_a[v]);

            // Recursively re-root for the rest of the children
            dfs2(v, u);
        }
    }
}

void solve() {
    if (!(cin >> n)) return;

    a.assign(n + 1, 0);
    adj.assign(n + 1, vector<int>());
    sum_a.assign(n + 1, 0);
    res.assign(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        total_a += a[i];
    }

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Step 1: Compute baseline using node 1 as the root
    dfs1(1, 0, 0);

    // Step 2: Propagate the answer by shifting the root to all other nodes
    dfs2(1, 0);

    // Step 3: Find the maximum answer among all possible roots
    long long max_ans = 0;
    for (int i = 1; i <= n; i++) {
        max_ans = max(max_ans, res[i]);
    }

    cout << max_ans << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
