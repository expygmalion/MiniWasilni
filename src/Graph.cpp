#include "../include/Graph.hpp"
#include <iostream>
#include <fstream>  // For file operations
#include <climits>
#include <queue>     // For BFS and Dijkstra
#include <stack>     // For DFS
#include <algorithm>

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
    //handles duplication of an edge
    for (auto it = adjList[from].begin(); it != adjList[from].end(); ++it) {
        if (it->first == to && it->second == dist) {
            cout << "Edge already exists.\n";
            return;
        }
    }

    // Ensure both cities exist in the graph before adding an edge
    if (adjList.find(from) == adjList.end()) {
        cout << "City " << from << " not found. Please add the city first.\n";
        return;
    }
    if (adjList.find(to) == adjList.end()) {
        cout << "City " << to << " not found. Please add the city first.\n";
        return;
    }

    adjList[from].emplace_back(to, dist);  // Add edge only from 'from' to 'to' (directed)
    cout << "Directed edge added from " << from << " to " << to << " with distance " << dist << ".\n";
}

// Display the graph (cities and their connections)
void Graph::display() const {
    if (adjList.empty()) {
        cout << "The graph is empty.\n";
        return;
    }

    cout << "\n=== Directed Graph Structure ===\n";
    cout << "Cities and their outgoing connections:\n";
    for (const auto& [city, neighbors] : adjList) {
        cout << "\n" << city << " → ";
        if (neighbors.empty()) {
            cout << "No outgoing connections";
        } else {
            for (size_t i = 0; i < neighbors.size(); ++i) {
                const auto& [neighbor, dist] = neighbors[i];
                cout << neighbor << " (dist: " << dist << ")";
                if (i < neighbors.size() - 1) {
                    cout << ", ";
                }
            }
        }
        cout << "\n";
    }
    cout << "\nTotal cities: " << adjList.size() << "\n";
}

void Graph::deleteCity(string name) {
    if (adjList.find(name) == adjList.end()) {
        cout << "City " << name << " not found in the graph.\n";
        return;
    }

    // Remove all edges connected to this city
    for (auto& [city, neighbors] : adjList) {
        neighbors.erase(
            remove_if(neighbors.begin(), neighbors.end(),
                [&name](const pair<string, int>& neighbor) { 
                    return neighbor.first == name; 
                }
            ),
            neighbors.end()
        );
    }

    // Remove the city
    adjList.erase(name);
    cout << "City " << name << " and all its connections have been removed.\n";
}

void Graph::deleteEdge(string from, string to) {
    // Check if the cities exist
    if (adjList.find(from) == adjList.end()) {
        cout << "City " << from << " not found in the graph.\n";
        return;
    }
    if (adjList.find(to) == adjList.end()) {
        cout << "City " << to << " not found in the graph.\n";
        return;
    }

    // Remove the directed edge from 'from' to 'to'
    auto& neighbors = adjList[from];
    auto sizeBefore = neighbors.size();
    
    neighbors.erase(
        remove_if(neighbors.begin(), neighbors.end(),
            [&to](const pair<string, int>& neighbor) { 
                return neighbor.first == to; 
            }
        ),
        neighbors.end()
    );
    
    if (neighbors.size() < sizeBefore) {
        cout << "Directed edge from " << from << " to " << to << " removed successfully.\n";
    } else {
        cout << "No edge found from " << from << " to " << to << ".\n";
    }
}