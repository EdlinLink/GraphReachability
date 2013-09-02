This is a course project solving a kind of problem:

Weight Constraint Reachability
Given an undirected weighted graph G=(V, E, w), and a query Q=(a, b, <=y), can node a reach node b such that every edge weight on the path is <=y?

And the graph has a large number of nodes and edges.


(1)Construct a MST
(2)Find the LCA(my implementation is not in O(1) but O(log(n)))
(3)Check whether the path from node to LCA is within distance y.

Reference:
Miao Qiao, Hong Cheng, Lu Qin, Jeffrey Xu Yu, Philip S. Yu and Lijun Chang. "Computing Weight Constraint Reachability in Large Networks". The VLDB Journal (VLDBJ), Vol. 22, Issue 3, pages 275-294, 2013.
