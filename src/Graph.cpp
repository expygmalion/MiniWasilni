// Graph.cpp

#include "../include/Graph.hpp"
#include <iostream>
#include <fstream>  // For file operations
#include <climits>
#include <queue>     // For BFS and Dijkstra
#include <stack>     // For DFS

using namespace std;

// Add a city (node) to the graph
void Graph::addCity(const string& city) {
    if (adjList.find(city) == adjList.end()) {
        adjList[city];  // Ensures the city exists by adding an empty list if not already present
        cout << "City " << city << " added.\n";
    } else {
        cout << "City " << city << " already exists.\n";
    }
}

// Add an edge (connection) between two cities with a distance
void Graph::addEdge(const string& from, const string& to, int dist) {
    // Ensure both cities exist in the graph before adding an edge
    if (adjList.find(from) == adjList.end()) {
        cout << "City " << from << " not found. Please add the city first.\n";
        return;
    }
    if (adjList.find(to) == adjList.end()) {
        cout << "City " << to << " not found. Please add the city first.\n";
        return;
    }

    adjList[from].emplace_back(to, dist);  // Add edge from 'from' to 'to'
    adjList[to].emplace_back(from, dist);  // Since it's an undirected graph, add the reverse edge
    cout << "Edge added between " << from << " and " << to << " with distance " << dist << ".\n";
}

// Display the graph (cities and their connections)
void Graph::display() const {
    if (adjList.empty()) {
        cout << "The graph is empty.\n";
        return;
    }

    cout << "\n=== Graph ===\n";
    for (const auto& [city, neighbors] : adjList) {
        cout << city << " -> ";
        for (const auto& [neighbor, dist] : neighbors) {
            cout << "(" << neighbor << ", " << dist << ") ";
        }
        cout << "\n";
    }
}

//  TODO: add deleteCity and deleteEdge  functions : @Olaalx
