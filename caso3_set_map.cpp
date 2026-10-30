// ============================================================
// CASO 3: LISTA CON SET / MAP (grafo dinámico)
// Usar cuando: aristas se agregan Y quitan, necesitas vecinos sin
// duplicados, o quieres ady/quitar en O(log deg).
// set: sin pesos. map: con pesos consultables/actualizables.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ------------------------------------------------------------
// PROBLEMA 1: Grafo dinámico - mantener suma de grados y vértices de grado impar
// Se procesan Q operaciones: "A u v" agrega arista, "D u v" la elimina.
// Después de cada operación imprimir: suma_grados (= 2m) y cantidad de vértices
// con grado impar.
// Útil: set ignora duplicados y quitar es O(log n), sin buscar manualmente.
// Límites: n <= 1e5, Q <= 2e5.
// Input: n Q, luego Q líneas "A u v" o "D u v"
// Output: una línea por operación con "suma_grados vértices_grado_impar"
// ------------------------------------------------------------
void problema1() {
    int n, Q;
    cin >> n >> Q;
    vector<set<int>> adj(n + 1);
    long long sumaGrados = 0;
    int gradoImpar = 0;

    auto agregar = [&](int u, int v) {
        if (u == v || adj[u].count(v)) return;
        bool paru = (adj[u].size() % 2 == 0);
        bool parv = (adj[v].size() % 2 == 0);
        adj[u].insert(v);
        adj[v].insert(u);
        sumaGrados += 2;
        if (paru) gradoImpar++;  else gradoImpar--;
        if (parv) gradoImpar++;  else gradoImpar--;
    };
    auto quitar = [&](int u, int v) {
        if (!adj[u].count(v)) return;
        bool paru = (adj[u].size() % 2 == 0);
        bool parv = (adj[v].size() % 2 == 0);
        adj[u].erase(v);
        adj[v].erase(u);
        sumaGrados -= 2;
        if (paru) gradoImpar--;  else gradoImpar++;
        if (parv) gradoImpar--;  else gradoImpar++;
    };

    while (Q--) {
        char op; int u, v;
        cin >> op >> u >> v;
        if (op == 'A') agregar(u, v);
        else quitar(u, v);
        cout << sumaGrados << " " << gradoImpar << "\n";
    }
}

// ------------------------------------------------------------
// PROBLEMA 2: Grafo con pesos actualizables - consulta de peso de arista
// Se procesan operaciones: "U u v w" actualiza (o agrega) peso de arista u-v,
// "D u v" elimina arista, "Q u v" consulta peso (-1 si no existe).
// Útil: map<int,int> permite adj[u][v] = w y consulta/actualización en O(log deg).
// Límites: n <= 1e5, Q <= 2e5.
// Input: n Q, luego Q líneas de operaciones
// Output: respuesta por cada consulta Q
// ------------------------------------------------------------
void problema2() {
    int n, Q;
    cin >> n >> Q;
    vector<map<int,int>> adj(n + 1);

    while (Q--) {
        string op; cin >> op;
        if (op == "U") {
            int u, v, w; cin >> u >> v >> w;
            adj[u][v] = w;
            adj[v][u] = w;
        } else if (op == "D") {
            int u, v; cin >> u >> v;
            adj[u].erase(v);
            adj[v].erase(u);
        } else {
            int u, v; cin >> u >> v;
            auto it = adj[u].find(v);
            cout << (it == adj[u].end() ? -1 : it->second) << "\n";
        }
    }
}

// ------------------------------------------------------------
// PROBLEMA 3: Recorrer vecinos en orden creciente (BFS ordenado)
// BFS donde en cada paso se visitan los vecinos en orden creciente de ID.
// Esto da el "BFS lexicográficamente mínimo".
// Útil: set mantiene vecinos ordenados sin costo extra.
// Límites: n,m <= 1e5.
// Input: n m s, luego m líneas "u v"
// Output: orden de visita BFS lexicográfico
// ------------------------------------------------------------
void problema3() {
    int n, m, s;
    cin >> n >> m >> s;
    vector<set<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].insert(v);
        adj[v].insert(u);
    }
    vector<bool> vis(n + 1, false);
    queue<int> q;
    vis[s] = true;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        cout << u << " ";
        for (int v : adj[u]) {
            if (!vis[v]) {
                vis[v] = true;
                q.push(v);
            }
        }
    }
    cout << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 4: Eliminar aristas duplicadas y lazos al leer el grafo
// El input puede tener aristas repetidas y lazos (u,u). Construir
// el grafo simple equivalente y reportar cuántas aristas tiene.
// Útil: set rechaza duplicados automáticamente en insert(), sin código extra.
// Límites: n <= 1e5, m <= 2e5 (con posibles duplicados).
// Input: n m, luego m líneas "u v"
// Output: número de aristas del grafo simple y lista de adyacencia
// ------------------------------------------------------------
void problema4() {
    int n, m;
    cin >> n >> m;
    vector<set<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        if (u != v) { adj[u].insert(v); adj[v].insert(u); }
    }
    long long aristas = 0;
    for (int u = 1; u <= n; u++) aristas += adj[u].size();
    aristas /= 2;
    cout << aristas << "\n";
    for (int u = 1; u <= n; u++) {
        cout << u << ":";
        for (int v : adj[u]) cout << " " << v;
        cout << "\n";
    }
}

// ------------------------------------------------------------
// PROBLEMA 5: Grafo dinámico - ¿sigue siendo conexo tras eliminar arista?
// Se dan Q eliminaciones de aristas. Después de cada una, reportar
// si el grafo sigue conexo.
// Estrategia: mantener el grafo con sets; tras cada eliminación hacer BFS/DFS.
// Útil: quitar arista en set es O(log deg), limpio y sin índices rotos.
// Límites: n <= 500, Q <= 1000 (BFS tras cada operación: O(Q*(n+m))).
// Input: n m Q, m aristas "u v", Q eliminaciones "u v"
// Output: "SI" o "NO" por eliminación
// ------------------------------------------------------------
void problema5() {
    int n, m, Q;
    cin >> n >> m >> Q;
    vector<set<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].insert(v);
        adj[v].insert(u);
    }

    auto conexo = [&]() -> bool {
        vector<bool> vis(n + 1, false);
        stack<int> st;
        st.push(1); vis[1] = true;
        int cnt = 1;
        while (!st.empty()) {
            int u = st.top(); st.pop();
            for (int v : adj[u]) {
                if (!vis[v]) { vis[v] = true; cnt++; st.push(v); }
            }
        }
        return cnt == n;
    };

    while (Q--) {
        int u, v; cin >> u >> v;
        adj[u].erase(v);
        adj[v].erase(u);
        cout << (conexo() ? "SI" : "NO") << "\n";
    }
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
