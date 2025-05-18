// File: include/Dijkstra.hpp
#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "Graph.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>
#include "../include/AStar.hpp"


using namespace std;

class Dijkstra {
public:
    static void findPath(Graph& graph);  // Main interface for path finding
    static void dijkstra(Graph& graph, const string& startCity, const string& endCity);
    static void aStar(Graph& graph, const string& startCity, const string& endCity);  // Placeholder for future implementation
private:
    static vector<string> reconstructPath(const unordered_map<string, string>& previous, 
                                        const string& startCity, 
                                        const string& endCity);
    static void displayPath(const vector<string>& path, int totalDistance);
};

#endif