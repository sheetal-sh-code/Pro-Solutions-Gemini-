# Deep Dive: Truck Routing & Dynamic Programming

## 1. The Core Intuition: Why Greedy or Simulation Fails

When faced with a grid routing problem, the natural instinct is to try a **Greedy Approach**:
* "Just find the closest warehouse, pick up the good, and drop it off."
* "If going to another warehouse nearby is cheaper, do that instead."

**Why this fails:** The truck's weight dynamically increases the cost of moving. A warehouse might be physically close, but if you visit it as your 4th stop, every single block you drive from there to the airport will be multiplied by $5$ (4 goods + empty weight). A greedy algorithm cannot foresee that making multiple smaller trips dropping off 2 goods at a time might be cheaper overall than one massive, heavy trip picking up 5 goods.

Because the state of the problem depends heavily on the *exact sequence* of visited warehouses, pure simulation or greedy pathfinding is impossible. We need to evaluate all valid sequences, but efficiently.

## 2. Why Pick This Specific Approach? (Bitmask DP)

This problem screams **Bitmask Dynamic Programming (DP)** due to one vital constraint:
**"There can be at max 13 warehouses."**

Whenever you see a target collection size of $N \le 15$ in a complex routing problem, it is a deliberate signal from the problem author to use a binary mask (Bitmask) to represent the "State" of visited locations.
* $2^{13} = 8192$ possible subsets of warehouses. This number is incredibly small for a computer, making exact subset evaluation extremely fast.

### Phase 1: Grid Condensation (Breadth-First Search)
We do not care about the empty road cells. We only care about the Garage, the Warehouses, and the Airport.
We run a simple BFS from these key points to build a **Distance Matrix** $D$.
$D[u][v]$ gives us the strict shortest distance between any two important points, completely bypassing trees.

### Phase 2: Trip Cost Evaluation
A **Trip** is defined as starting empty (either from the Garage or the Airport), visiting a specific subset of warehouses $S$, and ending at the Airport to unload.

To find the minimum cost of a trip covering subset $S$:
$$	ext{TripCost}(S) = \min_{w \in S} \left( 	ext{CostToReach}_w(S) + D[w][	ext{Airport}] 	imes (|S| + 1)
ight)$$

We use Bitmask DP to calculate $	ext{CostToReach}_w(S)$. If our truck is at warehouse $A$, and the next stop in our subset mask is $B$, the cost to travel between them is $D[A][B] 	imes (	ext{Goods Currently Carried} + 1)$. We build this DP from $1$ warehouse up to $13$ warehouses.

### Phase 3: Combining Trips (Submask Iteration)
Once we know the optimal cost of doing a single trip for *every possible subset*, we must recognize that the truck can do **multiple trips**.
To deliver a final subset $S$, the truck could split $S$ into multiple smaller trips (e.g., $S_1, S_2, S_3$).

How do we find the best split? We use a powerful technique called **Submask Iteration**.
For every subset $S$, we loop over every smaller piece $T$ inside $S$:
$$	ext{TotalCost}(S) = \min_{T \subset S} \left( 	ext{FirstTripCost}(T) + 	ext{RemainingTripsFromAirport}(S \setminus T)
ight)$$

The first trip must start at the Garage, but all subsequent trips will start at the Airport.

### Why is this the ONLY way?
By breaking the problem down into:
1. Shortest Paths (BFS) -> $\mathcal{O}(V+E)$
2. Single Trip Subset Costs (Bitmask DP) -> $\mathcal{O}(2^W \cdot W^2)$
3. Multi-Trip Combinations (Submask DP) -> $\mathcal{O}(3^W)$

We transform an infinitely complex routing simulation into strict, bounded mathematical states. $3^{13}  pprox 1.59 	imes 10^6$ operations, allowing us to solve all 50 test cases well within the 1-2 second strict competitive programming time limits.
