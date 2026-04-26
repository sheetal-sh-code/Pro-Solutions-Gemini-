# Algorithm Explanation: Equalizing the Necklace

## 1. The Core Intuition: Validating the Hint
Your initial hint (`Length of string - longest length of subarray with equal no of R and B`) is exactly the right path. Here is the mathematical proof of *why* it works:

When the problem states that you can **only remove stones from the left or right ends**, it imposes a strict geometric constraint. Imagine a piece of string. If you cut off a few inches from the left end, and a few inches from the right end, what you are holding in your hands is a single, unbroken, continuous piece from the middle.

In computer science terms, this unbroken piece is a **Contiguous Subarray**.
* If we want to **minimize** the stones thrown away, we must **maximize** the stones kept.
* Therefore, the problem translates perfectly to: *Find the longest contiguous subarray that contains an equal number of 'R' and 'B' stones.*

## 2. Converting Characters to Math (The +1 / -1 Trick)
Counting "equal numbers of two different characters" can be tricky to manage in a loop. We need a way to mathematically neutralize them.

The best way to do this is to assign a numerical weight to each character:
* Let `R` equal `+1`
* Let `B` equal `-1`

As we walk through the necklace from left to right, we keep a running total (a **Prefix Sum**).
If a segment has an equal number of `R`s and `B`s, their `+1`s and `-1`s will perfectly cancel each other out, adding up to exactly `0`.

## 3. The Prefix Sum Phenomenon
How do we find a segment that adds up to `0`?
Imagine our running total as an altitude tracker on a hike:
* We start at altitude `0`.
* Every `R` makes us climb `+1` meter.
* Every `B` makes us descend `-1` meter.

If we check our tracker and see we are currently at an altitude of `4`, and we look back at our notes and see we were *also* at an altitude of `4` two hours ago... it means in those past two hours, we climbed up and down but ultimately made zero net elevation change. The `+1`s and `-1`s canceled out.

**The Rule:** If a `prefix_sum` at index `j` is the exact same as a `prefix_sum` at an earlier index `i`, then the subarray between `i` and `j` has a sum of `0` (equal `R`s and `B`s).

## 4. Why this specific Data Structure? (Array Offset vs. Hash Map)
To find the longest valid subarray, we need to remember the *very first time* we saw a specific prefix sum.

The standard approach is to use a Hash Map (`unordered_map` in C++). However, in strict competitive programming, `unordered_map` has a worst-case time complexity of `O(N)` for lookups due to hash collisions. Hackers can deliberately feed your code test cases to trigger these collisions and cause a Time Limit Exceeded (TLE) error.

**The Offset Trick:**
Since the total string length is `N`, the prefix sum can never be higher than `+N` (all `R`s) or lower than `-N` (all `B`s). This is a known, tightly bounded range of size `2N + 1`.

Instead of a Hash Map, we can just use a massive Array of size `2N + 1`. But since arrays cannot have negative indices (like `-5`), we simply add an **offset** of `N` to every sum. A prefix sum of `0` gets stored at index `N`. A sum of `-5` gets stored at `N - 5`.
This guarantees ultra-fast, un-hackable, strict `O(1)` memory lookups, ensuring your solution easily passes even the tightest time limits.
