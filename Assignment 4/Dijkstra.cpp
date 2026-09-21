#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    int V, E;
    cout << "Enter vertices and edges: ";
    cin >> V >> E;

    vector<vector<pair<int, int>>> graph(V);

    cout << "Enter edges (u v w):\n";
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }

    int source;
    cout << "Enter source: ";
    cin >> source;

    int H;
    cout << "Enter number of hospitals: ";
    cin >> H;

    vector<int> hospitals(H);
    cout << "Enter hospitals: ";
    for (int i = 0; i < H; i++) {
        cin >> hospitals[i];
    }

    vector<int> dist(V, 1e9);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto edge : graph[u]) {
            int v = edge.first;
            int w = edge.second;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    int minTime = 1e9;
    int ansHospital = -1;

    for (int h : hospitals) {
        if (dist[h] < minTime) {
            minTime = dist[h];
            ansHospital = h;
        }
    }

    cout << "\n=========================================\n";
    cout << "               FINAL RESULT              \n";
    cout << "=========================================\n";

    if (ansHospital == -1) {
        cout << "No hospital reachable.\n";
    } else {
        cout << "Ambulance Source Location : Node " << source << "\n";
        cout << "Nearest Hospital Location : Node " << ansHospital << "\n";
        cout << "Minimum Travel Time       : " << minTime << " minutes\n";
    }

    cout << "=========================================\n";

    return 0;
}
