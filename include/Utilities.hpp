#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "Graph.hpp"
#include <string>

using namespace std;

class Utilities {
public:
    Utilities();
    void displayMenu();
    string getCityInput(const string& prompt, const Graph& g, bool shouldExist = false);
    int getIntInput(const string& prompt);
    bool isBackCommand(const string& input);
    int getMenuChoice();
    string getFileName(const string& prompt);
    bool goBack(const string& input);
};

#endif