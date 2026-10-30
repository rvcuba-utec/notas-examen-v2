// ============================================================
// BFS - BREADTH-FIRST SEARCH
// Propiedad clave: visita nodos por capas → dist[v] es la
// distancia mínima en número de aristas (o pasos) desde la fuente.
// Complejidad: O(V + E) en grafo, O(R*C) en grilla.
// Usar BFS cuando: pesos iguales (o 0/1 con deque), camino mínimo,
// niveles, alcanzabilidad, bipartito, flood fill, estados.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ------------------------------------------------------------
// PROBLEMA 1: BFS clásico - distancia mínima desde una fuente
// Dado grafo no dirigido, distancia mínima (aristas) de s a todos.
// Límites: n,m <= 2e5.
// Input: n m s
// Output: dist[1..n], -1 si no alcanzable
// ------------------------------------------------------------
void bfs_clasico() {
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
// PROBLEMA 2: BFS por niveles - procesar capa por capa
// Útil cuando necesitas saber exactamente qué nodos están a
// distancia k, o propagar "por rondas".
// Ejemplo: hallar el diámetro de un árbol (dos BFS).
// Límites: n,m <= 2e5.
// Input: n m (árbol: n-1 aristas)
// Output: diámetro del árbol
// ------------------------------------------------------------
int bfs_lejos(int src, int n, const vector<vector<int>>& adj, int& nodoLejos) {
    vector<int> dist(n + 1, -1);
    queue<int> q;
    dist[src] = 0;
    q.push(src);
    int maxDist = 0;
    nodoLejos = src;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                if (dist[v] > maxDist) { maxDist = dist[v]; nodoLejos = v; }
                q.push(v);
            }
        }
    }
    return maxDist;
}

void bfs_diametro() {
    int n; cin >> n;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    int a, b;
    bfs_lejos(1, n, adj, a);
    cout << bfs_lejos(a, n, adj, b) << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 3: BFS multiinicio (multi-source BFS)
// Varios nodos fuente al mismo tiempo. dist[v] = distancia al
// source más cercano. Útil: "distancia a la tienda más cercana",
// "propagación de fuego/virus desde múltiples orígenes".
// Límites: n,m <= 2e5, k fuentes.
// Input: n m k, k fuentes, m aristas
// Output: dist[1..n] al source más cercano
// ------------------------------------------------------------
void bfs_multiinicio() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> dist(n + 1, -1);
    queue<int> q;
    for (int i = 0; i < k; i++) {
        int s; cin >> s;
        dist[s] = 0;
        q.push(s);
    }
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
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
// PROBLEMA 4: BFS bidireccional - camino más corto entre s y t
// Lanza BFS desde s y desde t simultáneamente (un nivel cada uno).
// Se detiene cuando los frentes se tocan. Útil en grafos muy grandes
// donde BFS normal sería demasiado lento.
// Límites: n,m <= 2e5.
// Input: n m s t
// Output: distancia mínima o -1
// ------------------------------------------------------------
void bfs_bidireccional() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    if (s == t) { cout << 0 << "\n"; return; }

    vector<int> distS(n + 1, -1), distT(n + 1, -1);
    queue<int> qS, qT;
    distS[s] = 0; qS.push(s);
    distT[t] = 0; qT.push(t);

    int ans = -1;
    auto step = [&](queue<int>& q, vector<int>& d, vector<int>& otro) {
        int sz = q.size();
        while (sz--) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (d[v] == -1) {
                    d[v] = d[u] + 1;
                    q.push(v);
                }
                if (otro[v] != -1) {
                    int cand = d[v] + otro[v];
                    if (ans == -1 || cand < ans) ans = cand;
                }
            }
        }
    };

    while (!qS.empty() || !qT.empty()) {
        step(qS, distS, distT);
        step(qT, distT, distS);
        if (ans != -1) break;
    }
    cout << ans << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 5: 0-1 BFS (aristas de peso 0 o 1 solamente)
