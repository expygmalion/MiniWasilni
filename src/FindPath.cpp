#include "../include/FindPath.hpp"
#include "../include/Dijkstra.hpp"
#include "../include/AStar.hpp"
#include <iostream>
#include <limits>

using namespace std;

void FindPath::UniversalFind(Graph& graph) {




    int choice;
    string startCity, endCity;
    
    cout << "\n=== Path Finding Algorithms ===\n";
    cout << "1. Dijkstra's Algorithm\n";
    cout << "2. A* Algorithm\n";
    cout << "Select algorithm (1-2): ";
    cin >> choice;
    
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
        return;
    }
    
    // Get valid start city
    bool validStart = false;
    while (!validStart) {
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
        validStart = true;
    }
    
    // Get valid end city
    bool validEnd = false;
    while (!validEnd) {
        cout << "Enter destination city (or type 'back' to return to main menu): ";
        cin >> endCity;
        
        // Check if user wants to go back
        if (endCity == "back" || endCity == "BACK" || endCity == "Back") {
            return;
        }
        
        // Standardize the city name
        endCity = Graph::standardizeCity(endCity);
        // Verify city exists in the graph
        if (!graph.cityExists(endCity)) {
            cout << "City " << endCity << " not found in the graph. Please try again.\n";
            continue;
        }
        validEnd = true;
    }
    
    switch (choice) {
        case 1:
            Dijkstra::dijkstra(graph, startCity, endCity);
            break;
        case 2:
            AStar::findPath(graph, startCity, endCity);
            break;
        default:
            cout << "Invalid choice. Please select 1 or 2.\n";
    }
}

