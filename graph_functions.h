#ifndef GRAPH_FUNCTIONS_H
#define GRAPH_FUNCTIONS_H


#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <set>
#include <algorithm>

using namespace std;

const int INF = INT_MAX;
// Dijkstra's algorithm
// Parameters:
// - matrix: adjacency matrix representing the graph with edge weights
// - cities: list of city names corresponding to the nodes
// - start_city: the city to start the algorithm from
// Return type: void
void dijkstra(const vector<vector<int>>& matrix, const vector<string>& cities, const string& start_city);

// Prim's algorithm
// Parameters:
// - matrix: adjacency matrix representing the graph
// - cities: list of city names corresponding to the nodes
// Return type: void
void prim(const vector<vector<int>>& matrix, const vector<string>& cities);

// Kruskal's algorithm
// Parameters:
// - matrix: adjacency matrix representing the graph
// - cities: list of city names corresponding to the nodes
// Return type: void
void kruskal(const vector<vector<int>>& matrix, const vector<string>& cities);

// Find function used in union-find structure for Kruskal's algorithm
// Parameters:
// - u: the element to find the representative of
// - parent: reference to the parent vector used in union-find
// Return type: int (representative of the set containing u)
int find(int u, vector<int>& parent);



#endif