
# Deep Dive: String Concatenation and State Compression

## 1. The Core Intuition: Why Backtracking Fails
At first glance, this looks like a subset or combination problem: "Try picking different valid strings and see which combination yields the longest valid sequence."

**The Trap:** If you try a brute-force or recursive backtracking approach, for an array of size $N = 10^5$, you would evaluate $2^N$ possible subsequences. This will instantly result in a **Time Limit Exceeded (TLE)** error.

To optimize, we have to look closely at the constraints and the exact rules of merging.

## 2. State Compression (The DP Approach)
When we merge strings, what information actually matters?
If I merge "14", "421", and "123", I get "14421123". The total length is 8.

If I am looking at the *next* string in the array, say "389", what do I need to know about "14421123" to decide if I can attach "389" to it?
* Do I care about the middle digits "442112"? **No.**
* Do I care that it was made from exactly 3 strings? **No.**

I only care about **three things**:
1. The **First Digit** (because the problem demands the final string's first and last digit must match).
2. The **Last Digit** (to check if the next string can legally attach to it).
3. The **Total Length** accumulated so far.

Since the inputs consist entirely of integer representations, there are only 10 possible digits (`0` through `9`).
This shrinks our massive array of possibilities into a tiny $10 \times 10$ Dynamic Programming (DP) table!

## 3. The DP Table Definition
Let `dp[start][end]` represent the **maximum length** of a valid concatenated sequence that begins with the digit `start` and ends with the digit `end`.

* Initial State: The table is filled with `-1` (representing impossible states).
* When evaluating a string $S$ with a `first_digit` and `last_digit`:
  * It can attach to any existing sequence that currently ends in `first_digit`. So we look at all sequences `dp[x][first_digit]` and update `dp[x][last_digit] = max(existing, dp[x][first_digit] + length_of_S)`.
  * The string $S$ can also act as the starting point for a brand new sequence: `dp[first_digit][last_digit] = max(existing, length_of_S)`.

## 4. Pattern Recognition for the Future
* **"Matching Endpoints"**: Anytime a problem allows linking elements based only on their immediate boundaries (e.g., Dominoes, Word Arrays, Char matching), it is heavily signaling a bounded DP state based on those boundary characters.
* **$N=10^5$ with small properties**: If the input length is huge but the properties (like first/last characters) are tiny (only 10 digits or 26 letters), compress the state to $10 \times 10$ or $26 \times 26$ to achieve strict $O(N)$ time complexity.
