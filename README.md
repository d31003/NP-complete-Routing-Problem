# NP-complete-Routing-Problem
An NP-complete Routing Algorithm based on SSSP Algorithm
This is a Routing Problem given a plate grids with **size axa, maximum capacity of the network, number of nets,
and all the nets' starting and terminal coordinates.**

Our goal is to minimize  **overflow(numbers of route in the same grid < maximum capacity of the network)**,
then minimize **the path in total.**

The solution is based on **Dijkstra's Algorithm** of SSSP, and **Greedy Algorithm**, and optimize by heap property.
