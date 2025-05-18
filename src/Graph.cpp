#include "../include/Graph.hpp"
#include "../include/AStar.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

// Add a city (node) to the graph
void Graph::addCity(const string &city)
{
    if (adjList.find(city) == adjList.end())
    {
        adjList[city]; // Creates entry with empty neighbor list
        cout << "City " << city << " added.\n";
    }
    else
    {
        cout << "City " << city << " already exists.\n";
    }
}

// Add an edge (connection) between two cities with a distance
void Graph::addEdge(const string &from, const string &to, int dist)
{
    // Prevent duplication of edge
    for (auto it = adjList[from].begin(); it != adjList[from].end(); ++it)
    {
        if (it->first == to && it->second == dist)
        {
            cout << "Edge already exists.\n";
            return;
        }
    }

    // Ensure both cities exist
    if (adjList.find(from) == adjList.end())
    {
        cout << "City " << from << " not found. Please add the city first.\n";
        return;
    }
    if (adjList.find(to) == adjList.end())
    {
        cout << "City " << to << " not found. Please add the city first.\n";
        return;
    }

    adjList[from].emplace_back(to, dist);
    cout << "Directed edge added from " << from << " to " << to << " with distance " << dist << ".\n";
}

// Display the graph (cities and their connections)
void Graph::display() const
{
    if (adjList.empty())
    {
        cout << "The graph is empty.\n";
        return;
    }

    cout << "\n=== Graph ===\n";
    for (const auto &[city, neighbors] : adjList)
    {
        cout << city << " -> ";
        for (const auto &[neighbor, dist] : neighbors)
        {
            cout << "(" << neighbor << ", " << dist << ") ";
        }
        cout << "\n";
    }
}

// Delete a city and its edges from the graph
void Graph::deleteCity(string name)
{
    // Remove edges to the city
    for (auto it = adjList.begin(); it != adjList.end(); ++it)
    {
        vector<pair<string, int>> newNeighbors;
        for (const auto &neighbor : it->second)
        {
            if (neighbor.first != name)
            {
                newNeighbors.push_back(neighbor);
            }
        }
        it->second = newNeighbors;
    }

    // Remove city itself
    adjList.erase(name);
}

// Delete edge between two cities (undirected)
void Graph::deleteEdge(string from, string to)
{
    // Validate cities exist
    if (adjList.find(from) == adjList.end() || adjList.find(to) == adjList.end())
    {
        cout << "One or both cities not found in the graph.\n";
        return;
    }

    // Remove edge from 'from' to 'to'
    auto &neighborsFrom = adjList[from];
    neighborsFrom.erase(remove_if(neighborsFrom.begin(), neighborsFrom.end(),
                                  [&to](const pair<string, int> &neighbor)
                                  { return neighbor.first == to; }),
                        neighborsFrom.end());

    // Remove edge from 'to' to 'from' for undirected graph
    auto &neighborsTo = adjList[to];
    neighborsTo.erase(remove_if(neighborsTo.begin(), neighborsTo.end(),
                                [&from](const pair<string, int> &neighbor)
                                { return neighbor.first == from; }),
                      neighborsTo.end());

    cout << "Edge between " << from << " and " << to << " removed successfully.\n";
}