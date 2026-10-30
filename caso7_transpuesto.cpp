// ============================================================
// CASO 7: GRAFO TRANSPUESTO (dirigido)
// Usar cuando: necesitas "¿quién llega a v?" o grado de entrada,
// o para algoritmos que requieren recorrer aristas en sentido inverso.
// adj[u].size() = grado de salida; adjT[v].size() = grado de entrada.
// Construir transpuesto: O(n+m).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ------------------------------------------------------------
// PROBLEMA 1: Grado de entrada de cada vértice
// Dado un grafo dirigido, imprimir el grado de entrada de cada vértice.
// Útil: transpuesto da adjT[v] = lista de quién apunta a v, |adjT[v]| = in-degree.
// Alternativa más eficiente: contar directamente con vector<int> in.
// Límites: n,m <= 2e5.
// Input: n m, luego m líneas "u v" (arista u→v)
// Output: in-degree[1..n]
// ------------------------------------------------------------
void problema1() {
    int n, m;
    cin >> n >> m;
    vector<int> in(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        in[v]++;
    }
    for (int i = 1; i <= n; i++) cout << in[i] << " \n"[i == n];
}

// ------------------------------------------------------------
// PROBLEMA 2: Componentes fuertemente conexas (Kosaraju)
// Un SCC es un conjunto maximal donde todo par (u,v) tiene camino u→v y v→u.
// Algoritmo: 1) DFS en G, anotar orden de finalización. 2) DFS en G^T en ese orden.
// Útil: el transpuesto es la estructura central del algoritmo de Kosaraju.
// Límites: n,m <= 2e5.
// Input: n m, luego m líneas "u v"
// Output: número de SCCs, luego los IDs de cada SCC (uno por línea)
// ------------------------------------------------------------
void dfs1(int u, const vector<vector<int>>& adj, vector<bool>& vis, vector<int>& orden) {
    vis[u] = true;
    for (int v : adj[u]) if (!vis[v]) dfs1(v, adj, vis, orden);
    orden.push_back(u);
}

void dfs2(int u, const vector<vector<int>>& adjT, vector<int>& comp, int c) {
    comp[u] = c;
    for (int v : adjT[u]) if (!comp[v]) dfs2(v, adjT, comp, c);
}

void problema2() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1), adjT(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adjT[v].push_back(u);
    }
    vector<bool> vis(n + 1, false);
    vector<int> orden;
    for (int i = 1; i <= n; i++)
        if (!vis[i]) dfs1(i, adj, vis, orden);

    vector<int> comp(n + 1, 0);
    int numSCC = 0;
    for (int i = n - 1; i >= 0; i--) {
        int u = orden[i];
        if (!comp[u]) dfs2(u, adjT, comp, ++numSCC);
    }
    cout << numSCC << "\n";
    vector<vector<int>> sccs(numSCC + 1);
    for (int i = 1; i <= n; i++) sccs[comp[i]].push_back(i);
    for (int c = 1; c <= numSCC; c++) {
        for (int x : sccs[c]) cout << x << " ";
        cout << "\n";
    }
}

// ------------------------------------------------------------
// PROBLEMA 3: ¿Existe camino de s a t en grafo dirigido? (BFS bidireccional)
// Hacer BFS desde s en G y desde t en G^T. Si se intersectan, hay camino.
// Útil: el transpuesto permite BFS "hacia atrás" desde t.
// Límites: n,m <= 2e5.
// Input: n m s t, luego m líneas "u v"
// Output: "SI" o "NO"
// ------------------------------------------------------------
void problema3() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<int>> adj(n + 1), adjT(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adjT[v].push_back(u);
    }
    auto bfs = [&](int src, const vector<vector<int>>& g) {
        vector<bool> vis(n + 1, false);
        queue<int> q;
        vis[src] = true; q.push(src);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) if (!vis[v]) { vis[v] = true; q.push(v); }
        }
        return vis;
    };
    auto fromS = bfs(s, adj);
    cout << (fromS[t] ? "SI" : "NO") << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 4: Ordenamiento topológico inverso
// Dado un DAG, imprimir el orden topológico de mayor a menor
// (los sumideros primero). Equivalente al orden topológico del transpuesto.
// Útil: construir el transpuesto y aplicar Kahn sobre él.
// Límites: n,m <= 2e5.
// Input: n m, luego m líneas "u v" (u→v)
// Output: orden topológico inverso
// ------------------------------------------------------------
void problema4() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adjT(n + 1);
    vector<int> out(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adjT[v].push_back(u);
        out[u]++;
    }
    queue<int> q;
    for (int i = 1; i <= n; i++) if (out[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        cout << u << " ";
        for (int v : adjT[u])
            if (--out[v] == 0) q.push(v);
    }
    cout << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 5: Vértices desde los que se puede alcanzar t (antecesores)
// Dado un grafo dirigido y un vértice t, hallar todos los u tal que
// existe camino u→t.
// Útil: en el transpuesto, hacer BFS/DFS desde t en G^T da exactamente
// todos los u desde donde se alcanza t en G.
// Límites: n,m <= 2e5.
// Input: n m t, luego m líneas "u v"
// Output: lista de antecesores de t (incluyendo t)
// ------------------------------------------------------------
void problema5() {
    int n, m, t;
    cin >> n >> m >> t;
    vector<vector<int>> adjT(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adjT[v].push_back(u);
    }
    vector<bool> vis(n + 1, false);
    queue<int> q;
    vis[t] = true; q.push(t);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adjT[u]) if (!vis[v]) { vis[v] = true; q.push(v); }
    }
    for (int i = 1; i <= n; i++) if (vis[i]) cout << i << " ";
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
    else problema5();
}
