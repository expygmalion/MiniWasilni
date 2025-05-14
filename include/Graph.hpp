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

using namespace std;

class Graph {
    friend class IOManager;
public:
    void addCity(const string& city);
    void deleteCity(const string& from, const string& to);
    void addEdge(const string& from, const string& to, int distance);
    void display() const;
    void deleteCity(string name);
    void deleteEdge(string from, string to);
    void saveGraph(const string &filename);
    void loadGraph(const string& filename);
private:
//    Private to avoid editing by non allowed
    unordered_map<string, vector<pair<string, int>>> adjList;
};

#endif
