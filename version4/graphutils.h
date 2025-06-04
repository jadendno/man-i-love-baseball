// ========================
// 1. Place this helper in a new file (e.g., "graphutils.h") or inside mainwindow.cpp
// ========================

#ifndef GRAPHUTILS_H
#define GRAPHUTILS_H

#include "graphs.h"
#include "dijkprimkrus.h"
#include <vector>
#include <unordered_map>
#include <climits>

using namespace std;




inline void buildMatrixFromGraph(const Graphs& graph, vector<vector<int>>& matrix, vector<string>& cities) {
    const auto& adj = graph.getAdjList();

    unordered_map<string, int> indexMap;
    cities.clear();
    int index = 0;

    for (const auto& [name, _] : adj) {
        indexMap[name] = index++;
        cities.push_back(name);
    }

    int n = cities.size();
    matrix.assign(n, vector<int>(n, INF));

    for (const auto& [from, neighbors] : adj) {
        int i = indexMap[from];
        for (const auto& [to, dist] : neighbors) {
            int j = indexMap[to];
            matrix[i][j] = dist;
        }
    }
}

#endif // GRAPHUTILS_H
