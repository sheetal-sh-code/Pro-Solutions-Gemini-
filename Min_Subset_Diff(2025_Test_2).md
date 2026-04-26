# Deep Dive: The Minimum Partition Problem

## 1. The Core Intuition: Why Greedy Fails
When asked to divide items into two equal groups, the first instinct is often a **Greedy Approach**:
*Sort the cargoes descending, and always place the next heaviest cargo into whichever compartment currently has less weight.*

**Why this fails:** Let's look at the cargoes: `[7, 3, 3, 3, 1]`.
If we use the Greedy approach:
* Put `7` in A (Sum A = 7, Sum B = 0)
* Put `3` in B (Sum A = 7, Sum B = 3)
* Put `3` in B (Sum A = 7, Sum B = 6)
* Put `3` in B (Sum A = 7, Sum B = 9)
* Put `1` in A (Sum A = 8, Sum B = 9)
* Difference = `|8 - 9| = 1`.

Wait, greedy got it right here! But what about: `[6, 5, 5, 4, 4]`?
* Put `6` in A.
* Put `5` in B.
* Put `5` in B (B=10, A=6).
* Put `4` in A (B=10, A=10).
* Put `4` in A (B=10, A=14). Difference = 4.
**Optimal:** `[6, 6]` is impossible. But wait! `6 + 4 + 2`? No, the array is `[6, 5, 5, 4, 4]`. Sum = 24. Target = 12.
Optimal is `A = [4, 4, 4]` (Wait, only two 4s). Let's do `A = [6, 4], B = [5, 5, 4]` -> Wait, `6+4 = 10`, `5+5+4 = 14`. Is there `12`? `[4, 4, 4]` No. `[6, 6]` No. `[5, 5]` + something? No. But greedy might miss the best subset because it locks in early decisions without seeing the "puzzle pieces" that fit perfectly later. Greedy does not look ahead.

## 2. Mathematical Reduction (The Knapsack Link)
Let $S$ be the total sum of all cargoes.
We want to split the cargoes into two groups, $A$ and $B$, such that $A + B = S$.
We want to minimize $|A - B|$.

Without loss of generality, let's say $A \le B$.
This means $A$ can be at most $\lfloor S / 2 \rfloor$.
Since $B = S - A$, the difference between the two compartments is exactly:
$$B - A = (S - A) - A = S - 2A$$

To make $S - 2A$ as small as possible, we must make $A$ as **large as possible**, without exceeding $\lfloor S / 2 \rfloor$.
The problem perfectly translates to: *"Find a subset of cargoes whose sum is as close to $S/2$ as possible without going over."* This is the textbook **0/1 Knapsack Problem** (specifically, the Subset Sum variant).

## 3. Pattern Recognition for the Future
* **"Divide an array into two subsets to minimize difference"**: This is an absolute red flag for **Subset Sum DP**.
* **Target Sum / Change Making**: Whenever you have a pool of items and need to reach an exact numerical sum, use a boolean DP array.

## 4. Step-by-Step Algorithm Walkthrough
1. **Calculate the Target:** Sum all the elements. The `target` is `Total / 2`.
2. **Setup the DP Table:** We create a 1D boolean array `dp` of size `target + 1`. `dp[j]` will be `true` if it's possible to pick a combination of cargoes that exactly sum to `j`. We set `dp[0] = true` (an empty compartment weighs 0).
3. **Populating the Table:** We iterate through every cargo weight. For each cargo, we iterate backwards from `target` down to the `weight` of the cargo.
   * *Why backwards?* If we go forwards, we might use the same cargo multiple times. (e.g., if cargo is 3, setting `dp[3]=true` might instantly cause `dp[6]=true` in the same loop, acting as if we had two 3s).
   * If a previous sum `j - weight` was achievable, then adding our current cargo makes the new sum `j` achievable. `dp[j] = true`.
4. **Finding the Answer:** After processing all cargoes, we scan our `dp` array backwards starting from `target`. The very first index `j` that is `true` is the absolute maximum weight Compartment A can hold.
5. **The Final Output:** Compartment B holds the rest: `Total - A`. We print the difference!
