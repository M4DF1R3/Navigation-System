/*
Name: Kevin Qian
SID: 1709188
CCID: cq4
AnonID: 1000317291
CMPUT 275 , Winter 2023
Assignment 1: Part 1
*/
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <limits.h>
#include "wdigraph.h"
#include "dijkstra.h"

using namespace std;

struct Point // A struct to store the coordinates of a vertex
{
    long long lat; // latitude of the point
    long long lon; // longitude of the point
};

typedef pair<int, long long> PIL;

/*
Description : Compute the Manhattan distance between two points
Arguments : pt1, pt2 - the two points to compute the distance between
Return: The Manhattan distance between the two points
*/
long long manhattan(const Point &pt1, const Point &pt2)
{
    // Return the Manhattan distance between the two given points
    return abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
}

/*
Description : Read the graph from the file and store it in the graph object
Arguments : filename - the name of the file to read from
            graph - the graph object to store the graph in
            points - a mapping between vertex identifiers and their coordinates
Return: None
*/
void readGraph(string filename, WDigraph &graph, unordered_map<int, Point> &points)
{
    /*
    Read Edmonton map data from the provided file and
    load it into the WDigraph object passed to this function.
    Store vertex coordinates in Point struct and map each
    vertex identifier to its corresponding Point struct variable.
    PARAMETERS:
    filename: the name of the file that describes a road network
    graph: an instance of the weighted directed graph (WDigraph) class
    points: a mapping between vertex identifiers and their coordinates
    */
    string input;
    ifstream file(filename);
    while (getline(file, input))
    {
        if (input[0] == 'V')
        {
            // Parse the input and add the vertex to the graph
            input.erase(0, 2);
            size_t pos = input.find(",");
            int id = stoi(input.substr(0, pos));
            input.erase(0, pos + 1);
            pos = input.find(",");
            double lat = stod(input.substr(0, pos));
            input.erase(0, pos + 1);
            long long newlat = static_cast<long long>(lat * 100000);
            double lon = stod(input);
            long long newlon = static_cast<long long>(lon * 100000);
            graph.addVertex(id);

            // Store the longitude and latitude to the unordered_map
            Point point = {newlat, newlon};
            points[id] = point;
        }
        else if (input[0] == 'E')
        {
            // Parse the input and add the edge to the graph
            input.erase(0, 2);
            size_t pos = input.find(",");
            int id1 = stoi(input.substr(0, pos));
            input.erase(0, pos + 1);
            pos = input.find(",");
            int id2 = stoi(input.substr(0, pos));
            input.erase(0, pos + 1);
            long long weight = manhattan(points[id1], points[id2]);
            graph.addEdge(id1, id2, weight);
        }
    }
    file.close();
}

/*
Description : Maps the coordinates of the vertex to the closest vertex from the points
Arguments : latStart - the latitude of the start point
            lonStart - the longitude of the start point
            points - the unordered_map of the points
Return: closest vertex an integer of the closest vertex
*/
int closestVertex(long long latStart, long long lonStart, unordered_map<int, Point> &points)
{
    // Find the vertex closest to the given coordinates
    long long min = LLONG_MAX;
    int closest;
    for (auto it = points.begin(); it != points.end(); it++)
    {
        if (manhattan(it->second, {latStart, lonStart}) < min)
        {
            min = manhattan(it->second, {latStart, lonStart});
            closest = it->first;
        }
    }
    return closest;
}

/*
Description : This function returns the path from the start vertex to the end vertex
Arguments : startVertex - the start vertex
            endVertex - the end vertex
            tree - the tree of the shortest path
            points - the unordered_map of the points
            numpts - the number of points in the path
Return: Vector of points that is the path
*/
vector<Point> getPath(int startVertex, int endVertex, const unordered_map<int, PIL> &tree, const unordered_map<int, Point> &points, int &numpts)
{
    int current = endVertex;
    vector<Point> path;

    if (tree.find(endVertex) == tree.end())
    {
        return path;
    }

    while (current != startVertex)
    {
        path.push_back(points.at(current));
        current = tree.at(current).second;
        numpts++;
    }
    path.push_back(points.at(startVertex));
    numpts++;
    return path;
}

/*
Description : The main function that loads the graph from the "edmonton-roads-2.0.1.txt" file
              and then reads the input from the user and outputs the path.
Arguments : None
Return: None
*/
int main()
{
    WDigraph graph;
    unordered_map<int, Point> points;
    unordered_map<int, PIL> tree;

    // Create a weighted directed graph from the input file
    readGraph("edmonton-roads-2.0.1.txt", graph, points);
    char letter;
    cin >> letter;
    if (letter == 'R')
    {
        long long latStart, lonStart, latEnd, lonEnd;
        cin >> latStart >> lonStart >> latEnd >> lonEnd;
        int startVertex, endVertex, numpts = 0;
        startVertex = closestVertex(latStart, lonStart, points);
        endVertex = closestVertex(latEnd, lonEnd, points);
        dijkstra(graph, startVertex, tree);
        vector<Point> path = getPath(startVertex, endVertex, tree, points, numpts);
        cout << "N " << numpts << endl;
        for (int i = numpts - 1; i >= 0; i--)
        {
            char acknowledge;
            cin >> acknowledge;
            cout << "W " << path[i].lat << " " << path[i].lon << endl;
        }
        char acknowledge;
        cin >> acknowledge;
        cout << "E" << endl;
    }
}
