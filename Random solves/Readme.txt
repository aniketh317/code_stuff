Circulation:

This code calculates if there is any circulation possible in the network. In this network the edges are not only providede with an upper bound
on the flow through an edge but also a lower bound on the flow through an edge thereby creating a necessity to send a non-zero flow through
the system. There is no source or sink in the network

Each edge e has constraints: l_e <= f_e <= c_e (f_e is flow through the network) -- 0 <= f_e' <= c_e-l_e ( where f_e' = f_e - l_e)
1. This problem is reduced to a max flow problem by first adding source and sink to the network
2. We solve for f_e' = f_e - l_e with capacity constraints as c_e-l_e for every edge e
    a. Note that f_e' donot satify conservation of flow constraints. 
    b. Based on the Imbalance at a vertex, we add an edge from source to the vertex or vertex to the sink (with capacity of the added edge
       equal to imbalance)

Circulation is possible (iff) max flow in the new network is equal to the total capacity of edges out of source and is equal to total 
capacity of edges into sink.

Max-Flow problem solved using Push-Relabel algorithm
