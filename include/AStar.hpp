#ifndef ASTAR_HPP
#define ASTAR_HPP
 
#include "Graph.hpp"
#include <string>
#include <vector>
#include <unordered_map>
 
using namespace std;
 
class AStar
{
public:
  static vector<string> findPath(Graph &graph, const string &startCity, const string &goalCity);
 
private:
  // Heuristic function (can be enhanced)
  static int heuristic(Graph& graph,const string &a, const string &b);
 
  // Builds the path from cameFrom map
  static vector<string> reconstructPath(const unordered_map<string, string> &cameFrom,
                                        const string &startCity,
                                        const string &goalCity);
 
  // Prints the full path and total cost
  static void displayPath(const vector<string> &path, int totalCost);
};
 
#endif