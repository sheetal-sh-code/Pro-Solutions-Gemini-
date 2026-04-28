
# Deep Dive: Optimal Tile Selection

## 1. The Core Intuition: Geometric Translation
The problem asks us to minimize the maximum difference between any two chosen tiles. The difference between tile $i$ and tile $j$ is defined as:
$$	ext{Difference} = \max(|W_i - W_j|, |H_i - H_j|)$$

In mathematics, this exact formula is known as the **Chebyshev Distance** ($L_\infty$ norm).
Let's view each tile as a 2D point on a graph: $(W_i, H_i)$.
If a group of $K$ points has a maximum pairwise Chebyshev distance of $D$, it strictly means that the difference between the maximum $X$ and minimum $X$ is $\le D$, and the difference between the maximum $Y$ and minimum $Y$ is $\le D$.

**The Visual Translation:**
The problem perfectly translates to: *"Find the smallest axis-aligned square of side length $D$ that can capture at least $K$ points."*

## 2. Why This Approach? (Binary Search + Line Sweep)

1. **Binary Search on the Answer:** Whenever a problem asks you to **"minimize a maximum"**, it is the number one indicator for Binary Search. Instead of trying to construct the perfect set of $K$ tiles, we flip the question: *"Given a fixed square size $D$, can we catch $K$ points?"* If yes, we try a smaller $D$. If no, we must increase $D$.

2. **Line Sweep & Sliding Window ($O(N \log N)$):**
   Checking if a fixed square of size $D$ covers $K$ points could take $O(N^2)$ if done naively. We can drop this to $O(N \log N)$ by splitting the 2D problem into two 1D problems:
   * **The X-axis (Width):** We sort the points by width and use a Two-Pointer Sliding Window. As the right pointer moves, we include points. If the distance between the left and right pointer exceeds $D$, we slide the left pointer forward, removing those points.
   * **The Y-axis (Height):** For all the active points currently inside our valid $X$ window, we need to know if there is a vertical window of size $D$ that contains $K$ of them. We use a **Segment Tree with Lazy Propagation** for this.

## 3. Pattern Recognition for the Future
How do you instantly spot this combination in future contests?
* **"Minimize the maximum difference of two properties"**: Indicates Binary Search on the answer + Chebyshev Distance geometry.
* **"Find max points in a fixed-size 2D window"**: This is the textbook definition of the **Line Sweep Algorithm**. You use a sliding window for the $X$ coordinates, and a Segment Tree to manage the $Y$ intervals on the fly.

## 4. Step-by-Step Algorithm Walkthrough

### Outer Shell: Binary Search
1. We determine the absolute bounds. The lowest possible answer is `0`. The highest possible answer is the max width difference or max height difference among all tiles.
2. We pick a mid-point `D` and pass it to our `check(D)` function.

### Inner Shell: The `check(D)` Function
1. **Coordinate Compression:** We gather all the $Y$ coordinates (Heights) and their $Y+D$ counterparts. We sort and remove duplicates. This allows our Segment tree to be $O(N)$ size instead of $O(10^9)$, saving massive memory and time.
2. **The Sliding Window:** We sweep from left to right across our sorted points.
3. **Entering the Window:** When a point $R$ enters, we imagine a vertical bar of height $D$ resting entirely below this point. We add `+1` to the range $[H_R, H_R+D]$ in our Segment Tree.
4. **Leaving the Window:** If the width between our leftmost active point $L$ and our new point $R$ is greater than $D$, point $L$ is too far away. We remove it by subtracting `-1` from the range $[H_L, H_L+D]$ in our Segment Tree, and increment $L$.
5. **The Magic Check:** After adding point $R$, we simply check the global maximum of our Segment tree (`tree[1].mx`). If it is $\ge K$, it means at some $Y$ coordinate, $K$ vertical bars overlap simultaneously. Therefore, a $D 	imes D$ square can capture $K$ points! We immediately return `true`
