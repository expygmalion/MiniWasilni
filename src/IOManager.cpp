#include "../include/IOManager.hpp"
#include <filesystem>  // For directory operations
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>  // For exceptions
using namespace std;


// constructor

IOManager::IOManager() {
	// Create data directory if it doesn't exist
	if (!filesystem::exists("data")) {
		filesystem::create_directory("data");
		cout << "Created data directory\n";
	}
}

void IOManager::saveGraph(const string& filename, const Graph& graph)
{
	// Ensure the path is relative to the project root, not the build directory
	string filePath = filename;
	if (filename.find("data/") != 0 && filename.find("/") == string::npos) {
		filePath = "data/" + filename;
	}

	ofstream file(filePath);
	if (!file) {
		string errorMsg = "Error opening file for writing: " + filePath;
		cerr << errorMsg << endl;
		throw runtime_error(errorMsg);
	}
	
	// Save cities
	for (const auto& [city, neighbors] : graph.adjList) {
		file << city << endl;
	}
	
	// Check if file is still good
	if (!file) {
		string errorMsg = "Error writing cities to file: " + filePath;
		cerr << errorMsg << endl;
		throw runtime_error(errorMsg);
	}
	
	// Save edges
	for (const auto& [city, neighbors] : graph.adjList) {
		for (const auto& [neighbor, dist] : neighbors) {
			file << city << " " << neighbor << " " << dist << endl;
			
			// Periodically check if file is still good
			if (!file) {
				string errorMsg = "Error writing edges to file: " + filePath;
				cerr << errorMsg << endl;
				throw runtime_error(errorMsg);
			}
		}
	}
	
	file.close();
	cout << "Graph saved to " + filePath << endl;
}

void IOManager::loadGraph(const string& filename, Graph& graph)
{
	// Ensure the path is relative to the project root, not the build directory
	string filePath = filename;
	if (filename.find("data/") != 0 && filename.find("/") == string::npos) {
		filePath = "data/" + filename;
	}

	ifstream file(filePath);
	if (!file) {
		string errorMsg = "Error opening file for reading: " + filePath;
		cerr << errorMsg << endl;
		throw runtime_error(errorMsg);
	}
	
	// Clear existing graph first
	// Create a temporary empty graph and swap it with the current one
	Graph emptyGraph;
	graph = emptyGraph;
	
	string line;
	vector<string> cities;
	vector<tuple<string, string, int>> edges;
	
	// First pass: read the whole file and separate cities from edges
	while (getline(file, line)) {
		// Skip empty lines
		if (line.empty()) {
			continue;
		}
		
		istringstream iss(line);
		string first, second;
		int distance;
		
		if (iss >> first >> second >> distance) {
			// This is an edge
			// Standardize city names
			first = Graph::standardizeCity(first);
			second = Graph::standardizeCity(second);
			edges.push_back({first, second, distance});
		} else if (!line.empty()) {
			// This is a city
			// Standardize city name
			string city = Graph::standardizeCity(line);
			cities.push_back(city);
		}
	}
	
	if (!file.eof() && file.fail()) {
		string errorMsg = "Error reading file: " + filePath;
		cerr << errorMsg << endl;
		throw runtime_error(errorMsg);
	}
	
	// Second pass: add all cities first
	for (const auto& city : cities) {
		graph.addCity(city);
	}
	
	// Third pass: add all edges
	for (const auto& [from, to, dist] : edges) {
		graph.addEdge(from, to, dist);
	}
	
	cout << "Graph loaded from " << filePath << endl;
	cout << "Loaded " << cities.size() << " cities and " << edges.size() << " edges." << endl;
}



