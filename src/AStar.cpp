#include "../include/AStar.hpp"
#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;
#include <cmath> // for sqrt, pow

unordered_map<string, pair<int, int>> cityCoords = {
    {"Khartoum", {0, 0}},
    {"PortSudan", {5, 4}},
    {"Omdurman", {1, 2}},
    {"Atbara", {3, 1}},
    {"WadMadni", {2, -1}},
    {"Bahri", {1, 1}},
    // TODO add The cities here
};

int AStar::heuristic(const string &a, const string &b)
{

  if (cityCoords.find(a) == cityCoords.end() || cityCoords.find(b) == cityCoords.end())
  {
    return 0; // meaning A* behaves like Dijkstra
  }

  auto [x1, y1] = cityCoords[a];
  auto [x2, y2] = cityCoords[b];

  return static_cast<int>(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2))); // Euclidean distance
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