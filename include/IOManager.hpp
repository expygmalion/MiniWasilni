#ifndef IOMANAGER_HPP
#define IOMANAGER_HPP
#include "Graph.hpp"
#include <string>
#include <fstream> // For file operations
#include <sstream> // For string stream operations
#include <iostream> 

using namespace std;

// File save/load logic here .

class IOManager {

	IOManager();
public:

	void saveGraph(const string& filename, const Graph& graph);
	void loadGraph(const string& filename, Graph& graph);

};

#endif
