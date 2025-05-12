// BSF.h
//.hpp made -> 12-5-25
#ifndef BSF_H
#define BSF_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <vector>

using namespace std;

class BFS {
private:
    unordered_map<string, list<string>> adjList;

public:
    void loadFromWeightedGraph(const unordered_map<string, list<pair<string, int>>>& weightedAdj);

    void traverse(const string& start);

    vector<string> shortestPath(const string& start, const string& end);


    void printPath(const vector<string>& path);
};

#endif 