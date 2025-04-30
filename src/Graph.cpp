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
void Graph::deleteCity(string name)
{

        unordered_map<string, vector<pair<string, int>>>::iterator it;

        for (it = adjList.begin(); it != adjList.end(); ++it) {
            vector<pair<string, int>> newNeighbors;

            for (const auto& neighbor : it->second) {
                if (neighbor.first != name) {
                    newNeighbors.push_back(neighbor);
                }
            }

            it->second = newNeighbors;
        }

        adjList.erase(name);

  
}
void Graph::deleteEdge(string from, string  to) {

    cout << "Enter the first city: ";
    cin >> from;
    cout << "Enter the second city: ";
    cin >> to;

    // Check if the edge exists
    if (adjList.find(from) == adjList.end() || adjList.find(to) == adjList.end()) {
        cout << "One or both cities not found in the graph.\n";
        return;
    }

    // Remove the edge from 'from' to 'to' cities
    auto& neighbors = adjList[from];
    neighbors.erase(remove_if(neighbors.begin(), neighbors.end(),
        [&to](const pair<string, int>& neighbor) { return neighbor.first == to; }),
        neighbors.end());

    // For undirected graph, remove the edge in the opposite direction as well
    auto& neighborsTo = adjList[to];
    neighborsTo.erase(remove_if(neighborsTo.begin(), neighborsTo.end(),
        [&from](const pair<string, int>& neighbor) { return neighbor.first == from; }),
        neighbors.end());

    cout << "Edge between " << from << " and " << to << " removed successfully.\n";
}