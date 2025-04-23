#include "../include/Graph.hpp"
#include <iostream>


using namespace std;



void displayMenu() {
    cout << "\n=== MiniWasilni Menu ===\n";
    cout << "1. Add City\n";
    cout << "2. Add Edge\n";
    cout << "3. Display Graph\n";
    cout << "4. Traverse\n";
    cout << "5. Shortest Path\n";
    cout << "6. Save\n";
    cout << "7. Load\n";
    cout << "8. Exit\n";
    cout << "Select option: ";
}

int main() {
    Graph g;
    int choice;

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
        }
        else if (choice == 2) {
            string from, to;
            int dist;
            cout << "From: "; cin >> from;
            cout << "To: "; cin >> to;
            cout << "Distance: "; cin >> dist;
            g.addEdge(from, to, dist);
        }
        else if (choice == 3) {
            g.display();
        }
        else if (choice == 8) {
            break;
        }
        else {
            cout << "Feature not implemented yet.\n";
        }
    }

    return 0;
}

  