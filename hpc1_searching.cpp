#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
using namespace std;

vector<vector<int>> graph;

void addEdge(int src, int dest) {
    graph[src].push_back(dest);
    graph[dest].push_back(src);
}

void displayGraph(int numVertices) {
    cout << "\nGraph Representation:\n";
    for (int i = 0; i < numVertices; ++i) {
        cout << "Vertex " << i << ": ";
        for (int neighbor : graph[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

void printVector(const vector<int>& v) {
    for (int val : v) cout << val << " ";
    cout << endl <<endl <<endl;
}

void sequentialBFS(int start, int numVertices) {
    double t1 = omp_get_wtime();
    vector<bool> visited(numVertices, false);
    vector<int> result;
    queue<int> q;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int current = q.front(); q.pop();
        result.push_back(current);
        for (int neighbor : graph[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    double t2 = omp_get_wtime();
    cout << "\nSequential BFS Output:\n";
    printVector(result);
    cout << "Time: " << (t2 - t1) << " seconds\n";
}

void parallelBFS(int start, int numVertices) {
    double t1 = omp_get_wtime();
    vector<bool> visited(numVertices, false);
    queue<int> q;
    visited[start] = true;
    q.push(start);
    vector<int> result;

    while (!q.empty()) {
        int current = q.front(); q.pop();

        #pragma omp critical
        result.push_back(current);

        vector<int> neighbors = graph[current];

        #pragma omp parallel for
        for (int i = 0; i < neighbors.size(); ++i) {
            int neighbor = neighbors[i];
            #pragma omp critical
            {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                    cout << "Thread " << omp_get_thread_num() << " visiting node " << neighbor << endl;
                }
            }
        }
    }

    double t2 = omp_get_wtime();
    cout << "\nParallel BFS Output:\n";
    printVector(result);
    cout << "Time: " << (t2 - t1) << " seconds\n";
}

void sequentialDFS(int start, int numVertices) {
    double t1 = omp_get_wtime();
    vector<bool> visited(numVertices, false);
    stack<int> s;
    vector<int> result;
    visited[start] = true;
    s.push(start);

    while (!s.empty()) {
        int current = s.top(); s.pop();
        result.push_back(current);
        for (int neighbor : graph[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                s.push(neighbor);
            }
        }
    }

    double t2 = omp_get_wtime();
    cout << "\nSequential DFS Output:\n";
    printVector(result);
    cout << "Time: " << (t2 - t1) << " seconds\n";
}

void parallelDFS(int start, int numVertices) {
    double t1 = omp_get_wtime();
    vector<bool> visited(numVertices, false);
    stack<int> s;
    vector<int> result;
    visited[start] = true;
    s.push(start);

    while (!s.empty()) {
        int current = s.top(); s.pop();

        #pragma omp critical
        result.push_back(current);

        vector<int> neighbors = graph[current];

        #pragma omp parallel for
        for (int i = 0; i < neighbors.size(); ++i) {
            int neighbor = neighbors[i];
            #pragma omp critical
            {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    s.push(neighbor);
                    cout << "Thread " << omp_get_thread_num() << " visiting node " << neighbor << endl;
                }
            }
        }
    }

    double t2 = omp_get_wtime();
    cout << "\nParallel DFS Output:\n";
    printVector(result);
    cout << "Time: " << (t2 - t1) << " seconds\n";
}

int main() {
    int numVertices, numEdges;
    cout << "Enter number of vertices: ";
    cin >> numVertices;
    graph.resize(numVertices);

    cout << "Enter number of edges: ";
    cin >> numEdges;
    cout << "Enter edges (source destination):\n";
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
    }

    displayGraph(numVertices);

    int startVertex;
    cout << "Enter start vertex: ";
    cin >> startVertex;

    int numThreads;
    cout << "Enter number of threads to use: ";
    cin >> numThreads;
    omp_set_num_threads(numThreads);
    cout << "Using " << numThreads << " threads for parallel operations.\n";

    // cout << "Default max threads: " << omp_get_max_threads() << endl;
    cout << "\n --Sequential BFS: \n";
    sequentialBFS(startVertex, numVertices);
    cout << "\n --Parallel BFS: \n";
    parallelBFS(startVertex, numVertices);
    cout << "\n --Sequential DFS: \n";
    sequentialDFS(startVertex, numVertices);
    cout << "\n --Parallel DFS: \n";
    parallelDFS(startVertex, numVertices);

    return 0;
}
