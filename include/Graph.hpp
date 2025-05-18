#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <unordered_map>

#include <vector>
#include <unordered_map>
#include <list>


#include <string>
#include <queue>
#include <stack>        
#include <algorithm>

using namespace std;

class Graph {
    friend class IOManager;
    friend class Traversal;
    friend class Dijkstra;
    friend class AStar;
public:
    void addCity(const string& city);
    void deleteCity(const string& from, const string& to);
    void addEdge(const string& from, const string& to, int distance);
    void display() const;
    void deleteCity(string name);
    void deleteEdge(string from, string to);
    void saveGraph(const string &filename);
    void loadGraph(const string& filename);
    const unordered_map<string, vector<pair<string, int>>>& getAdjList() const { return adjList; }
    
    // Check if a city exists
    bool cityExists(const string& city) const;
    
    // Convert city name to standard format (uppercase)
    static string standardizeCity(const string& city);
    unordered_map<string, vector<pair<string, int>>> adjList;
};

#endif
