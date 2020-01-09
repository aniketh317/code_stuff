This problem has been reduced to shortest superstring problem. 
In this problem Overlap graph(a weighted directed graph) is calaculated with each edge having weight as the amount of overlap
  a) u = aw , v = wb (w is the largest suffix of u that is prefix of v), wei(u->v) = |w|
  Then find out max-weighted Hamiltonian path using a Greedy approximation. String calculated using greedy approximation is at the most 2 times the shortest superstring
  
