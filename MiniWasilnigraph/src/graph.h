// Graph.hpp

#define GRAPH_HPP


#include <unordered_map>
#include <list>
#include <string>
#include <queue>
#include <stack>        
#include <vector> 
using namespace std;

class Graph {
private:
    unordered_map<string, list<pair<string, int>>> adjList;  // Adjacency list (city -> list of connected cities with distances)

public:

    void addCity(const string& city);


    void addEdge(const string& from, const string& to, int dist);

   
    void display() const;

    void saveGraph(const string& filename);


    void loadGraph(const string& filename);
};
//  Created by Mishkat Ahmed on 21/04/2025.


