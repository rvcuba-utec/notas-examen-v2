// ============================================================
// CASOS 4, 5, 8, 11: HASH / LISTA ORDENADA / CSR / VECINOS EN COMÚN
// Caso 4: hash O(1) adyacencia, n grande con muchas consultas.
// Caso 5: lista ordenada + binary_search, grafo estático determinista.
// Caso 8: CSR, grafo estático de rendimiento crítico (memoria contigua).
// Caso 11: vecinos en común en O(deg u + deg v) con marca.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================
// CASO 11: VECINOS EN COMÚN
// ------------------------------------------------------------
// PROBLEMA 1: Contar vecinos en común entre u y v (eficiente)
// Para Q consultas (u,v), contar cuántos vértices w son vecinos de ambos.
// Útil: marcar adj[u] en O(deg u), luego recorrer adj[v] en O(deg v).
//       Con matriz sería Θ(n) por consulta; aquí O(deg u + deg v).
// Límites: n,m <= 2e5, Q <= 2e5.
// Input: n m Q, m aristas "u v", Q consultas "u v"
// Output: número de vecinos comunes por consulta
// ------------------------------------------------------------
void problema_vecinos_comunes() {
    int n, m, Q;
    cin >> n >> m >> Q;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<char> marca(n + 1, 0);
    while (Q--) {
        int u, v; cin >> u >> v;
        for (int x : adj[u]) marca[x] = 1;
        int cnt = 0;
        for (int y : adj[v]) if (marca[y]) cnt++;
        for (int x : adj[u]) marca[x] = 0;
        cout << cnt << "\n";
    }
}

// ------------------------------------------------------------
// PROBLEMA 2: Contar triángulos con lista de adyacencia (O(m * sqrt(m)))
// Para cada arista (u,v), contar vecinos comunes de u y v = triángulos
// que contienen esa arista. Dividir entre 3 al final (cada trío contado 3 veces).
// Límites: n,m <= 2e5.
// Input: n m, luego m aristas "u v"
// Output: número de triángulos
// ------------------------------------------------------------
void problema_triangulos() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<char> marca(n + 1, 0);
    long long total = 0;
    for (int u = 1; u <= n; u++) {
        for (int x : adj[u]) marca[x] = 1;
        for (int v : adj[u]) {
            for (int w : adj[v]) {
                if (marca[w]) total++;
            }
        }
        for (int x : adj[u]) marca[x] = 0;
    }
    cout << total / 6 << "\n";
}

// ============================================================
// CASO 5: LISTA ORDENADA + BINARY SEARCH
// ------------------------------------------------------------
// PROBLEMA 3: Grafo estático con muchas consultas de adyacencia
// Dado un grafo estático (se lee todo al inicio), responder Q consultas
// "¿son u y v adyacentes?" en O(log deg u).
// Útil: grafo estático (no cambia), muchas consultas, n grande (hash puede colisionar).
// Límites: n,m <= 2e5, Q <= 1e6.
// Input: n m Q, m aristas "u v", Q consultas "u v"
// Output: "SI" o "NO" por consulta
// ------------------------------------------------------------
void problema_lista_ordenada() {
    int n, m, Q;
    cin >> n >> m >> Q;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (auto& l : adj) {
        sort(l.begin(), l.end());
        l.erase(unique(l.begin(), l.end()), l.end());
    }
    while (Q--) {
        int u, v; cin >> u >> v;
        bool ok = binary_search(adj[u].begin(), adj[u].end(), v);
        cout << (ok ? "SI" : "NO") << "\n";
    }
}

// ============================================================
// CASO 4: LISTA + HASH (adyacencia O(1) promedio)
// ------------------------------------------------------------
// PROBLEMA 4: Adyacencia O(1) con n grande
// Mismo problema que el anterior pero con hash para O(1) amortizado.
// Útil cuando Q es enorme y log(deg) es demasiado lento.
// Límites: n,m <= 2e5, Q <= 2e6.
// Input: n m Q, m aristas "u v", Q consultas "u v"
// Output: "SI" o "NO" por consulta
// ------------------------------------------------------------
struct SplitMix {
    static uint64_t mix(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t S = chrono::steady_clock::now().time_since_epoch().count();
        return mix(x + S);
    }
};

void problema_hash() {
    int n, m, Q;
    cin >> n >> m >> Q;
    vector<vector<int>> adj(n + 1);
    unordered_set<long long, SplitMix> E;
    E.reserve(4 * m + 4);

    auto clave = [&](long long u, long long v) { return u * (n + 1) + v; };

    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        if (u != v && !E.count(clave(u, v))) {
            adj[u].push_back(v);
            adj[v].push_back(u);
            E.insert(clave(u, v));
            E.insert(clave(v, u));
        }
    }
    while (Q--) {
        int u, v; cin >> u >> v;
        cout << (E.count(clave(u, v)) ? "SI" : "NO") << "\n";
    }
}

// ============================================================
// CASO 8: CSR (lista compacta)
// ------------------------------------------------------------
// PROBLEMA 5: BFS en grafo enorme con CSR (memoria contigua)
// Grafo estático de n hasta 1e7 nodos. CSR evita overhead de vector<vector>.
// BFS desde s, reportar componente alcanzable.
// Límites: n <= 1e7, m <= 2e7 (CSR necesario; vector<vector> usa demasiada memoria).
// Input: n m s, m aristas "u v"
// Output: tamaño de la componente alcanzable desde s
// ------------------------------------------------------------
void problema_csr() {
    int n, m, s;
    cin >> n >> m >> s;

    vector<pair<int,int>> aristas(m);
    vector<int> grado(n + 2, 0);
    for (auto& [u, v] : aristas) {
        cin >> u >> v;
        grado[u]++; grado[v]++;
    }

    vector<int> inicio(n + 2, 0);
    for (int i = 1; i <= n; i++) inicio[i + 1] = inicio[i] + grado[i];
    vector<int> vecinos(inicio[n + 1]);
    vector<int> pos(inicio.begin(), inicio.end());
    for (auto [u, v] : aristas) {
        vecinos[pos[u]++] = v;
        vecinos[pos[v]++] = u;
    }

    vector<bool> vis(n + 1, false);
    queue<int> q;
    vis[s] = true; q.push(s);
    int cnt = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        cnt++;
        for (int i = inicio[u]; i < inicio[u + 1]; i++) {
            int v = vecinos[i];
            if (!vis[v]) { vis[v] = true; q.push(v); }
        }
    }
    cout << cnt << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t; cin >> t;
    if (t == 1) problema_vecinos_comunes();
    else if (t == 2) problema_triangulos();
    else if (t == 3) problema_lista_ordenada();
    else if (t == 4) problema_hash();
    else problema_csr();
}
