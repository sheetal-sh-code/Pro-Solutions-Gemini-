#include <iostream>
#include <vector>
#include <deque>

using namespace std;

// Structure to hold our current state in the BFS
struct State {
    int a; // number of apples eaten so far (next target is a + 1)
    int r; // current row
    int c; // current column
    int d; // current direction
};

// Direction vectors: 0: RIGHT, 1: DOWN, 2: LEFT, 3: UP (Clockwise order for right turns)
const int dr[] = {0, 1, 0, -1};
const int dc[] = {1, 0, -1, 0};
const int INF = 1e9;

void solve() {
    int t;
    if (!(cin >> t)) return;

    for (int tc = 1; tc <= t; ++tc) {
        int n;
        cin >> n;

        vector<vector<int>> grid(n, vector<int>(n));
        int m = 0; // Maximum apple number

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> grid[i][j];
                if (grid[i][j] > m) {
                    m = grid[i][j];
                }
            }
        }

        // To prevent massive overhead, we use a flattened 1D array for our 4D state.
        // State space size: (M + 1) * N * N * 4.
        // For N=60, M=150, this is ~2.17 million states, which easily fits in ~8.6 MB of memory.
        int max_states = (m + 1) * n * n * 4;
        vector<int> dist(max_states, INF);
        vector<bool> vis(max_states, false);

        // Helper lambda to instantly convert a 4D state into a 1D index
        auto get_idx = [&](int a, int r, int c, int d) {
            return ((a * n + r) * n + c) * 4 + d;
        };

        // Edge Case: Check if Apple 1 happens to spawn exactly at the starting cell (0, 0)
        int start_a = (grid[0][0] == 1) ? 1 : 0;
        int start_idx = get_idx(start_a, 0, 0, 0); // Start at (0,0) facing RIGHT (0)

        dist[start_idx] = 0;
        deque<State> dq;
        dq.push_back({start_a, 0, 0, 0});

        int ans = -1;

        // 0-1 BFS
        while (!dq.empty()) {
            auto [a, r, c, d] = dq.front();
            dq.pop_front();

            int idx = get_idx(a, r, c, d);

            // If already processed with a shorter or equal path, skip it
            if (vis[idx]) continue;
            vis[idx] = true;

            // Since it's a 0-1 BFS, the FIRST time we process a state where all apples are eaten,
            // it is mathematically guaranteed to be the absolute minimum cost.
            if (a == m) {
                ans = dist[idx];
                break;
            }

            // --- Option 1: Move Forward (Cost = 0) ---
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (nr >= 0 && nr < n && nc >= 0 && nc < n && grid[nr][nc] != -1) {
                int na = a;
                // If the cell contains the EXACT apple we are looking for, we eat it
                if (grid[nr][nc] == a + 1) {
                    na = a + 1;
                }

                int nidx = get_idx(na, nr, nc, d);
                if (dist[nidx] > dist[idx]) {
                    dist[nidx] = dist[idx];
                    dq.push_front({na, nr, nc, d}); // Cost 0 -> Push to FRONT
                }
            }

            // --- Option 2: Turn Right in Place (Cost = 1) ---
            int nd = (d + 1) % 4; // Clockwise rotation
            int nidx = get_idx(a, r, c, nd);

            if (dist[nidx] > dist[idx] + 1) {
                dist[nidx] = dist[idx] + 1;
                dq.push_back({a, r, c, nd}); // Cost 1 -> Push to BACK
            }
        }

        cout << "#" << tc << " " << ans << "\n";
    }
}

int main() {
    // Fast I/O for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
