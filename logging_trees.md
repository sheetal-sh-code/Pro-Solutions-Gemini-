# Intuition and Step-by-Step Breakdown

## 1. Intuition: Why this Approach?
At its core, this problem combines routing with a strict ordering constraint.
The rule states that we can only harvest trees if their length is $\le$ the previously harvested tree. This forces us to group the trees by length and process them in **descending order**.

If we have multiple trees of the *same* length, we can harvest them in any order we want. Let's say all trees of length 5 are located at points `{2, 4, 8}`. Because the robot moves 1 unit at a time, simply traveling between the extremes—the minimum position `L=2` and maximum position `R=8`—guarantees that we pass over (and can harvest) every tree in between for free!

Therefore, for any group of identically sized trees, the most efficient path will **always** end at one of two places:
1. It ends at the leftmost tree `L` (meaning the robot visited `R` first).
2. It ends at the rightmost tree `R` (meaning the robot visited `L` first).

This binary choice immediately signals that we can use **Dynamic Programming (DP)**.

## 2. Pattern Recognition for the Future
How can you spot this pattern in future questions?
* **"Visit points on a 1D line"**: 1D grid problems almost always boil down to sorting positions and tracking the left/right boundaries.
* **"Must be done in a specific order / sequence"**: When you are forced to process subsets of items in a strict order, your DP state transitions entirely between the subsets. You only need to remember *where* you ended up after the previous subset.

## 3. Step-by-Step Algorithm Walkthrough

**Step 1: Parse and Group by Length**
We read all the trees and group them by their length using a map. We use `greater<int>` to automatically sort the keys from tallest to shortest. Instead of saving every single tree's position, we only save the leftmost (`L`) and rightmost (`R`) position for each length group.

**Step 2: DP State Initialization**
We initialize our robot at the starting point.
* `pos0 = 0`, `pos1 = 0` (Our current positions)
* `cost0 = 0`, `cost1 = 0` (The time taken to reach these positions)

**Step 3: Calculating Transitions (The Core Logic)**
For each group of trees (from tallest to shortest), we calculate two new possibilities:
* **To end at `L`**: The robot must travel to `R` first. It can start from its previous `pos0` or `pos1`. The cost is the minimum distance to reach `R`, plus the distance to sweep back to `L` (`R - L`).
* **To end at `R`**: The robot must travel to `L` first. The cost is the minimum distance to reach `L`, plus the sweep distance to `R` (`R - L`).

We update our `pos` and `cost` variables and move to the next group.

**Step 4: The Final Stretch & Cutting Time**
After processing the shortest trees, our robot is resting at either the final `L` or `R`. We simply calculate the minimum distance to drive the rest of the way to point `N`.
Finally, because *every single tree* takes exactly 1 minute to cut and load regardless of movement, we just add `M` (total number of trees) to our total movement time to get the final answer.

## Complexity
* **Time Complexity:** $O(M \log M)$ because we iterate through the $M$ trees and insert them into a sorted map. The DP loop runs in $O(K)$ time where $K$ is the number of unique lengths. This easily executes well within the 1-second limit.
* **Space Complexity:** $O(K)$ to store the bounds for the unique tree lengths in the map. Extremely memory efficient.
