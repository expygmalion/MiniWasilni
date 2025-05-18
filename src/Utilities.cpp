#include "../include/Utilities.hpp"
#include "../include/Graph.hpp"
#include <iostream>
#include <limits>

using namespace std;

bool Utilities::isBackCommand(const string& input) {
    return input == "back" || input == "BACK" || input == "Back";
}

void Utilities::displayMenu() {
    cout << "\n=== MiniWasilni Menu ===\n";
    cout << "1. Add City\n";
    cout << "2. Add Edge\n";
    cout << "3. Delete City\n";
    cout << "4. Delete Edge\n";
    cout << "5. Display Graph\n"; 
    cout << "6. Traverse Graph\n"; 
    cout << "7. Find Shortest Path\n";
    cout << "8. Save\n"; 
    cout << "9. Load\n"; 
    cout << "10. Exit\n";
    cout << "11. Sort all connections by city name\n";
    cout << "12. Sort all connections by distance\n";
    cout << "Select option: ";
}

Utilities::Utilities() {
    cout << "Utilities constructor called" << endl;
}

bool Utilities::goBack(const string& input) {
    return input == "BACK";
}

int Utilities::getMenuChoice() {
    int choice;
    cin >> choice;
    
    if (cin.fail()) {
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Try again.\n";
        return -1;  // Invalid choice indicator
    }
    
    return choice;
}

string Utilities::getFileName(const string& prompt) {
    string filename;
    
    cout << prompt << " (or type 'back' to return to main menu): ";
    cin >> filename;
    
    // Check if user wants to go back
    if (isBackCommand(filename)) {
        return "BACK";
    }
    
    return filename;
}

string Utilities::getCityInput(const string& prompt, const Graph& g, bool shouldExist) {
    string city;
    bool validInput = false;
    
    while (!validInput) {
        cout << prompt << " (or type 'back' to return to main menu): ";
        cin >> city;
        
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again.\n";
            continue;
        }
        
        // Check if user wants to go back
        if (isBackCommand(city)) {
            return "BACK";
        }
        
        if (shouldExist && !g.cityExists(city)) {
            cout << "City " << Graph::standardizeCity(city) << " does not exist in the graph. Please try again.\n";
            continue;
        }
        
        validInput = true;
    }
    
    return city;
}

int Utilities::getIntInput(const string& prompt) {
    int value;
    string input;
    bool validInput = false;
    
    while (!validInput) {
        cout << prompt << " (or type 'back' to return to main menu): ";
        cin >> input;
        
        if (isBackCommand(input)) {
            return -1;  // Special value indicating to go back
        }
        
        // Try to convert to integer
        try {
            value = stoi(input);
            if (value < 0) {
                cout << "Invalid input. Please enter a non-negative number.\n";
                continue;
            }
            validInput = true;
        } catch (...) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    return value;
}