// ============================================================
// CASO 9: VÉRTICES CON NOMBRES O IDs ENORMES
// Usar cuando: vértices son strings (ciudades) o enteros hasta 1e9+.
// Se mapean a IDs 0..k-1 y se trabaja con lista de adyacencia normal.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ------------------------------------------------------------
// PROBLEMA 1: Grafo de ciudades - BFS con nombres de ciudades
// Dado un grafo donde los vértices son nombres de ciudades (strings),
// hallar la distancia mínima (en aristas) entre dos ciudades dadas.
// Útil: mapear strings a IDs, luego BFS clásico.
// Límites: n <= 1e5 ciudades, m <= 2e5 rutas.
// Input: n m, n ciudades (un nombre por línea), m rutas "A B",
//        luego s t (nombres de origen y destino)
// Output: distancia mínima o -1
// ------------------------------------------------------------
void problema1() {
    int n, m;
    cin >> n >> m;
    unordered_map<string, int> id;
    for (int i = 0; i < n; i++) {
        string s; cin >> s;
        id[s] = i;
    }
    vector<vector<int>> adj(n);
    for (int i = 0; i < m; i++) {
        string a, b; cin >> a >> b;
        int u = id[a], v = id[b];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    string ss, tt; cin >> ss >> tt;
    int s = id[ss], t = id[tt];

    vector<int> dist(n, -1);
    queue<int> q;
    dist[s] = 0; q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u])
            if (dist[v] == -1) { dist[v] = dist[u] + 1; q.push(v); }
    }
    cout << dist[t] << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 2: Compresión de coordenadas - grafo con IDs hasta 1e9
// Los vértices son enteros hasta 1e9 pero hay a lo sumo n <= 2e5 distintos.
// Comprimir a 1..k y aplicar BFS.
// Límites: n <= 2e5 vértices distintos, m <= 5e5 aristas.
// Input: n m s t, luego m líneas "u v" (u,v pueden ser hasta 1e9)
// Output: distancia mínima de s a t o -1
// ------------------------------------------------------------
void problema2() {
    int n, m;
    long long s, t;
    cin >> n >> m >> s >> t;

    vector<pair<long long,long long>> aristas(m);
    vector<long long> vals;
    vals.push_back(s); vals.push_back(t);
    for (auto& [u, v] : aristas) {
        cin >> u >> v;
        vals.push_back(u); vals.push_back(v);
    }
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    auto compress = [&](long long x) {
        return (int)(lower_bound(vals.begin(), vals.end(), x) - vals.begin()) + 1;
    };

    int N = (int)vals.size();
    vector<vector<int>> adj(N + 1);
    for (auto [u, v] : aristas) {
        int cu = compress(u), cv = compress(v);
        adj[cu].push_back(cv);
        adj[cv].push_back(cu);
    }
    int cs = compress(s), ct = compress(t);
    vector<int> dist(N + 1, -1);
    queue<int> q;
    dist[cs] = 0; q.push(cs);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u])
            if (dist[v] == -1) { dist[v] = dist[u] + 1; q.push(v); }
    }
    cout << dist[ct] << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 3: Componentes conexas con IDs enormes
// Los nodos son enteros hasta 1e18. Hallar número de componentes conexas.
// Útil: comprimir a 0..k-1 con unordered_map, luego DSU o BFS.
// Límites: m <= 2e5 aristas.
// Input: m, luego m líneas "u v"
// Output: número de componentes (considerando todos los nodos mencionados)
// ------------------------------------------------------------
void problema3() {
    int m; cin >> m;
    unordered_map<long long, int> id;
    int cnt = 0;
    auto getId = [&](long long x) {
        auto it = id.find(x);
        if (it != id.end()) return it->second;
        id[x] = cnt++;
        return cnt - 1;
    };

    vector<pair<int,int>> aristas(m);
    for (auto& [u, v] : aristas) {
        long long a, b; cin >> a >> b;
        u = getId(a); v = getId(b);
    }

    int N = cnt;
    vector<int> p(N); iota(p.begin(), p.end(), 0);
    vector<int> sz(N, 1);
    function<int(int)> find = [&](int x) {
        return p[x] == x ? x : p[x] = find(p[x]);
    };
    int componentes = N;
    for (auto [u, v] : aristas) {
        int pu = find(u), pv = find(v);
        if (pu != pv) {
            if (sz[pu] < sz[pv]) swap(pu, pv);
            p[pv] = pu; sz[pu] += sz[pv];
            componentes--;
        }
    }
    cout << componentes << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 4: Red social - shortest path entre usuarios con nombres
// Usuarios identificados por strings. Q consultas "A B": distancia
// mínima entre A y B en la red de amistades.
// Útil: map de nombre a ID + BFS por consulta (si n pequeño) o
//       BFS desde cada nodo (si n grande, precomputar).
// Límites: n <= 1000 usuarios, m <= 5000 amistades, Q <= 1000.
// Input: n m Q, n nombres (uno por línea), m amistades "A B",
//        Q consultas "A B"
// Output: distancia por consulta o -1
// ------------------------------------------------------------
void problema4() {
    int n, m, Q;
    cin >> n >> m >> Q;
    map<string, int> id;
    for (int i = 0; i < n; i++) {
        string s; cin >> s;
        id[s] = i;
    }
    vector<vector<int>> adj(n);
    for (int i = 0; i < m; i++) {
        string a, b; cin >> a >> b;
        int u = id[a], v = id[b];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    auto bfs = [&](int src) {
        vector<int> dist(n, -1);
        queue<int> q;
        dist[src] = 0; q.push(src);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u])
                if (dist[v] == -1) { dist[v] = dist[u] + 1; q.push(v); }
        }
        return dist;
    };
    vector<vector<int>> D(n);
    for (int i = 0; i < n; i++) D[i] = bfs(i);

    while (Q--) {
        string a, b; cin >> a >> b;
        cout << D[id[a]][id[b]] << "\n";
    }
}

// ------------------------------------------------------------
// PROBLEMA 5: Dijkstra con IDs de vértice como strings
// Grafo de aeropuertos (IATA codes como strings) con tiempos de vuelo.
// Hallar tiempo mínimo de viaje entre dos aeropuertos.
// Límites: n <= 500 aeropuertos, m <= 5000 vuelos.
// Input: n m, n códigos IATA, m líneas "A B w", luego s t
// Output: tiempo mínimo o -1
// ------------------------------------------------------------
void problema5() {
    int n, m;
    cin >> n >> m;
    map<string, int> id;
    for (int i = 0; i < n; i++) {
        string s; cin >> s;
        id[s] = i;
    }
    vector<vector<pair<int,int>>> adj(n);
    for (int i = 0; i < m; i++) {
        string a, b; int w;
        cin >> a >> b >> w;
        int u = id[a], v = id[b];
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    string ss, tt; cin >> ss >> tt;
    int s = id[ss], t = id[tt];

    const long long INF = 1e18;
    vector<long long> dist(n, INF);
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    dist[s] = 0; pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u])
            if (dist[u] + w < dist[v]) { dist[v] = dist[u] + w; pq.push({dist[v], v}); }
    }
    cout << (dist[t] == INF ? -1 : dist[t]) << "\n";
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
