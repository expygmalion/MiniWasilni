#include "../include/Traversal.hpp"
#include <iostream>

using namespace std;

void Traversal::traverse(Graph& graph) {
    int choice;
    string startCity;
    
    cout << "\n=== Traversal Options ===\n";
    cout << "1. Depth First Search (DFS)\n";
    cout << "2. Breadth First Search (BFS)\n";
    cout << "Select traversal type (1-2): ";
    cin >> choice;
    
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Please enter a number.\n";
        return;
    }
    
    cout << "Enter starting city: ";
    cin >> startCity;
    
    switch (choice) {
        case 1:
            dfs(graph, startCity);
            break;
        case 2:
            bfs(graph, startCity);
            break;
        default:
            cout << "Invalid choice. Please select 1 or 2.\n";
    }
}

void Traversal::dfs(Graph& graph, const string& startCity) {
    // Get the adjacency list from the graph
    const auto& adjList = graph.getAdjList();
    
    if (adjList.find(startCity) == adjList.end()) {
        cout << "Start city not found in the graph.\n";
        return;
    }

    unordered_set<string> visited;
    stack<string> s;
    
    // Push the start city onto the stack
    s.push(startCity);
    
    cout << "DFS Visit Order:\n";
    
    while (!s.empty()) {
        string current = s.top();
        s.pop();
        
        // If the current city hasn't been visited
        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            cout << "Visiting: " << current << endl;
            
            // Push all unvisited neighbors onto the stack
            // We push them in reverse order to maintain the correct visit order
            for (auto it = adjList.at(current).rbegin(); it != adjList.at(current).rend(); ++it) {
                string neighbor = it->first;
                if (visited.find(neighbor) == visited.end()) {
                    s.push(neighbor);
                }
            }
        }
    }
    
    cout << "DFS traversal completed.\n";
}

void Traversal::bfs(Graph& graph, const string& startCity) {
    // Get the adjacency list from the graph
    const auto& adjList = graph.getAdjList();
    
    if (adjList.find(startCity) == adjList.end()) {
        cout << "Start city not found in the graph.\n";
        return;
    }

    unordered_set<string> visited;
    queue<string> q;
    
    // Add the start city to the queue and mark as visited
    q.push(startCity);
    visited.insert(startCity);
    
    cout << "BFS Visit Order:\n";
    cout << "Visiting: " << startCity << endl;
    
    while (!q.empty()) {
        string current = q.front();
        q.pop();
        
        // Visit all unvisited neighbors
        for (const auto& [neighbor, dist] : adjList.at(current)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
                cout << "Visiting: " << neighbor << endl;
            }
        }
    }
    
    cout << "BFS traversal completed.\n";
} 