# NP-complete-Routing-Problem
An NP-complete Routing Algorithm based on SSSP Algorithm
This is a Routing Problem given a plate grids with __**size axa, maximum capacity of the network, number of nets,
and all the nets' starting and terminal coordinates.**__

Our goal is to minimize  __**overflow(numbers of route in the same grid < maximum capacity of the network)**,
then minimize **the path in total.**__

The solution is based on __**Dijkstra's Algorithm** of SSSP, and **Greedy Algorithm**__, and optimize by heap property.