// Usar deque: peso 0 → push_front, peso 1 → push_back.
// Equivale a Dijkstra pero en O(V+E) en vez de O((V+E)log V).
// Ejemplo: laberinto donde algunas puertas cuestan 0 y otras 1.
// Límites: n,m <= 2e5.
// Input: n m s t, m líneas "u v w" con w ∈ {0,1}
// Output: costo mínimo de s a t
// ------------------------------------------------------------
void bfs_01() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<pair<int,int>>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v, w; cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    const int INF = 1e9;
    vector<int> dist(n + 1, INF);
    deque<int> dq;
    dist[s] = 0;
    dq.push_back(s);
    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }
    cout << (dist[t] == INF ? -1 : dist[t]) << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 6: BFS en grilla - distancia mínima con obstáculos
// Grilla R×C, '#' = pared, '.' = libre. BFS desde 'S' hasta 'E'.
// Límites: R,C <= 1000.
// Input: R C, luego R filas con C chars
// Output: distancia mínima o -1
// ------------------------------------------------------------
void bfs_grilla() {
    int R, C;
    cin >> R >> C;
    vector<string> g(R);
    for (auto& s : g) cin >> s;

    int sf = -1, sc = -1, tf = -1, tc = -1;
    for (int f = 0; f < R; f++)
        for (int c = 0; c < C; c++) {
            if (g[f][c] == 'S') { sf = f; sc = c; }
            if (g[f][c] == 'E') { tf = f; tc = c; }
        }

    const int df[4] = {-1,1,0,0}, dc[4] = {0,0,-1,1};
    vector<vector<int>> dist(R, vector<int>(C, -1));
    queue<pair<int,int>> q;
    dist[sf][sc] = 0;
    q.push({sf, sc});
    while (!q.empty()) {
        auto [f, c] = q.front(); q.pop();
        for (int k = 0; k < 4; k++) {
            int nf = f + df[k], nc = c + dc[k];
            if (nf < 0 || nf >= R || nc < 0 || nc >= C) continue;
            if (g[nf][nc] == '#' || dist[nf][nc] != -1) continue;
            dist[nf][nc] = dist[f][c] + 1;
            q.push({nf, nc});
        }
    }
    cout << dist[tf][tc] << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 7: BFS multiinicio en grilla - distancia a celda libre más cercana
// Dada una grilla con celdas '1' (ocupadas) y '0' (libres), para cada
// celda '1' hallar la distancia (Manhattan en 4-dirs) a la celda '0' más cercana.
// Estrategia: multi-source BFS iniciando desde TODAS las celdas '0'.
// Límites: R,C <= 1000.
// Input: R C, luego la grilla
// Output: matriz de distancias
// ------------------------------------------------------------
void bfs_distancia_a_libre() {
    int R, C;
    cin >> R >> C;
    vector<string> g(R);
    for (auto& s : g) cin >> s;

    const int df[4] = {-1,1,0,0}, dc[4] = {0,0,-1,1};
    vector<vector<int>> dist(R, vector<int>(C, -1));
    queue<pair<int,int>> q;
    for (int f = 0; f < R; f++)
        for (int c = 0; c < C; c++)
            if (g[f][c] == '0') { dist[f][c] = 0; q.push({f, c}); }

    while (!q.empty()) {
        auto [f, c] = q.front(); q.pop();
        for (int k = 0; k < 4; k++) {
            int nf = f + df[k], nc = c + dc[k];
            if (nf < 0 || nf >= R || nc < 0 || nc >= C) continue;
            if (dist[nf][nc] != -1) continue;
            dist[nf][nc] = dist[f][c] + 1;
            q.push({nf, nc});
        }
    }
    for (int f = 0; f < R; f++) {
        for (int c = 0; c < C; c++) cout << dist[f][c] << " \n"[c == C - 1];
    }
}

// ------------------------------------------------------------
// PROBLEMA 8: BFS para detectar ciclo más corto (girth) en grafo no dirigido
// El girth es la longitud del ciclo más corto. Para cada nodo u,
// hacer BFS: si se encuentra un vecino ya visitado (que no sea el padre)
// a distancia d, el ciclo tiene longitud dist[u] + dist[v] + 1.
// Límites: n,m <= 2e3 (BFS por nodo: O(n*(n+m))).
// Input: n m
// Output: longitud del ciclo más corto o -1 si no hay ciclos
// ------------------------------------------------------------
void bfs_girth() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    int girth = INT_MAX;
    for (int s = 1; s <= n; s++) {
        vector<int> dist(n + 1, -1), padre(n + 1, -1);
        queue<int> q;
        dist[s] = 0; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    padre[v] = u;
                    q.push(v);
                } else if (padre[u] != v) {
                    girth = min(girth, dist[u] + dist[v] + 1);
                }
            }
        }
    }
    cout << (girth == INT_MAX ? -1 : girth) << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 9: BFS para verificar bipartito y hallar los dos grupos
