#include "dijkprimkrus.h"

// Dijkstra's algorithmAdd commentMore actions
// Precondition: 'matrix' is a square matrix with non-negative weights; 'cities' size matches matrix dimensions; 'start_city' exists in 'cities'.
// Postcondition: Outputs or stores the shortest path from 'start_city' to all other cities.
void dijkstra(const vector<vector<int>>& matrix, const vector<string>& cities, const string& start_city) {
    cout << "---Dijkstra's Algorithm (starting from Seattle)---" << endl;

    int n = cities.size();
    vector<int> dist(n, INF);
    vector<int> prev(n, -1);                //record a tree of paths formed from dijkstra algorithm
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[0] = 0;  // Starting point is Seattle (index 0)
    pq.push({0, 0});

    while (!pq.empty()) {
        int u = pq.top().second;            //city index
        int d = pq.top().first;             //distance from start city
        pq.pop();

        if (d > dist[u]) continue;          //if the stored dist is less, skip

        //find all connection of u by traversing matrix with v
        for (int v = 0; v < n; v++) {
            //if that connection is finite
            //if the current identified shortest path is longer than
            //distance of connection plus distance of the u from start pt
            if (matrix[u][v] != INF && dist[v] > dist[u] + matrix[u][v]) {
                dist[v] = dist[u] + matrix[u][v];
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    //print
    for (int i = 0; i < n; i++) {
        cout << cities[i] << ": " << dist[i] << " {";
        int cur = i;
        vector<string> path;
        while (cur != -1) {
            path.push_back(cities[cur]);
            cur = prev[cur];
        }
        for (int j = path.size() - 1; j >= 0; j--) {
            cout << path[j];
            if (j > 0) cout << ", ";
        }
        cout << "}" << endl;
    }
}

// Prim's algorithm
// Precondition: 'matrix' is a square matrix with symmetric and non-negative weights; 'cities' size matches matrix dimensions.
// Postcondition: Outputs or stores the Minimum Spanning Tree (MST) of the graph.
void prim(const vector<vector<int>>& matrix, const vector<string>& cities) {
    cout << "\n---Prim's MST---" << endl;

    int n = cities.size();
    vector<bool> visited(n, false);
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;
    int total_cost = 0;
    int edge_count = 0;

    pq.push({0, {0, -1}});  // (cost, {current_node, parent})
    while (!pq.empty()) {
        int u = pq.top().second.first;              //current
        int cost = pq.top().first;                  //cost
        int parent = pq.top().second.second;        //parent
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        if (parent != -1) {
            cout << ++edge_count << ": (" << cities[parent] << ", " << cities[u] << ") " << cost << endl;
            total_cost += cost;
        }

        for (int v = 0; v < n; v++) {
            if (!visited[v] && matrix[u][v] != INF) {
                pq.push({matrix[u][v], {v, u}});
            }
        }
    }

    cout << "Total mileage: " << total_cost << endl;
}

// Find function used in union-find structure for Kruskal's algorithm
// Precondition: 'parent' is a valid vector with initialized parent elements.
// Postcondition: Returns the root representative of the element 'u'.
int find(int u, vector<int>& parent) {
    while (parent[u] != u) {
        parent[u] = parent[parent[u]];
        u = parent[u];
    }
    return u;
}

// Kruskal's algorithm
// Precondition: 'matrix' is a square matrix with non-negative weights; 'cities' size matches matrix dimensions.
// Postcondition: Outputs or stores the Minimum Spanning Tree (MST) of the graph.
void kruskal(const vector<vector<int>>& matrix, const vector<string>& cities) {
    cout << "\n---Kruskal's MST---" << endl;

    int n = cities.size();
    vector<int> parent(n);
    for (int i = 0; i < n; i++) parent[i] = i;

    vector<pair<int, pair<int, int>>> edges;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (matrix[i][j] != INF) {
                edges.push_back({matrix[i][j], {i, j}});
            }
        }
    }

    sort(edges.begin(), edges.end());

    int total_cost = 0;
    int edge_count = 0;


    for (int i = 0; i < edges.size(); i++) {
        int u = edges[i].second.first;
        int v = edges[i].second.second;
        int cost = edges[i].first;

        int root_u = find(u, parent);
        int root_v = find(v, parent);

        //if two node are not connected (do not have same root)
        //connect them, since the vector is already sorted from lowest cost to highest
        if (root_u != root_v) {
            parent[root_u] = root_v;
            cout << ++edge_count << ": (" << cities[u] << ", " << cities[v] << ") " << cost << endl;
            total_cost += cost;
        }
    }

    cout << "Total mileage: " << total_cost << endl;
}
