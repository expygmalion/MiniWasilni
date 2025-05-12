#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Graph {
public:
    void addCity(const string& city);
    void addEdge(const string& from, const string& to, int distance);
    void display() const;
    void deleteCity(string name);


    //added -> 12-5-25
    const unordered_map<string, vector<pair<string, int>>>& getAdjList() const;

    
private:
//    Private to avoid editing by non allowed
    unordered_map<string, vector<pair<string, int>>> adjList;
};

#endif
