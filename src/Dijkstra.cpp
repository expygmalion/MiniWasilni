#include "../include/Dijkstra.hpp"
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;


void Dijkstra::dijkstra(Graph& graph, const string& startCity, const string& endCity) {
    const auto& adjList = graph.getAdjList();
    
    // Standardize city names
    string standardStart = Graph::standardizeCity(startCity);
    string standardEnd = Graph::standardizeCity(endCity);
    
    // Check if cities exist
    if (!graph.cityExists(standardStart) || !graph.cityExists(standardEnd)) {
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
    distances[standardStart] = 0;
    pq.push({0, standardStart});
    
    while (!pq.empty()) {
        string current = pq.top().second;
        int currentDist = pq.top().first;
        pq.pop();
        
        // If we've found a better path to current, skip
        if (currentDist > distances[current]) continue;
        
        // If we've reached the end city, we're done
        if (current == standardEnd) break;
        
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
    if (distances[standardEnd] == numeric_limits<int>::max()) {
        cout << "No path exists between " << standardStart << " and " << standardEnd << ".\n";
        return;
    }
    
    // Reconstruct and display the path
    vector<string> path = reconstructPath(previous, standardStart, standardEnd);
    displayPath(path, distances[standardEnd]);
}

// @rawanizzeldin write reconstructPath function here
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

// @expygmalion 
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


/// @mishkatzaki write aStar function here

