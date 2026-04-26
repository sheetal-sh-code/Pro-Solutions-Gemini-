#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const long long INF = 1e15;

// Helper to count set bits
int popcount(int n) {
    int count = 0;
    while (n) {
        n &= (n - 1);
        count++;
    }
    return count;
}

void solve() {
    int h, w, c;
    if (!(cin >> h >> w >> c)) return;

    vector<vector<int>> grid(h, vector<int>(w));
    pair<int, int> garage = {-1, -1};
    pair<int, int> airport = {-1, -1};
    vector<pair<int, int>> warehouses;

    for (int r = 0; r < h; r++) {
        for (int c_idx = 0; c_idx < w; c_idx++) {
            cin >> grid[r][c_idx];
            if (grid[r][c_idx] == 2) garage = {r, c_idx};
            else if (grid[r][c_idx] == 3) warehouses.push_back({r, c_idx});
            else if (grid[r][c_idx] == 4) airport = {r, c_idx};
        }
    }

    int W = warehouses.size();
    if (W == 0 || garage.first == -1 || airport.first == -1) {
        cout << 0 << "\n";
        return;
    }

    int K = W + 2;
    // 0: Garage, 1..W: Warehouses, W+1: Airport
    vector<vector<long long>> dist(K, vector<long long>(K, INF));
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    auto get_distances = [&](int start_node, pair<int, int> start_coord) {
        vector<vector<long long>> d_grid(h, vector<long long>(w, INF));
        queue<pair<int, int>> q;

        q.push(start_coord);
        d_grid[start_coord.first][start_coord.second] = 0;

        while (!q.empty()) {
            auto [r, curr_c] = q.front();
            q.pop();

            for (int i = 0; i < 4; i++) {
                int nr = r + dr[i];
                int nc = curr_c + dc[i];

                if (nr >= 0 && nr < h && nc >= 0 && nc < w && grid[nr][nc] != 1) {
                    if (d_grid[nr][nc] == INF) {
                        d_grid[nr][nc] = d_grid[r][curr_c] + 1;
                        q.push({nr, nc});
                    }
                }
            }
        }

        dist[start_node][0] = d_grid[garage.first][garage.second];
        for (int i = 0; i < W; i++) {
            dist[start_node][i + 1] = d_grid[warehouses[i].first][warehouses[i].second];
        }
        dist[start_node][W + 1] = d_grid[airport.first][airport.second];
    };

    get_distances(0, garage);
    for (int i = 0; i < W; i++) get_distances(i + 1, warehouses[i]);
    get_distances(W + 1, airport);

    // dp_trip[start_type][mask][last_warehouse]
    // start_type: 0 for Garage, 1 for Airport
    vector<vector<vector<long long>>> dp_trip(2, vector<vector<long long>>(1 << W, vector<long long>(W, INF)));

    for (int i = 0; i < W; i++) {
        if (dist[0][i + 1] != INF) dp_trip[0][1 << i][i] = dist[0][i + 1] * 1;
        if (dist[W + 1][i + 1] != INF) dp_trip[1][1 << i][i] = dist[W + 1][i + 1] * 1;
    }

    for (int mask = 1; mask < (1 << W); mask++) {
        int sz = popcount(mask);
        for (int last = 0; last < W; last++) {
            if (!(mask & (1 << last))) continue;

            for (int next = 0; next < W; next++) {
                if (mask & (1 << next)) continue;
                if (dist[last + 1][next + 1] == INF) continue;

                int new_mask = mask | (1 << next);
                long long segment_cost = dist[last + 1][next + 1] * (sz + 1);

                if (dp_trip[0][mask][last] != INF) {
                    dp_trip[0][new_mask][next] = min(dp_trip[0][new_mask][next], dp_trip[0][mask][last] + segment_cost);
                }
                if (dp_trip[1][mask][last] != INF) {
                    dp_trip[1][new_mask][next] = min(dp_trip[1][new_mask][next], dp_trip[1][mask][last] + segment_cost);
                }
            }
        }
    }

    vector<long long> trip_G(1 << W, INF);
    vector<long long> trip_A(1 << W, INF);
    trip_G[0] = 0;
    trip_A[0] = 0;

    for (int mask = 1; mask < (1 << W); mask++) {
        int sz = popcount(mask);
        for (int last = 0; last < W; last++) {
            if (!(mask & (1 << last))) continue;
            if (dist[last + 1][W + 1] == INF) continue;

            long long drop_cost = dist[last + 1][W + 1] * (sz + 1);
            if (dp_trip[0][mask][last] != INF) {
                trip_G[mask] = min(trip_G[mask], dp_trip[0][mask][last] + drop_cost);
            }
            if (dp_trip[1][mask][last] != INF) {
                trip_A[mask] = min(trip_A[mask], dp_trip[1][mask][last] + drop_cost);
            }
        }
    }

    vector<long long> dp_A(1 << W, INF);
    dp_A[0] = 0;
    for (int mask = 1; mask < (1 << W); mask++) {
        for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
            if (trip_A[sub] != INF && dp_A[mask ^ sub] != INF) {
                dp_A[mask] = min(dp_A[mask], trip_A[sub] + dp_A[mask ^ sub]);
            }
        }
    }

    vector<long long> dp_G(1 << W, INF);
    dp_G[0] = 0;
    for (int mask = 1; mask < (1 << W); mask++) {
        for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
            if (trip_G[sub] != INF && dp_A[mask ^ sub] != INF) {
                dp_G[mask] = min(dp_G[mask], trip_G[sub] + dp_A[mask ^ sub]);
            }
        }
    }

    int max_goods = 0;
    for (int mask = 0; mask < (1 << W); mask++) {
        if (dp_G[mask] <= c) {
            max_goods = max(max_goods, popcount(mask));
        }
    }

    cout << max_goods << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    if (cin >> t) {
        while (t--) {
            solve();
        }
    }
    return 0;
}
