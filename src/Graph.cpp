#include "../include/Graph.hpp"
#include <iostream>
#include <fstream>  // For file operations
#include <climits>
#include <queue>     // For BFS and Dijkstra
#include <stack>     // For DFS
#include <algorithm>
#include <cctype>    // For toupper

using namespace std;

// Helper function to standardize city names (convert to uppercase)
string Graph::standardizeCity(const string& city) {
    string result = city;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// Check if a city exists in the graph
bool Graph::cityExists(const string& city) const {
    string standardizedCity = standardizeCity(city);
    return adjList.find(standardizedCity) != adjList.end();
}

// Add a city (node) to the graph
void Graph::addCity(const string& city) {
    string standardizedCity = standardizeCity(city);
    
    if (adjList.find(standardizedCity) == adjList.end()) {
        adjList[standardizedCity];  // Ensures the city exists by adding an empty list if not already present
        cout << "City " << standardizedCity << " added.\n";
    } else {
        cout << "City " << standardizedCity << " already exists.\n";
    }
}

// Add an edge (connection) between two cities with a distance
void Graph::addEdge(const string& from, const string& to, int dist) {
    string standardizedFrom = standardizeCity(from);
    string standardizedTo = standardizeCity(to);
    
    // Ensure both cities exist in the graph before adding an edge
    if (!cityExists(standardizedFrom)) {
        cout << "City " << standardizedFrom << " not found. Please add the city first.\n";
        return;
    }
    if (!cityExists(standardizedTo)) {
        cout << "City " << standardizedTo << " not found. Please add the city first.\n";
        return;
    }
    
    //handles duplication of an edge
    for (auto it = adjList[standardizedFrom].begin(); it != adjList[standardizedFrom].end(); ++it) {
        if (it->first == standardizedTo) {
            cout << "Edge already exists from " << standardizedFrom << " to " << standardizedTo << ".\n";
            return;
        }
    }

    adjList[standardizedFrom].emplace_back(standardizedTo, dist);  // Add edge only from 'from' to 'to' (directed)
    cout << "Directed edge added from " << standardizedFrom << " to " << standardizedTo << " with distance " << dist << ".\n";
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
    string standardizedName = standardizeCity(name);
    
    if (!cityExists(standardizedName)) {
        cout << "City " << standardizedName << " not found in the graph.\n";
        return;
    }

    // Remove all edges connected to this city
    for (auto& [city, neighbors] : adjList) {
        neighbors.erase(
            remove_if(neighbors.begin(), neighbors.end(),
                [&standardizedName](const pair<string, int>& neighbor) { 
                    return neighbor.first == standardizedName; 
                }
            ),
            neighbors.end()
        );
    }

    // Remove the city
    adjList.erase(standardizedName);
    cout << "City " << standardizedName << " and all its connections have been removed.\n";
}

void Graph::deleteEdge(string from, string to) {
    string standardizedFrom = standardizeCity(from);
    string standardizedTo = standardizeCity(to);
    
    // Check if the cities exist
    if (!cityExists(standardizedFrom)) {
        cout << "City " << standardizedFrom << " not found in the graph.\n";
        return;
    }
    if (!cityExists(standardizedTo)) {
        cout << "City " << standardizedTo << " not found in the graph.\n";
        return;
    }

    // Remove the directed edge from 'from' to 'to'
    auto& neighbors = adjList[standardizedFrom];
    auto sizeBefore = neighbors.size();
    
    neighbors.erase(
        remove_if(neighbors.begin(), neighbors.end(),
            [&standardizedTo](const pair<string, int>& neighbor) { 
                return neighbor.first == standardizedTo; 
            }
        ),
        neighbors.end()
    );
    
    if (neighbors.size() < sizeBefore) {
        cout << "Directed edge from " << standardizedFrom << " to " << standardizedTo << " removed successfully.\n";
    } else {
        cout << "No edge found from " << standardizedFrom << " to " << standardizedTo << ".\n";
    }
}