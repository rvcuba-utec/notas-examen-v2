// ============================================================
// CASO 6: LISTA DE ARISTAS + KRUSKAL (procesar aristas por peso)
// Usar cuando: MST, o cualquier algoritmo que ordene aristas por peso.
// Memoria Θ(m). No sirve para recorrer vecinos (habría que ver las m aristas).
// DSU con path compression + union by size: find y union en O(α(n)) ≈ O(1).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, sz;
    DSU(int n) : p(n + 1), sz(n + 1, 1) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b];
        return true;
    }
    bool same(int a, int b) { return find(a) == find(b); }
};

// ------------------------------------------------------------
// PROBLEMA 1: Árbol de expansión mínima (MST) - peso total
// Dado grafo conexo no dirigido con pesos, hallar el peso total del MST.
// Útil: lista de aristas ordenada por peso + DSU = Kruskal clásico.
// Límites: n <= 2e5, m <= 5e5, pesos positivos.
// Input: n m, luego m líneas "u v w"
// Output: peso total del MST
// ------------------------------------------------------------
void problema1() {
    int n, m;
    cin >> n >> m;
    vector<tuple<int,int,int>> E(m);
    for (auto& [w, u, v] : E) cin >> u >> v >> w;
    sort(E.begin(), E.end());
    DSU dsu(n);
    long long total = 0;
    int aristas = 0;
    for (auto [w, u, v] : E) {
        if (dsu.unite(u, v)) {
            total += w;
            if (++aristas == n - 1) break;
        }
    }
    cout << total << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 2: MST - reconstruir las aristas del árbol
// Además del peso, listar las aristas (u, v, w) que pertenecen al MST.
// Límites: n <= 2e5, m <= 5e5.
// Input: n m, luego m líneas "u v w"
// Output: peso total, luego n-1 líneas con aristas del MST
// ------------------------------------------------------------
void problema2() {
    int n, m;
    cin >> n >> m;
    vector<tuple<int,int,int>> E(m);
    for (auto& [w, u, v] : E) cin >> u >> v >> w;
    sort(E.begin(), E.end());
    DSU dsu(n);
    long long total = 0;
    vector<tuple<int,int,int>> mst;
    for (auto [w, u, v] : E) {
        if (dsu.unite(u, v)) {
            total += w;
            mst.push_back({u, v, w});
        }
    }
    cout << total << "\n";
    for (auto [u, v, w] : mst) cout << u << " " << v << " " << w << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 3: MST máximo (Maximum Spanning Tree)
// Hallar el árbol de expansión de peso máximo.
// Truco: ordenar aristas de mayor a menor (o negar pesos y aplicar Kruskal normal).
// Límites: n <= 2e5, m <= 5e5.
// Input: n m, luego m líneas "u v w"
// Output: peso total del MST máximo
// ------------------------------------------------------------
void problema3() {
    int n, m;
    cin >> n >> m;
    vector<tuple<int,int,int>> E(m);
    for (auto& [w, u, v] : E) cin >> u >> v >> w;
    sort(E.rbegin(), E.rend());
    DSU dsu(n);
    long long total = 0;
    for (auto [w, u, v] : E)
        if (dsu.unite(u, v)) total += w;
    cout << total << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 4: Número de componentes conexas (DSU sin Kruskal)
// Dado n y m aristas, hallar cuántas componentes conexas hay.
// Útil: DSU procesa aristas en O(m·α(n)) sin necesitar estructura de adj.
// Límites: n,m <= 2e5.
// Input: n m, luego m líneas "u v"
// Output: número de componentes
// ------------------------------------------------------------
void problema4() {
    int n, m;
    cin >> n >> m;
    DSU dsu(n);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        dsu.unite(u, v);
    }
    set<int> raices;
    for (int i = 1; i <= n; i++) raices.insert(dsu.find(i));
    cout << raices.size() << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 5: ¿Agregar esta arista crea un ciclo? (detección online)
// Se dan m aristas. Por cada una, imprimir "CICLO" si crea un ciclo
// en el grafo acumulado, o "OK" si se agrega sin ciclo.
// Útil: DSU detecta ciclos en O(α(n)) por arista, sin BFS.
// Límites: n,m <= 2e5.
// Input: n m, luego m líneas "u v"
// Output: "OK" o "CICLO" por arista
// ------------------------------------------------------------
void problema5() {
    int n, m;
    cin >> n >> m;
    DSU dsu(n);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        if (!dsu.unite(u, v)) cout << "CICLO\n";
        else cout << "OK\n";
    }
}

// ------------------------------------------------------------
// PROBLEMA 6: Minimax path - minimizar el peso máximo de arista en un camino
// Dado grafo no dirigido con pesos, para cada consulta (u, v) hallar
// el camino u→v tal que la arista de mayor peso sea mínima.
// Insight: la respuesta es la arista más pesada del MST en el camino u-v.
// Estrategia: construir MST con Kruskal; en el MST, la "bottleneck" de
// cualquier par es la arista máxima en el único camino entre ellos.
// Para consultas múltiples se necesita LCA + sparse table en el MST;
// para una sola consulta basta BFS/DFS en el MST.
// Límites: n <= 2e5, m <= 5e5, Q <= 1e5 (aquí versión de una consulta).
// Input: n m u t, luego m líneas "u v w"
// Output: peso de la arista máxima mínima en el camino u-t
// ------------------------------------------------------------
void problema6() {
    int n, m, S, T;
    cin >> n >> m >> S >> T;
    vector<tuple<int,int,int>> E(m);
    for (auto& [w, u, v] : E) cin >> u >> v >> w;
    sort(E.begin(), E.end());
    DSU dsu(n);
    vector<vector<pair<int,int>>> mstAdj(n + 1);
    for (auto [w, u, v] : E) {
        if (dsu.unite(u, v)) {
            mstAdj[u].push_back({v, w});
            mstAdj[v].push_back({u, w});
        }
    }
    vector<int> dist(n + 1, -1);
    vector<int> maxPeso(n + 1, 0);
    queue<int> q;
    dist[S] = 0;
    q.push(S);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto [v, w] : mstAdj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                maxPeso[v] = max(maxPeso[u], w);
                q.push(v);
            }
        }
    }
    cout << maxPeso[T] << "\n";
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
    else problema6();
}
