#include "../include/AStar.hpp"
#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

// Example heuristic (currently zero, meaning A* behaves like Dijkstra)
int AStar::heuristic(const string &a, const string &b)
{
  return 0; // TODO: improve with domain-specific heuristic if possible
}

vector<string> AStar::findPath(Graph &graph, const string &startCity, const string &goalCity)
{
  const auto &adjList = graph.getAdjList();

  if (adjList.find(startCity) == adjList.end() || adjList.find(goalCity) == adjList.end())
  {
    cout << "Start or goal city not found in the graph.\n";
    return {};
  }

  unordered_map<string, int> gScore;
  unordered_map<string, int> fScore;
  unordered_map<string, string> cameFrom;

  for (const auto &[city, _] : adjList)
  {
    gScore[city] = numeric_limits<int>::max();
    fScore[city] = numeric_limits<int>::max();
  }

  gScore[startCity] = 0;
  fScore[startCity] = heuristic(startCity, goalCity);

  using PQElement = pair<int, string>; // (fScore, city)
  priority_queue<PQElement, vector<PQElement>, greater<>> openSet;
  openSet.push({fScore[startCity], startCity});

  while (!openSet.empty())
  {
    string current = openSet.top().second;
    openSet.pop();

    if (current == goalCity)
    {
      vector<string> path = reconstructPath(cameFrom, startCity, goalCity);

      // Calculate total cost from gScore map
      int totalCost = gScore[goalCity];

      displayPath(path, totalCost);
      return path;
    }

    for (const auto &[neighbor, dist] : adjList.at(current))
    {
      int tentative_gScore = gScore[current] + dist;
      if (tentative_gScore < gScore[neighbor])
      {
        cameFrom[neighbor] = current;
        gScore[neighbor] = tentative_gScore;
        fScore[neighbor] = tentative_gScore + heuristic(neighbor, goalCity);
        openSet.push({fScore[neighbor], neighbor});
      }
    }
  }

  cout << "No path found.\n";
  return {};
}

vector<string> AStar::reconstructPath(const unordered_map<string, string> &cameFrom,
                                      const string &startCity,
                                      const string &goalCity)
{
  vector<string> path;
  string current = goalCity;
  while (current != startCity)
  {
    path.push_back(current);
    current = cameFrom.at(current);
  }
  path.push_back(startCity);
  reverse(path.begin(), path.end());
  return path;
}

void AStar::displayPath(const vector<string> &path, int totalCost)
{
  cout << "\nShortest Path Found (A*):\n";
  cout << "Total Cost: " << totalCost << "\nPath: ";
  for (size_t i = 0; i < path.size(); i++)
  {
    cout << path[i];
    if (i < path.size() - 1)
      cout << " -> ";
  }
  cout << "\n";
}