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


void dijkstra(const vector<vector<int>>& matrix, const vector<string>& cities, const string& start_city);

void prim(const vector<vector<int>>& matrix, const vector<string>& cities);

void kruskal(const vector<vector<int>>& matrix, const vector<string>& cities);

int find(int u, vector<int>& parent);



#endif