// Un grafo es bipartito ↔ no tiene ciclos impares ↔ es 2-coloreable.
// BFS asigna colores alternados; si un vecino tiene el mismo color → no bipartito.
// Límites: n,m <= 2e5.
// Input: n m
// Output: "BIPARTITO\n" y los dos grupos, o "NO\n"
// ------------------------------------------------------------
void bfs_bipartito() {
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
        color[s] = 0; q.push(s);
        while (!q.empty() && ok) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (color[v] == -1) { color[v] = 1 - color[u]; q.push(v); }
                else if (color[v] == color[u]) ok = false;
            }
        }
    }
    if (!ok) { cout << "NO\n"; return; }
    cout << "BIPARTITO\n";
    for (int i = 1; i <= n; i++) if (color[i] == 0) cout << i << " ";
    cout << "\n";
    for (int i = 1; i <= n; i++) if (color[i] == 1) cout << i << " ";
    cout << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 10: BFS con reconstrucción de camino
// Hallar el camino más corto (lista de nodos) entre s y t.
// Límites: n,m <= 2e5.
// Input: n m s t
// Output: camino s → ... → t o "NO"
// ------------------------------------------------------------
void bfs_camino() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> padre(n + 1, -1);
    vector<bool> vis(n + 1, false);
    queue<int> q;
    vis[s] = true; q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == t) break;
        for (int v : adj[u]) {
            if (!vis[v]) { vis[v] = true; padre[v] = u; q.push(v); }
        }
    }
    if (!vis[t]) { cout << "NO\n"; return; }
    vector<int> camino;
    for (int v = t; v != -1; v = padre[v]) camino.push_back(v);
    reverse(camino.begin(), camino.end());
    for (int x : camino) cout << x << " ";
    cout << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 11: BFS de estados con par (x, y) - Robot en grilla con llave
// Grilla con '#'=pared, '.'=libre, 'K'=llave, 'S'=inicio, 'E'=fin.
// Solo se puede pasar por 'E' si se tiene la llave.
// Estado = (fila, col, tienes_llave). BFS sobre este estado compuesto.
// Límites: R,C <= 500.
// Input: R C, luego la grilla
// Output: pasos mínimos o -1
// ------------------------------------------------------------
void bfs_con_llave() {
    int R, C;
    cin >> R >> C;
    vector<string> g(R);
    for (auto& s : g) cin >> s;

    int sf = -1, sc = -1;
    for (int f = 0; f < R; f++)
        for (int c = 0; c < C; c++)
            if (g[f][c] == 'S') { sf = f; sc = c; }

    const int df[4] = {-1,1,0,0}, dc[4] = {0,0,-1,1};
    vector<vector<array<int,2>>> dist(R, vector<array<int,2>>(C, {-1,-1}));
    queue<tuple<int,int,int>> q;
    dist[sf][sc][0] = 0;
    q.push({sf, sc, 0});

    while (!q.empty()) {
        auto [f, c, k] = q.front(); q.pop();
        if (g[f][c] == 'E' && k == 1) { cout << dist[f][c][k] << "\n"; return; }
        for (int d = 0; d < 4; d++) {
            int nf = f + df[d], nc = c + dc[d];
            if (nf < 0 || nf >= R || nc < 0 || nc >= C) continue;
            if (g[nf][nc] == '#') continue;
            int nk = k || (g[nf][nc] == 'K') ? 1 : 0;
            if (g[nf][nc] == 'E' && nk == 0) continue;
            if (dist[nf][nc][nk] == -1) {
                dist[nf][nc][nk] = dist[f][c][k] + 1;
                q.push({nf, nc, nk});
            }
        }
    }
    cout << -1 << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 12: BFS para contar caminos mínimos
// Hallar cuántos caminos distintos de longitud mínima existen de s a t.
// Límites: n,m <= 2e5.
// Input: n m s t
// Output: número de caminos mínimos (puede ser grande, dar mod 1e9+7)
// ------------------------------------------------------------
void bfs_contar_caminos() {
    const int MOD = 1e9 + 7;
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> dist(n + 1, -1);
    vector<long long> cnt(n + 1, 0);
    queue<int> q;
    dist[s] = 0; cnt[s] = 1; q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
            if (dist[v] == dist[u] + 1) {
                cnt[v] = (cnt[v] + cnt[u]) % MOD;
            }
        }
    }
    cout << cnt[t] << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 13: Flood fill - pintar componente conexa en grilla
