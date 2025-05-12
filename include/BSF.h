#ifndef BFS_HPP
#define BFS_HPP

#include <string>
#include <unordered_map>
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;

class BFS {
private:
    unordered_map<string, vector<string>> adjList;  

public:

    void loadFromWeightedGraph(const unordered_map<string, vector<pair<string, int>>>& weightedAdj);

    void traverse(const string& start);

    vector<string> shortestPath(const string& start, const string& end);

    void printPath(const vector<string>& path);

};

#endif 
