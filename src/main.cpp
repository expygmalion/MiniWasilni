#include "../include/Graph.hpp"
#include <iostream>

    //added -> 12-5-25
#include "../include/BSF.hpp"

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


//hello there
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
//
//
    //   g.addCity("New York");
    //   g.addCity("Los Angeles");
    //   g.addCity("Chicago");

    //   g.addEdge("New York", "Los Angeles", 2800);
    //   g.addEdge("New York", "Chicago", 800);
    //   g.addEdge("Los Angeles", "Chicago", 2000);  // This should add the edge successfully

    //   g.display();

    //   // Attempt to add an edge with a non-existing city
    //   g.addEdge("Chicago", "Miami", 1300);  // This should display an error

    //  ma 8adra a3ml runt where is it mayb
    return 0;
}

  