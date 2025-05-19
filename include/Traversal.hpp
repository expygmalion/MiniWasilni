#ifndef TRAVERSAL_HPP
#define TRAVERSAL_HPP

#include "Graph.hpp"
#include <string>
#include <stack>
#include <queue>
#include <unordered_set>
#include <vector>

using namespace std;

class Traversal {
public:
    static void traverse(Graph& graph);
    static void dfs(Graph& graph, const string& startCity);
    static void bfs(Graph& graph, const string& startCity);

    
    // Add path capturing versions
    static void dfsWithPath(const Graph& graph, const string& startCity, vector<string>& path);
    static void bfsWithPath(const Graph& graph, const string& startCity, vector<string>& path);

};

#endif
