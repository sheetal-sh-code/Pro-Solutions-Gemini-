# Deep Dive: Optimal Threshold Selection

## 1. The Core Intuition: Why Brute Force and Binary Search Fail
The problem asks us to find a value $D$ (where $0 \le D \le 10^9$) that maximizes the difference between the score of array $A$ and array $B$.

* **Why Brute Force fails:** Checking every single number from $0$ to $10^9$ will result in a guaranteed **Time Limit Exceeded (TLE)**.
* **Why Binary Search on the Answer fails:** Binary search requires the target function to be *monotonic* (strictly increasing or strictly decreasing). The difference between $A$'s score and $B$'s score goes up and down randomly depending on the cluster of numbers. Since there is no single peak, Binary Search cannot be used.

## 2. The "Critical Points" Observation
If we pick a $D = 5$, any element $\le 5$ scores 1, and $> 5$ scores 2.
If we increase $D$ to $6$, the score of an element **only changes if that element was exactly $6$**.
If there are no $6$s in either array $A$ or array $B$, then setting $D=6$ gives the exact same total scores as setting $D=5$.

**The mathematical breakthrough:**
The total scores of $A$ and $B$ only ever change when $D$ exactly crosses a value that exists in array $A$ or array $B$.
Therefore, instead of checking 1 billion numbers, we only need to check the numbers that actually appear in the arrays, plus $0$ (the baseline where everything scores 2).

## 3. Pattern Recognition for the Future
* **"Score changes based on a threshold"**: Whenever items are scored or split into two categories based on a continuous variable $D$, you almost never check all possible values of $D$. You extract all unique elements from the given arrays, put them in a set/list, and only test those specific "Critical Points".
* **Two Arrays / Counting elements $\le X$**: This pattern is a massive sign pointing to **Sorting + Binary Search (`upper_bound`)**. Once arrays are sorted, finding how many items are $\le D$ goes from $O(N)$ down to $O(\log N)$.

## 4. Step-by-Step Algorithm Walkthrough
1. **Gather Candidates:** We read arrays $A$ and $B$. Every number we read is pushed into a `candidates` array. We also push `0` into this array.
2. **Sort the Arrays:** We sort $A$ and $B$. This allows us to use fast searching algorithms later.
3. **Coordinate Compression:** We sort our `candidates` array and remove all duplicates. If the number `15` appears ten times, we only need to test $D=15$ once. This saves operations.
4. **Evaluate each Candidate:**
   * For a candidate $D$, how many items in $A$ are $\le D$? We use `upper_bound` which finds this in $O(\log N)$ time. Let's call this `count_A`.
   * The score for $A$ is exactly `count_A * 1 + (Total_Size - count_A) * 2`.
   * We do the exact same `upper_bound` calculation for $B$ to find the score for $B$.
5. **Track the Maximum:** We calculate `Score_A - Score_B`. We keep a running maximum of this difference. If we find a $D$ that gives a larger difference, we update our best answer. (If there is a tie, it is standard practice to pick the one that gives $A$ the highest total score).

## 5. Complexity Breakdown
* **Time Complexity:** Sorting arrays $A$ and $B$ takes $O(N \log N + M \log M)$. Evaluating $|A+B|$ candidates takes $O((N+M) \log N)$. The overall time complexity is firmly $\mathcal{O}(K \log K)$ where $K = N+M$. This safely processes $10^5$ constraints in roughly $0.05$ seconds.
* **Space Complexity:** $\mathcal{O}(N+M)$ to store the arrays and the candidate list