// Cambiar todas las celdas de un color conectadas a (sf,sc) al nuevo color.
// BFS equivalente al "balde" de paint.
// Límites: R,C <= 1000.
// Input: R C sf sc colorViejo colorNuevo, luego la grilla (ints)
// Output: grilla modificada
// ------------------------------------------------------------
void bfs_flood_fill() {
    int R, C, sf, sc, cv, cn;
    cin >> R >> C >> sf >> sc >> cv >> cn;
    vector<vector<int>> g(R, vector<int>(C));
    for (auto& row : g) for (auto& x : row) cin >> x;

    if (cv == cn) { for (auto& row : g) { for (int x : row) cout << x << " "; cout << "\n"; } return; }

    const int df[4] = {-1,1,0,0}, dc[4] = {0,0,-1,1};
    queue<pair<int,int>> q;
    g[sf][sc] = cn; q.push({sf, sc});
    while (!q.empty()) {
        auto [f, c] = q.front(); q.pop();
        for (int k = 0; k < 4; k++) {
            int nf = f + df[k], nc = c + dc[k];
            if (nf < 0 || nf >= R || nc < 0 || nc >= C) continue;
            if (g[nf][nc] != cv) continue;
            g[nf][nc] = cn; q.push({nf, nc});
        }
    }
    for (auto& row : g) { for (int x : row) cout << x << " "; cout << "\n"; }
}

// ------------------------------------------------------------
// PROBLEMA 14: BFS en grafo dirigido - nodos alcanzables desde s
// En grafo dirigido, BFS solo sigue aristas en su dirección.
// Hallar todos los nodos alcanzables desde s.
// Límites: n,m <= 2e5.
// Input: n m s
// Output: nodos alcanzables desde s (incluyendo s), en cualquier orden
// ------------------------------------------------------------
void bfs_dirigido() {
    int n, m, s;
    cin >> n >> m >> s;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
    }
    vector<bool> vis(n + 1, false);
    queue<int> q;
    vis[s] = true; q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        cout << u << " ";
        for (int v : adj[u])
            if (!vis[v]) { vis[v] = true; q.push(v); }
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t; cin >> t;
    if      (t == 1)  bfs_clasico();
    else if (t == 2)  bfs_diametro();
    else if (t == 3)  bfs_multiinicio();
    else if (t == 4)  bfs_bidireccional();
    else if (t == 5)  bfs_01();
    else if (t == 6)  bfs_grilla();
    else if (t == 7)  bfs_distancia_a_libre();
    else if (t == 8)  bfs_girth();
    else if (t == 9)  bfs_bipartito();
    else if (t == 10) bfs_camino();
    else if (t == 11) bfs_con_llave();
    else if (t == 12) bfs_contar_caminos();
    else if (t == 13) bfs_flood_fill();
    else              bfs_dirigido();
}
