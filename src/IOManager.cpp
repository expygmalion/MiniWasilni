#include "../include/IOManager.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

IOManager::IOManager()
{
	try
	{
		if (!fs::exists("data"))
		{
			fs::create_directory("data");
			cout << "Created 'data' directory.\n";
		}
	}
	catch (const fs::filesystem_error &e)
	{
		cerr << "Filesystem error: " << e.what() << endl;
	}
}
void IOManager::saveGraph(const string &filename, const Graph &graph)
{
	ofstream file(filename);
	if (!file)
	{
		cerr << "Error opening file for writing: " << filename << endl;
		return;
	}

	// Write cities
	for (const auto &[city, neighbors] : graph.adjList)
	{
		file << city << endl;
	}
	file << "\n"; // Separator between cities and edges

	// Write edges
	for (const auto &[city, neighbors] : graph.adjList)
	{
		for (const auto &[neighbor, dist] : neighbors)
		{
			file << city << " " << neighbor << " " << dist << endl;
		}
	}

	file.close();
	cout << "Graph saved to " << filename << endl;
}

void IOManager::loadGraph(const string &filename, Graph &graph)
{
	ifstream file(filename);
	if (!file)
	{
		cerr << "Error opening file for reading: " << filename << endl;
		return;
	}

	string line;

	// Load cities
	while (getline(file, line))
	{
		if (line.empty())
			break; // Stop at separator
		graph.addCity(line);
	}

	// Load edges
	while (getline(file, line))
	{
		istringstream iss(line);
		string from, to;
		int dist;
		if (iss >> from >> to >> dist)
		{
			graph.addEdge(from, to, dist);
		}
	}

	file.close();
	cout << "Graph loaded from " << filename << endl;
}