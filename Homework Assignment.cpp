#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <string>

using namespace std;

const int INF = INT_MAX / 2; // A large value representing infinity to prevent overflow

// Structure to represent a path with nodes, cost, and an optional heuristic estimate
struct Path {
    vector<int> nodes;
    int cost;
    int estimate = 0; // Default heuristic estimate for A* Search

    // Constructor to initialize a Path
    Path(const vector<int>& n, int c, int e = 0) : nodes(n), cost(c), estimate(e) {}

    // Comparator to enable priority queue to prioritize paths by cost + estimate
    bool operator>(const Path& other) const {
        return (cost + estimate) > (other.cost + other.estimate);
    }
};

// Function to calculate the total cost of a given path based on the graph's adjacency matrix
int pathCost(const vector<int>& path, const vector<vector<int>>& graph) {
    int totalCost = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        totalCost += graph[path[i]][path[i + 1]];
    }
    totalCost += graph[path.back()][path.front()]; // Adding the cost to return to the start
    return totalCost;
}

// Function to print the path using city names for better readability
void printPath(const vector<int>& path, const vector<string>& cityNames) {
    if (path.empty()) {
        cout << "No path available" << endl;
        return;
    }
    for (size_t i = 0; i < path.size(); ++i) {
        if (i > 0) cout << " -> ";
        cout << cityNames[path[i]];
    }
    cout << endl;
}

// Breadth-First Search (BFS) Implementation
void bfs(const vector<vector<int>>& graph, const vector<string>& cityNames, int start) {
    queue<Path> q; // Queue to manage the paths
    q.push(Path({ start }, 0)); // Initialize with the start city

    int minCost = INF; // Variable to store the minimum cost found
    vector<int> bestPath; // Variable to store the best path found

    while (!q.empty()) {
        Path current = q.front();
        q.pop();

        // If a complete path is found, close the loop and calculate the cost
        if (current.nodes.size() == graph.size()) {
            current.nodes.push_back(start); // Close the loop
            int cost = pathCost(current.nodes, graph);
            if (cost < minCost) {
                minCost = cost;
                bestPath = current.nodes;
            }
        }
        else {
            // Expand the current path to all unvisited cities
            int lastNode = current.nodes.back();
            for (int i = 0; i < graph.size(); i++) {
                if (find(current.nodes.begin(), current.nodes.end(), i) == current.nodes.end()) {
                    vector<int> newNodes = current.nodes;
                    newNodes.push_back(i);
                    int newCost = pathCost(newNodes, graph);
                    q.push(Path(newNodes, newCost)); // Add new path to the queue
                }
            }
        }
    }

    cout << "BFS Best Path: ";
    printPath(bestPath, cityNames);
    cout << "Cost: " << minCost << endl;
}

// Least Cost Search (Uniform Cost Search) Implementation
void leastCostSearch(const vector<vector<int>>& graph, const vector<string>& cityNames, int start) {
    priority_queue<Path, vector<Path>, greater<Path>> pq; // Priority queue to manage paths
    pq.push(Path({ start }, 0)); // Initialize with the start city

    int minCost = INF; // Variable to store the minimum cost found
    vector<int> bestPath; // Variable to store the best path found

    while (!pq.empty()) {
        Path current = pq.top();
        pq.pop();

        // If a complete path is found, close the loop and calculate the cost
        if (current.nodes.size() == graph.size()) {
            current.nodes.push_back(start); // Close the loop
            int cost = pathCost(current.nodes, graph);
            if (cost < minCost) {
                minCost = cost;
                bestPath = current.nodes;
            }
        }
        else {
            // Expand the current path to all unvisited cities
            int lastNode = current.nodes.back();
            for (int i = 0; i < graph.size(); i++) {
                if (find(current.nodes.begin(), current.nodes.end(), i) == current.nodes.end()) {
                    vector<int> newNodes = current.nodes;
                    newNodes.push_back(i);
                    int newCost = pathCost(newNodes, graph);
                    pq.push(Path(newNodes, newCost)); // Add new path to the priority queue
                }
            }
        }
    }

    cout << "Least-Cost Search Best Path: ";
    printPath(bestPath, cityNames);
    cout << "Cost: " << minCost << endl;
}

// A* Search Implementation
void aStarSearch(const vector<vector<int>>& graph, const vector<string>& cityNames, int start) {
    // Heuristic function for A* Search; currently returns 0 (can be improved)
    auto heuristic = [](const vector<int>& path, const vector<vector<int>>& graph) -> int {
        return 0;  // Placeholder for simplicity, should be replaced with a real heuristic
        };

    priority_queue<Path, vector<Path>, greater<Path>> pq; // Priority queue to manage paths
    pq.push(Path({ start }, 0, heuristic({ start }, graph)));  // Initialize with the start city and heuristic

    int minCost = INF; // Variable to store the minimum cost found
    vector<int> bestPath; // Variable to store the best path found

    while (!pq.empty()) {
        Path current = pq.top();
        pq.pop();

        // If a complete path is found, close the loop and calculate the cost
        if (current.nodes.size() == graph.size()) {
            current.nodes.push_back(start);  // Close the loop
            int cost = pathCost(current.nodes, graph);
            if (cost < minCost) {
                minCost = cost;
                bestPath = current.nodes;
            }
        }
        else {
            // Expand the current path to all unvisited cities
            int lastNode = current.nodes.back();
            for (int i = 0; i < graph.size(); i++) {
                if (find(current.nodes.begin(), current.nodes.end(), i) == current.nodes.end()) {
                    vector<int> newNodes = current.nodes;
                    newNodes.push_back(i);
                    int newCost = current.cost + graph[lastNode][i];
                    int estimate = heuristic(newNodes, graph);
                    pq.push(Path(newNodes, newCost, estimate)); // Add new path to the priority queue
                }
            }
        }
    }

    cout << "A* Search Best Path: ";
    printPath(bestPath, cityNames);
    cout << "Cost: " << minCost << endl;
}

int main() {
    vector<string> cityNames = { "Craiova", "Timisoara", "Cluj", "Oradea", "Constanta", "Bucharest" };
    vector<vector<int>> graph = {
        // Craiova, Timisoara, Cluj, Oradea, Constanta, Bucharest
        {0, 349, 442, 500, 631, 230},   // Craiova
        {349, 0, 329, 167, 755, 533},  // Timisoara
        {442, 329, 0, 152, 836, 447},  // Cluj
        {500, 167, 152, 0, 915, 614},  // Oradea
        {631, 755, 836, 915, 0, 225},  // Constanta
        {230, 533, 447, 614, 225, 0}   // Bucharest
    };

    int startCity = 0; // Craiova
    bfs(graph, cityNames, startCity); // Call BFS
    leastCostSearch(graph, cityNames, startCity); // Call Least Cost Search
    aStarSearch(graph, cityNames, startCity); // Call A* Search

    return 0;
}
