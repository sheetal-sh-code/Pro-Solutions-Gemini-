
#include <iostream>
#include <vector>

using namespace std;

// Represents a linear function Y = m*X + c
struct Line {
    long long m, c;
};

// Evaluates the line equation for a given X
long long eval(Line l, long long x) {
    return l.m * x + l.c;
}

// Determines if line l2 is mathematically obsolete and should be popped.
// Cross-multiplication is done using 128-bit integers to prevent overflow
// when dealing with massive Y-intercepts.
bool check_pop(Line l1, Line l2, Line l3) {
    __int128 c2_c1 = l2.c - l1.c;
    __int128 m2_m3 = l2.m - l3.m;
    __int128 c3_c2 = l3.c - l2.c;
    __int128 m1_m2 = l1.m - l2.m;
    return c2_c1 * m2_m3 >= c3_c2 * m1_m2;
}

void solve() {
    int n;
    long long m;
    // Assuming standard CP format: input N, then M, then the array.
    if (!(cin >> n >> m)) return;

    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    // Prefix sums for array A and array (index * A)
    vector<long long> p(n, 0), s(n, 0);
    p[0] = a[0];
    s[0] = 0; // 0 * a[0]
    for (int i = 1; i < n; i++) {
        p[i] = p[i - 1] + a[i];
        s[i] = s[i - 1] + (long long)i * a[i];
    }

    // Safe accessors for -1 bounds
    auto get_p = [&](int i) { return i >= 0 ? p[i] : 0; };
    auto get_s = [&](int i) { return i >= 0 ? s[i] : 0; };

    vector<long long> dp(n + 1, 0);
    vector<Line> deque;
    int head = 0;

    for (int i = 1; i <= n; i++) {
        int j = i - 1;

        // Generate the new line for the current 'j'
        long long m_j = -j;
        long long c_j = dp[j] - get_s(j - 1) + (long long)j * get_p(j - 1);
        Line new_line = {m_j, c_j};

        // Maintain the lower convex hull
        while (deque.size() - head >= 2) {
            Line l2 = deque.back();
            Line l1 = deque[deque.size() - 2];
            if (check_pop(l1, l2, new_line)) {
                deque.pop_back();
            } else {
                break;
            }
        }
        deque.push_back(new_line);

        // Query the hull for X = P[i-1]
        long long x = get_p(i - 1);
        while (deque.size() - head >= 2) {
            Line l1 = deque[head];
            Line l2 = deque[head + 1];
            // Since we want the minimum, if l2 is better/equal to l1, drop l1.
            if (eval(l1, x) >= eval(l2, x)) {
                head++;
            } else {
                break;
            }
        }

        Line best = deque[head];
        dp[i] = m + get_s(i - 1) + eval(best, x);
    }

    // Minimum cost to clean the whole array (prefix of length N)
    cout << dp[n] << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
