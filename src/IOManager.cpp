#include "IOManager.hpp"
#include <iostream>
using namespace std;

 
void IOManager::saveGraph(const string& filename, const Graph& graph)
{
	ofstream file(filename);
	if (!file) {
		cerr << "Error opening file for writing: " << filename << endl;
		return;
	}
	// Save cities
	for (const auto& [city, neighbors] : graph.adjList) {
		file << city << endl;
	}
	// Save edges
	for (const auto& [city, neighbors] : graph.adjList) {
		for (const auto& [neighbor, dist] : neighbors) {
			file << city << " " << neighbor << " " << dist << endl;
		}
	}
	file.close();
	cout << "Graph saved to " << filename << endl;
}

void IOManager::loadGraph(const string& filename, Graph& graph)
{
	ifstream file(filename);
	if (!file) {
		cerr << "Error opening file for reading: " << filename << endl;
		return;
	}
	string line;
	// Load cities
	while (getline(file, line)) {
		if (line.empty()) break; // Stop if an empty line is encountered
		graph.addCity(line);
	}
	// Load edges
	while (getline(file, line)) {
		istringstream iss(line);
		string from, to;
		int distance;
		if (iss >> from >> to >> distance) {
			graph.addEdge(from, to, distance);
		}
	}
	file.close();
	cout << "Graph loaded from " << filename << endl;
}


