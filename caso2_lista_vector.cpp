// ============================================================
// CASO 2: LISTA DE ADYACENCIA CON VECTOR (opción por defecto)
// Usar cuando: n y m grandes, grafo disperso, BFS/DFS/Dijkstra.
// Memoria Θ(n+m). Recorrer vecinos O(deg u). Grado O(1).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ------------------------------------------------------------
// PROBLEMA 1: BFS - Distancia mínima (grafo no ponderado)
// Dado un grafo no dirigido, hallar la distancia mínima en aristas
// desde una fuente s a todos los demás vértices.
// Límites: n,m <= 2e5.
// Input: n m s, luego m líneas "u v"
// Output: dist[1..n] (-1 si no alcanzable)
// ------------------------------------------------------------
void problema1() {
    int n, m, s;
    cin >> n >> m >> s;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> dist(n + 1, -1);
    queue<int> q;
    dist[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    for (int i = 1; i <= n; i++) cout << dist[i] << " \n"[i == n];
}

// ------------------------------------------------------------
// PROBLEMA 2: DFS iterativo - Componentes conexas
// Hallar cuántas componentes conexas tiene el grafo y el tamaño de cada una.
// Límites: n,m <= 2e5.
// Input: n m, luego m líneas "u v"
// Output: número de componentes y tamaño de cada una ordenadas
// ------------------------------------------------------------
void problema2() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<bool> vis(n + 1, false);
    vector<int> tamanios;
    for (int s = 1; s <= n; s++) {
        if (vis[s]) continue;
        int sz = 0;
        stack<int> st;
        st.push(s);
        vis[s] = true;
        while (!st.empty()) {
            int u = st.top(); st.pop();
            sz++;
            for (int v : adj[u]) {
                if (!vis[v]) { vis[v] = true; st.push(v); }
            }
        }
        tamanios.push_back(sz);
    }
    sort(tamanios.rbegin(), tamanios.rend());
    cout << tamanios.size() << "\n";
    for (int x : tamanios) cout << x << " ";
    cout << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 3: Verificar si el grafo es bipartito (2-coloreable)
// BFS con 2 colores. Si un vecino tiene el mismo color → no bipartito.
// Útil para: scheduling, matching, problemas de "dos equipos".
// Límites: n,m <= 2e5.
// Input: n m, luego m líneas "u v"
// Output: "BIPARTITO" o "NO"
// ------------------------------------------------------------
void problema3() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> color(n + 1, -1);
    bool ok = true;
    for (int s = 1; s <= n && ok; s++) {
        if (color[s] != -1) continue;
        queue<int> q;
        color[s] = 0;
        q.push(s);
        while (!q.empty() && ok) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    q.push(v);
                } else if (color[v] == color[u]) {
                    ok = false;
                }
            }
        }
    }
    cout << (ok ? "BIPARTITO" : "NO") << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 4: Dijkstra - Distancia mínima con pesos positivos
// Desde fuente s, hallar dist mínima a todos usando priority_queue.
// Límites: n <= 2e5, m <= 5e5, pesos positivos.
// Input: n m s, luego m líneas "u v w"
// Output: dist[1..n] (-1 si no alcanzable)
// ------------------------------------------------------------
void problema4() {
    int n, m, s;
    cin >> n >> m >> s;
    vector<vector<pair<int,int>>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v, w; cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    const long long INF = 1e18;
    vector<long long> dist(n + 1, INF);
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    dist[s] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    for (int i = 1; i <= n; i++)
        cout << (dist[i] == INF ? -1 : dist[i]) << " \n"[i == n];
}

// ------------------------------------------------------------
// PROBLEMA 5: Detección de ciclo en grafo no dirigido (DFS)
// Un grafo tiene ciclo si DFS encuentra un back-edge (vecino ya visitado
// que no es el padre).
// Límites: n,m <= 2e5.
// Input: n m, luego m líneas "u v"
// Output: "CICLO" o "ARBOL"
// ------------------------------------------------------------
bool dfs_ciclo(int u, int p, const vector<vector<int>>& adj, vector<bool>& vis) {
    vis[u] = true;
    for (int v : adj[u]) {
        if (!vis[v]) {
            if (dfs_ciclo(v, u, adj, vis)) return true;
        } else if (v != p) {
            return true;
        }
    }
    return false;
}

void problema5() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<bool> vis(n + 1, false);
    bool ciclo = false;
    for (int s = 1; s <= n && !ciclo; s++)
        if (!vis[s]) ciclo = dfs_ciclo(s, -1, adj, vis);
    cout << (ciclo ? "CICLO" : "ARBOL") << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 6: Ordenamiento topológico (Kahn / BFS)
// Dado un DAG, encontrar un ordenamiento topológico de los nodos.
// Si hay ciclo, no existe ordenamiento topológico.
// Límites: n,m <= 2e5.
// Input: n m, luego m líneas "u v" (arista dirigida u→v)
// Output: orden topológico o "CICLO"
// ------------------------------------------------------------
void problema6() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    vector<int> in(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        in[v]++;
    }
    queue<int> q;
    for (int i = 1; i <= n; i++) if (in[i] == 0) q.push(i);
    vector<int> orden;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        orden.push_back(u);
        for (int v : adj[u]) {
            if (--in[v] == 0) q.push(v);
        }
    }
    if ((int)orden.size() != n) { cout << "CICLO\n"; return; }
    for (int x : orden) cout << x << " ";
    cout << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 7: Árbol BFS - Reconstruir camino más corto
// Hallar el camino más corto (en número de aristas) entre s y t.
// Límites: n,m <= 2e5.
// Input: n m s t, luego m líneas "u v"
// Output: los nodos del camino o -1 si no existe
// ------------------------------------------------------------
void problema7() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> dist(n + 1, -1), padre(n + 1, -1);
    queue<int> q;
    dist[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                padre[v] = u;
                q.push(v);
            }
        }
    }
    if (dist[t] == -1) { cout << -1 << "\n"; return; }
    vector<int> camino;
    for (int v = t; v != -1; v = padre[v]) camino.push_back(v);
    reverse(camino.begin(), camino.end());
    for (int x : camino) cout << x << " ";
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t; cin >> t;
    if (t == 1) problema1();
    else if (t == 2) problema2();
    else if (t == 3) problema3();
    else if (t == 4) problema4();
    else if (t == 5) problema5();
    else if (t == 6) problema6();
    else problema7();
}
