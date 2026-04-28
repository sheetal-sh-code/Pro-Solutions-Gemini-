
 Deep Dive: Simultaneous Car Parking

## 1. The Core Intuition: The 45-Degree Rotation
Whenever a grid problem involves moving in 4 cardinal directions (or navigating Manhattan distances), calculating overlaps is notoriously difficult because a "circle" in Manhattan distance looks like a tilted square (a diamond).

We can completely fix this by transforming our coordinates into a 45-degree rotated space:
$$u = x + y$$
$$v = x - y$$

Why do we do this?
If a car moves a Manhattan distance of exactly $K$, its true $X$ and $Y$ coordinates change in heavily dependent ways. But in the $U$ and $V$ coordinate system, moving a distance of $K$ allows $U$ and $V$ to change by *up to* $\pm K$ completely independently!
The problem transforms from *"cars navigating a 2D grid"* into *"two separate 1D number lines ($U$ and $V$) where cars must reach a center point."*

## 2. The Parity Trap (Why it sometimes returns -1)
A car moves exactly $1$ distance, then $2$, then $3 \dots$ up to $K$.
Every time you move horizontally or vertically, your $(x+y)$ coordinate sum changes its parity (Odd to Even, or Even to Odd) depending strictly on whether the step size is odd or even.

After $K$ drives, the total sum of steps taken is $S = rac{K(K+1)}{2}$.
No matter what path a car takes, twisting left or right, the parity of its final coordinate sum is mathematically locked to:
$$	ext{Final\_Parity} \equiv (	ext{Initial}\ x + 	ext{Initial}\ y) + S \pmod 2$$

Because *all* cars must meet at the *same* final position $(p, q)$, they must all share the same final parity. Because $S$ is identical for all cars, **this proves every car must start with the exact same $(x+y) \pmod 2$ parity.** If even one car starts on a different parity, it is physically impossible for them to ever meet. We immediately return `-1`.

## 3. Geometric Bounding (Finding Minimum Drives)
Once parity is validated, finding the minimum drives is simple math.
1. We find the extreme edges of all cars in the $U$ axis ($u_{min}$ and $u_{max}$) and the $V$ axis ($v_{min}$ and $v_{max}$).
2. To minimize the maximum distance any single car has to travel, we must place our target parking spot dead in the center of these extremes.
3. The required reach in the $U$ axis is $R_u = \lceil rac{u_{max} - u_{min}}{2}
ceil$, and identically $R_v$ for the $V$ axis.
4. The absolute minimum reach our cars need is $R = \max(R_u, R_v)$.

## 4. Binary Search for K
In $K$ drives, a car can cover a maximum distance of $S = 1 + 2 + 3 + \dots + K = rac{K(K+1)}{2}$.
Since coordinates can stretch up to $10^{17}$, the maximum distance between two cars could be $2 \cdot 10^{17}$.
We just use Binary Search to find the smallest integer $K$ such that $rac{K(K+1)}{2} \ge R$.
Because $K$ maxes out around $ pprox 6.3 	imes 10^8$, Binary Search takes just $\sim 31$ operations, making this code lightning fast $\mathcal{O}(N)$
