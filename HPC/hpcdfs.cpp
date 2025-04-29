#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>
#include <unordered_map>
#include <cctype>

using namespace std;

const int MAX = 100000;
unordered_map<string, int> node_map; // To store string (numeric or alphabet) -> integer mapping
unordered_map<int, string> reverse_node_map; // Reverse mapping (for printing)
vector<int> graph[MAX];
bool visited[MAX];

int getNodeIndex(string node) {
    if (node_map.find(node) != node_map.end()) {
        return node_map[node];
    }
    
    // If not found, assign a new index to this node
    int index = node_map.size();
    node_map[node] = index;
    reverse_node_map[index] = node;
    return index;
}

void dfs(int node) {
    stack<int> s;
    s.push(node);

    while (!s.empty()) {
        int curr_node = s.top();
        s.pop();

        if (!visited[curr_node]) {
            visited[curr_node] = true;
            cout << reverse_node_map[curr_node] << " ";

            #pragma omp parallel for
            for (int i = 0; i < graph[curr_node].size(); i++) {
                int adj_node = graph[curr_node][i];
                if (!visited[adj_node]) {
                    s.push(adj_node);
                }
            }
        }
    }
}

int main() {
    int n, m;
    string start_node;
    cout << "Enter No of Nodes and Edges: ";
    cin >> n >> m;
    cout << "Enter start node (can be a number or letter): ";
    cin >> start_node;

    // Initialize the node_map and reverse_node_map with given nodes
    cout << "Enter Pair of edges (using alphanumeric nodes):\n";
    for (int i = 0; i < m; i++) {
        string u, v;
        cin >> u >> v;

        int u_int = getNodeIndex(u);
        int v_int = getNodeIndex(v);

        graph[u_int].push_back(v_int);
        graph[v_int].push_back(u_int);
    }

    // Initialize visited array
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Start DFS from the start node (convert it to integer)
    int start_int = getNodeIndex(start_node);
    dfs(start_int);

    return 0;
}

