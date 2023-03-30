/*
Name: Kevin Qian
SID: 1709188
CCID: cq4
AnonID: 1000317291
CMPUT 275 , Winter 2023
Assignment 1: Part 1
*/
#include <iostream>
#include <unordered_map>
#include <list>
#include <queue>
#include <utility> // for std::pair
#include <vector>
#include "wdigraph.h"
#include "dijkstra.h"

using namespace std;

/*
Description : Computes the least cost paths that start from a start vertex to all other vertices in the graph.
    Use a binary heap to efficiently retrieve an unexplored vertex that has the minimum distance from the start vertex
    at every iteration.
Arguments : graph - the graph object
            startVertex - the vertex to start the search from
            tree - a search tree to construct the least cost path from startVertex to some vertex
Return: None
*/
void dijkstra(const WDigraph &graph, int startVertex, unordered_map<int, PIL> &tree)
{
    unordered_set<int> visited;
    priority_queue<PIL, vector<PIL>, greater<PIL>> pq; // Make the priority queue a min heap
    pq.push({0, startVertex});
    while (!pq.empty())
    {
        PIL current = pq.top();
        pq.pop();
        if (visited.find(current.second) == visited.end())
        {
            visited.insert(current.second);
            for (auto i = graph.neighbours(current.second); i != graph.endIterator(current.second); i++)
            {
                if (visited.find(*i) == visited.end())
                {
                    long long dist = tree[current.second].first + graph.getCost(current.second, *i);
                    if (tree.find(*i) == tree.end() || dist < tree[*i].first)
                    {
                        tree[*i] = {dist, current.second};
                        pq.push({dist, *i});
                    };
                }
            }
        }
    }
}
