#include "../include/Dijkstra.hpp"
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

void Dijkstra::findPath(Graph& graph) {
    int choice;
    string startCity, endCity;
    
    cout << "\n=== Path Finding Algorithms ===\n";
    cout << "1. Dijkstra's Algorithm\n";
    cout << "2. A* Algorithm (Not implemented)\n";
    cout << "Select algorithm (1-2): ";
    cin >> choice;
    
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Please enter a number.\n";
        return;
    }
    
    cout << "Enter starting city: ";
    cin >> startCity;
    cout << "Enter destination city: ";
    cin >> endCity;
    
    switch (choice) {
        case 1:
            dijkstra(graph, startCity, endCity);
            break;
        case 2:
            aStar(graph, startCity, endCity);
            break;
        default:
            cout << "Invalid choice. Please select 1 or 2.\n";
    }
}

void Dijkstra::dijkstra(Graph& graph, const string& startCity, const string& endCity) {
    const auto& adjList = graph.getAdjList();
    
    // Check if cities exist
    if (adjList.find(startCity) == adjList.end() || adjList.find(endCity) == adjList.end()) {
        cout << "One or both cities not found in the graph.\n";
        return;
    }
    
    // Initialize data structures
    unordered_map<string, int> distances;
    unordered_map<string, string> previous;
    priority_queue<pair<int, string>, 
                  vector<pair<int, string>>, 
                  greater<pair<int, string>>> pq;
    
    // Set initial distances
    for (const auto& [city, _] : adjList) {
        distances[city] = numeric_limits<int>::max();
    }
    distances[startCity] = 0;
    pq.push({0, startCity});
    
    while (!pq.empty()) {
        string current = pq.top().second;
        int currentDist = pq.top().first;
        pq.pop();
        
        // If we've found a better path to current, skip
        if (currentDist > distances[current]) continue;
        
        // If we've reached the end city, we're done
        if (current == endCity) break;
        
        // Check all neighbors
        for (const auto& [neighbor, dist] : adjList.at(current)) {
            int newDist = distances[current] + dist;
            
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                previous[neighbor] = current;
                pq.push({newDist, neighbor});
            }
        }
    }
    
    // If we couldn't reach the end city
    if (distances[endCity] == numeric_limits<int>::max()) {
        cout << "No path exists between " << startCity << " and " << endCity << ".\n";
        return;
    }
    
    // Reconstruct and display the path
    vector<string> path = reconstructPath(previous, startCity, endCity);
    displayPath(path, distances[endCity]);
}

void Dijkstra::aStar(Graph& graph, const string& startCity, const string& endCity) {
    cout << "A* algorithm is not implemented yet.\n";
}

vector<string> Dijkstra::reconstructPath(const unordered_map<string, string>& previous,
                                       const string& startCity,
                                       const string& endCity) {
    vector<string> path;
    string current = endCity;
    
    while (current != startCity) {
        path.push_back(current);
        current = previous.at(current);
    }
    path.push_back(startCity);
    
    reverse(path.begin(), path.end());
    return path;
}

void Dijkstra::displayPath(const vector<string>& path, int totalDistance) {
    cout << "\nShortest Path Found:\n";
    cout << "Total Distance: " << totalDistance << " units\n";
    cout << "Path: ";
    
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i < path.size() - 1) {
            cout << " -> ";
        }
    }
    cout << "\n";
} 