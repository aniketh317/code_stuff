In this problem, we are given 4 rods and starting from some
initial arrangement, we are to reach the actual initial state
wherein all the rings are on rod 0.. 

We are supposed to find out the minimum no. Of moves for doing so.

Solved the problem using graph approach where every arrangement has been
represented as a vertex and an edge existing between 2 nodes iff it is possible
to move from one arrangement to the other(corresponding to the nodes)

For finding out te minimum number of moves, BFS(Breadth first search) has been used.
