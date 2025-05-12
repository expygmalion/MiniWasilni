#include "BSF.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

void BFS::loadFromWeightedGraph(const unordered_map<string, vector<pair<string, int>>>& weightedAdj) {
    adjList.clear();
    for (const auto& [city, neighbors] : weightedAdj) {
        for (const auto& [neighbor, _] : neighbors) {
            adjList[city].push_back(neighbor);
        }
    }
}

void BFS::traverse(const string& start) {
    unordered_map<string, bool> visited;
    queue<string> q;

    visited[start] = true;
    q.push(start);

    cout << "BFS traversal from " << start << ": ";

    while (!q.empty()) {
        string current = q.front();
        q.pop();
        cout << current << " ";

        for (const string& neighbor : adjList[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    cout << endl;
}

vector<string> BFS::shortestPath(const string& start, const string& end) {
    unordered_map<string, string> parent;
    unordered_map<string, bool> visited;
    queue<string> q;

    visited[start] = true;
    parent[start] = "";
    q.push(start);

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        if (current == end) break;

        for (const string& neighbor : adjList[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    vector<string> path;
    if (!visited[end]) {
        cout << "No path found from " << start << " to " << end << endl;
        return path;
    }

    for (string at = end; !at.empty(); at = parent[at]) {
        path.push_back(at);
    }

    reverse(path.begin(), path.end());
    return path;
}

void BFS::printPath(const vector<string>& path) {
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i != path.size() - 1) cout << " -> ";
    }
    cout << endl;
}
