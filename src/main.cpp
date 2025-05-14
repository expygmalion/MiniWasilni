#include "../include/Graph.hpp"  
#include "../include/IOManager.hpp"
#include "../include/Traversal.hpp"
#include <iostream>
#include <string>

using namespace std;



void displayMenu() {
    cout << "\n=== MiniWasilni Menu ===\n";
    cout << "1. Add City\n";
    cout << "2. Add Edge\n";
    cout << "3. Display Graph\n"; 
    cout << "4. Traverse Graph\n"; 
    cout << "5. Shortest Path (Not implemented)\n"; // work on this
    cout << "6. Delete City\n";
    cout << "7. Delete Edge\n";
    cout << "8. Save\n"; 
    cout << "9. Load\n"; 
    cout << "10. Exit\n";
    cout << "Select option: ";
}


//hello there
int main() {


    Graph g;
    int choice;
    string filename; 
    IOManager ioManager;


    while (true) {
        displayMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        if (choice == 1) {
            string city;
            cout << "Enter city name: ";
            cin >> city;
            g.addCity(city);
            continue;
        }
        else if (choice == 2) {
            string from, to;
            int dist;
            cout << "From: "; cin >> from;
            cout << "To: "; cin >> to;
            cout << "Distance: "; cin >> dist;
            g.addEdge(from, to, dist);
            continue;
        }
        else if (choice == 3) {
            g.display();
            continue;
        }
        else if (choice == 4) {
            Traversal::traverse(g);
            continue;
        }
        else if (choice == 5) {
            cout << "Shortest path feature not implemented yet.\n";
            continue;
        }
        else if (choice == 6) {
            string city;
            cout << "Enter city name to delete: ";
            cin >> city;
            g.deleteCity(city);
            continue;
        }
        else if (choice == 7) {
            string from, to;
            cout << "From: "; cin >> from;
            cout << "To: "; cin >> to;
            g.deleteEdge(from, to);
            continue;
        }
        else if (choice == 8) {
            cout << "Enter filename to save (e.g., data/graph.txt): ";
            cin >> filename;
            ioManager.saveGraph(filename,g);
            continue;
        }   
        else if (choice == 9) {
            cout << "Enter filename to load (e.g., data/graph.txt): ";
            cin >> filename;
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

  