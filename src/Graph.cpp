#include "../include/Graph.hpp"
#include <iostream>

using namespace std;

void Graph::addCity(const string& city) {
    adjList[city]; // ensures the city exists
}

void Graph::addEdge(const string& from, const string& to, int distance) {
    adjList[from].emplace_back(to, distance);
    adjList[to].emplace_back(from, distance); // assume undirected graph
}

void Graph::display() const {
    for (const auto& [city, neighbors] : adjList) {
        cout << city << " -> ";
        for (const auto& [neighbor, dist] : neighbors) {
            cout << "(" << neighbor << ", " << dist << ") ";
        }
        cout << "\n";
    }
}
