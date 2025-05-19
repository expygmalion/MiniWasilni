#include "../include/Traversal.hpp"
#include <iostream>
#include <limits>

using namespace std;

void Traversal::traverse(Graph& graph) {
    int choice;
    string startCity, input;
    
    cout << "\n=== Traversal Options ===\n";
    cout << "1. Depth First Search (DFS)\n";
    cout << "2. Breadth First Search (BFS)\n";
    cout << "3. Back to Main Menu\n";
    cout << "Select traversal type (1-3): ";
    cin >> input;
    
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
        return;
    }
    
    // Check if input is "back"
    if (input == "back" || input == "BACK" || input == "Back" || input == "3") {
        return;
    }
    
    // Try to convert to integer
    try {
        choice = stoi(input);
    } catch (...) {
        cout << "Invalid input. Returning to main menu.\n";
        return;
    }
    
    if (choice < 1 || choice > 2) {
        cout << "Invalid choice. Returning to main menu.\n";
        return;
    }
    
    // Get start city with validation
    bool validCity = false;
    while (!validCity) {
        cout << "Enter starting city (or type 'back' to return to main menu): ";
        cin >> startCity;
        
        // Check if user wants to go back
        if (startCity == "back" || startCity == "BACK" || startCity == "Back") {
            return;
        }
        
        // Standardize the city name
        startCity = Graph::standardizeCity(startCity);
        
        if (!graph.cityExists(startCity)) {
            cout << "City " << startCity << " not found in the graph. Please try again.\n";
            continue;
        }
        
        validCity = true;
    }
    
    switch (choice) {
        case 1:
            dfs(graph, startCity);
            break;
        case 2:
            bfs(graph, startCity);

            bfs(graph, startCity);

            break;
        default:
            cout << "Invalid choice. Please select 1 or 2.\n";
    }
}

void Traversal::dfs(Graph& graph, const string& startCity) {
    // Get the adjacency list from the graph
    const auto& adjList = graph.getAdjList();
    string standardCity = Graph::standardizeCity(startCity);
    
    if (adjList.find(standardCity) == adjList.end()) {
        cout << "Start city not found in the graph.\n";
        return;
    }

    unordered_set<string> visited;
    stack<string> s;
    
    // Push the start city onto the stack
    s.push(standardCity);
    
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
	string standardCity = Graph::standardizeCity(startCity);
	
	if (adjList.find(standardCity) == adjList.end()) {
		cout << "Start city not found in the graph.\n";
		return;
	}

	unordered_set<string> visited;
	queue<string> q;
	
	// Push the start city onto the queue
	q.push(standardCity);
	
	cout << "BFS Visit Order:\n";
	
	while (!q.empty()) {
		string current = q.front();
		q.pop();
		
		// If the current city hasn't been visited
		if (visited.find(current) == visited.end()) {
			visited.insert(current);
			cout << "Visiting: " << current << endl;
			
			// Push all unvisited neighbors onto the queue
			for (const auto& neighbor : adjList.at(current)) {
				if (visited.find(neighbor.first) == visited.end()) {
					q.push(neighbor.first);
				}
			}
		}
	}
	
	cout << "BFS traversal completed.\n";
}

// DFS with path capturing for GUI
void Traversal::dfsWithPath(const Graph& graph, const string& startCity, vector<string>& path) {
    path.clear();
    
    // Get the adjacency list from the graph
    const auto& adjList = graph.getAdjList();
    string standardCity = Graph::standardizeCity(startCity);
    
    if (adjList.find(standardCity) == adjList.end()) {
        return;  // Start city not found
    }
    
    unordered_set<string> visited;
    stack<string> s;
    
    // Push the start city onto the stack
    s.push(standardCity);
    
    while (!s.empty()) {
        string current = s.top();
        s.pop();
        
        // If the current city hasn't been visited
        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            path.push_back(current);  // Add to path
            
            // Push all unvisited neighbors onto the stack (reverse order for correct DFS)
            if (adjList.find(current) != adjList.end()) {
                for (auto it = adjList.at(current).rbegin(); it != adjList.at(current).rend(); ++it) {
                    string neighbor = it->first;
                    if (visited.find(neighbor) == visited.end()) {
                        s.push(neighbor);
                    }
                }
            }
        }
    }
}

// BFS with path capturing for GUI
void Traversal::bfsWithPath(const Graph& graph, const string& startCity, vector<string>& path) {
    path.clear();
    
    // Get the adjacency list from the graph
    const auto& adjList = graph.getAdjList();
    string standardCity = Graph::standardizeCity(startCity);
    
    if (adjList.find(standardCity) == adjList.end()) {
        return;  // Start city not found
    }
    
    unordered_set<string> visited;
    queue<string> q;
    
    // Enqueue the start city
    q.push(standardCity);
    visited.insert(standardCity);
    
    while (!q.empty()) {
        string current = q.front();
        q.pop();
        
        path.push_back(current);  // Add to path
        
        // Enqueue all unvisited neighbors
        if (adjList.find(current) != adjList.end()) {
            for (const auto& [neighbor, _] : adjList.at(current)) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.push(neighbor);
                }
            }
        }
    }
}
