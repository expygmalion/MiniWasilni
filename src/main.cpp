#include "../include/Graph.hpp"  
#include "../include/IOManager.hpp"
#include "../include/Traversal.hpp"
#include "../include/Dijkstra.hpp"
#include "../include/Utilities.hpp"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

int main() {
    Graph g;
    int choice;
    string filename; 
    IOManager ioManager;
    Utilities utilities;

    while (true) {
        utilities.displayMenu();
        
        choice = utilities.getMenuChoice();
        
        if (choice == -1) continue; // Invalid input, try again

        if (choice == 1) {
            string city = utilities.getCityInput("Enter city name: ", g);
            if (utilities.goBack(city)) continue;
            g.addCity(city);
            continue;
        }
        else if (choice == 2) {
            string from = utilities.getCityInput("From: ", g, true);
            if (utilities.goBack(from)) continue;
            string to = utilities.getCityInput("To: ", g, true);
            if (utilities.goBack(to)) continue;
            int dist = utilities.getIntInput("Distance: ");
            if (dist == -1) continue;
            g.addEdge(from, to, dist);
            continue;
        }
        else if (choice == 3) {
            string city = utilities.getCityInput("Enter city name to delete: ", g, true);
            if (utilities.goBack(city)) continue;
            g.deleteCity(city);
            continue;
        }
        else if (choice == 4) {
            string from = utilities.getCityInput("From: ", g, true);
            if (utilities.goBack(from)) continue;
            string to = utilities.getCityInput("To: ", g, true);
            if (utilities.goBack(to)) continue;
            g.deleteEdge(from, to);
            continue;
        }
        else if (choice == 5) {
            g.display();
            continue;
        }
        else if (choice == 6) {
            Traversal::traverse(g);
            continue;
        }
        else if (choice == 7) {
            Dijkstra::findPath(g);
            continue;
        }
        else if (choice == 8) {
            filename = utilities.getFileName("Enter filename to save (e.g., data/graph.txt)");
            if (utilities.goBack(filename)) continue;
            ioManager.saveGraph(filename,g);
            continue;
        }   
        else if (choice == 9) {
            filename = utilities.getFileName("Enter filename to load (e.g., data/graph.txt)");
            if (utilities.goBack(filename)) continue;
            ioManager.loadGraph(filename,g);
            continue;
        }
        else if (choice == 10) {
            break;
        }
        else {
            cout << "Feature not implemented yet.\n";
        }
    }
    return 0;
}

  