#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

class Graph {
public:
    unordered_map<char, vector<char>> adj;

    void addEdge(char u, char v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected graph
    }

    void parallelBFS(char start) {
        unordered_map<char, bool> visited;
        queue<char> q;

        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS starting from node " << start << ": ";

        while (!q.empty()) {
            int size = q.size();

            #pragma omp parallel for
            for (int i = 0; i < size; i++) {
                char current;

                #pragma omp critical
                {
                    if (!q.empty()) {
                        current = q.front();
                        q.pop();
                        cout << current << " ";
                    }
                }

                #pragma omp critical
                {
                    for (char neighbor : adj[current]) {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }

        cout << endl;
    }

    void printAdjList() {
        cout << "\nAdjacency List Representation:\n";
        for (auto& pair : adj) {
            cout << pair.first << " -> ";
            for (char neighbor : pair.second) {
                cout << neighbor << ", ";
            }
            cout << "\b\b \n";
        }
    }
};

int main() {
    Graph g;
    int numEdges;
    char u, v;

    cout << "Enter number of edges: ";
    cin >> numEdges;

    for (int i = 0; i < numEdges; ++i) {
        cout << "Enter edge (format: A B): ";
        cin >> u >> v;
        g.addEdge(u, v);
    }

    g.printAdjList();

    char start;
    cout << "\nEnter start node for BFS: ";
    cin >> start;

    g.parallelBFS(start);

    return 0;
}

