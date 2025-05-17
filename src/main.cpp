#include "../include/Graph.hpp"
#include "../include/IOManager.hpp"
#include "../include/Traversal.hpp"
#include "../include/AStar.hpp"
#include <iostream>
#include <string>

using namespace std;

void displayMenu()
{
    cout << "\n=== MiniWasilni Menu ===\n";
    cout << "1. Add City\n";
    cout << "2. Add Edge\n";
    cout << "3. Display Graph\n";
    cout << "4. Traverse Graph (DFS / BFS)\n";
    cout << "5. Shortest Path (A*)\n";
    cout << "6. Delete City\n";
    cout << "7. Delete Edge\n";
    cout << "8. Save\n";
    cout << "9. Load\n";
    cout << "10. Test Save/Load\n"; // Switched
    cout << "11. Exit\n";           // Switched
    cout << "Select option: ";
}

void testSaveLoad(Graph &g, IOManager &ioManager)
{
    g = Graph();
    g.addCity("Khartoum");
    g.addCity("PortSudan");
    g.addCity("Omdurman");
    g.addEdge("Khartoum", "PortSudan", 100);
    g.addEdge("PortSudan", "Omdurman", 200);

    cout << "Original graph:\n";
    g.display();

    ioManager.saveGraph("data/test_graph.txt", g);

    g = Graph();
    cout << "\nGraph after clearing:\n";
    g.display();

    ioManager.loadGraph("data/test_graph.txt", g);
    cout << "\nGraph after loading:\n";
    g.display();
}

int main()
{
    Graph g;
    IOManager ioManager;
    int choice;
    string filename;

    while (true)
    {
        displayMenu();
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        if (choice == 1)
        {
            string city;
            cout << "Enter city name: ";
            cin >> city;
            g.addCity(city);
        }
        else if (choice == 2)
        {
            string from, to;
            int dist;
            cout << "From: ";
            cin >> from;
            cout << "To: ";
            cin >> to;
            cout << "Distance: ";
            cin >> dist;
            g.addEdge(from, to, dist);
        }
        else if (choice == 3)
        {
            g.display();
        }
        else if (choice == 4)
        {
            Traversal::traverse(g);
        }
        else if (choice == 5)
        {
            string start, goal;
            cout << "Enter start city: ";
            cin >> start;
            cout << "Enter goal city: ";
            cin >> goal;

            vector<string> path = AStar::findPath(g, start, goal);
            if (path.empty())
            {
                cout << "No path found.\n";
            }
        }
        else if (choice == 6)
        {
            string city;
            cout << "Enter city name to delete: ";
            cin >> city;
            g.deleteCity(city);
        }
        else if (choice == 7)
        {
            string from, to;
            cout << "From: ";
            cin >> from;
            cout << "To: ";
            cin >> to;
            g.deleteEdge(from, to);
        }
        else if (choice == 8)
        {
            cout << "Enter filename to save (e.g., data/graph.txt): ";
            cin >> filename;
            ioManager.saveGraph(filename, g);
        }
        else if (choice == 9)
        {
            cout << "Enter filename to load (e.g., data/graph.txt): ";
            cin >> filename;
            ioManager.loadGraph(filename, g);
        }
        else if (choice == 10)
        {
            testSaveLoad(g, ioManager); // now option 10
        }
        else if (choice == 11)
        {
            break; // now option 11
        }
        else
        {
            cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}