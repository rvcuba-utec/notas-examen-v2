// ============================================================
// CASO 10: GRAFOS IMPLÍCITOS (grillas y espacios de estados)
// No se almacena el grafo: los vecinos se calculan al vuelo.
// Grilla R×C: cada casilla es un nodo, vecinos = 4 direcciones.
// Estados: cada estado del problema es un nodo, transiciones = aristas.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

const int df[4] = {-1, 1, 0, 0};
const int dc[4] = {0, 0, -1, 1};

// ------------------------------------------------------------
// PROBLEMA 1: Perímetro del terreno + casillas por grado (imagen de ejemplo)
// Grilla n×m de '#' (tierra) y '.' (agua). Calcular el perímetro total
// (lados expuestos al agua o al borde) y contar casillas por grado 0,1,2,3,4.
// Útil: grafo implícito, grado(f,c) = vecinos de tierra de (f,c).
//       P = Σ (4 - grado(f,c)) para toda casilla de tierra.
// Límites: n,m <= 1000.
// Input: n m, luego n líneas de m caracteres '#' o '.'
// Output: P c0 c1 c2 c3 c4
// ------------------------------------------------------------
void problema1() {
    int n, m;
    cin >> n >> m;
    vector<string> g(n);
    for (auto& s : g) cin >> s;

    long long P = 0;
    long long cnt[5] = {};
    for (int f = 0; f < n; f++) {
        for (int c = 0; c < m; c++) {
            if (g[f][c] != '#') continue;
            int grado = 0;
            for (int k = 0; k < 4; k++) {
                int nf = f + df[k], nc = c + dc[k];
                if (nf >= 0 && nf < n && nc >= 0 && nc < m && g[nf][nc] == '#') grado++;
            }
            P += 4 - grado;
            cnt[grado]++;
        }
    }
    cout << P;
    for (int k = 0; k < 5; k++) cout << " " << cnt[k];
    cout << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 2: BFS en laberinto - distancia mínima de S a E
// Grilla con '#' = pared, '.' = libre, 'S' = inicio, 'E' = fin.
// Hallar el menor número de pasos para ir de S a E (4 direcciones).
// Límites: n,m <= 1000.
// Input: n m, luego n líneas del laberinto
// Output: distancia mínima o -1
// ------------------------------------------------------------
void problema2() {
    int n, m;
    cin >> n >> m;
    vector<string> g(n);
    for (auto& s : g) cin >> s;

    int sf = -1, sc = -1, tf = -1, tc = -1;
    for (int f = 0; f < n; f++)
        for (int c = 0; c < m; c++) {
            if (g[f][c] == 'S') { sf = f; sc = c; }
            if (g[f][c] == 'E') { tf = f; tc = c; }
        }

    vector<vector<int>> dist(n, vector<int>(m, -1));
    queue<pair<int,int>> q;
    dist[sf][sc] = 0;
    q.push({sf, sc});
    while (!q.empty()) {
        auto [f, c] = q.front(); q.pop();
        if (f == tf && c == tc) { cout << dist[f][c] << "\n"; return; }
        for (int k = 0; k < 4; k++) {
            int nf = f + df[k], nc = c + dc[k];
            if (nf < 0 || nf >= n || nc < 0 || nc >= m) continue;
            if (g[nf][nc] == '#' || dist[nf][nc] != -1) continue;
            dist[nf][nc] = dist[f][c] + 1;
            q.push({nf, nc});
        }
    }
    cout << -1 << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 3: Número de islas (componentes conexas en grilla)
// Contar cuántas islas (grupos de '#' conectados en 4 direcciones) hay.
// Límites: n,m <= 1000.
// Input: n m, luego n líneas
// Output: número de islas
// ------------------------------------------------------------
void problema3() {
    int n, m;
    cin >> n >> m;
    vector<string> g(n);
    for (auto& s : g) cin >> s;

    vector<vector<bool>> vis(n, vector<bool>(m, false));
    int islas = 0;
    for (int f = 0; f < n; f++) {
        for (int c = 0; c < m; c++) {
            if (g[f][c] != '#' || vis[f][c]) continue;
            islas++;
            queue<pair<int,int>> q;
            vis[f][c] = true;
            q.push({f, c});
            while (!q.empty()) {
                auto [cf, cc] = q.front(); q.pop();
                for (int k = 0; k < 4; k++) {
                    int nf = cf + df[k], nc = cc + dc[k];
                    if (nf < 0 || nf >= n || nc < 0 || nc >= m) continue;
                    if (g[nf][nc] != '#' || vis[nf][nc]) continue;
                    vis[nf][nc] = true;
                    q.push({nf, nc});
                }
            }
        }
    }
    cout << islas << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 4: Isla más grande (máxima componente conexa en grilla)
// Hallar el área (número de casillas '#') de la isla más grande.
// Límites: n,m <= 1000.
// Input: n m, luego n líneas
// Output: área de la isla más grande (0 si no hay tierra)
// ------------------------------------------------------------
void problema4() {
    int n, m;
    cin >> n >> m;
    vector<string> g(n);
    for (auto& s : g) cin >> s;

    vector<vector<bool>> vis(n, vector<bool>(m, false));
    int maxArea = 0;
    for (int f = 0; f < n; f++) {
        for (int c = 0; c < m; c++) {
            if (g[f][c] != '#' || vis[f][c]) continue;
            int area = 0;
            queue<pair<int,int>> q;
            vis[f][c] = true;
            q.push({f, c});
            while (!q.empty()) {
                auto [cf, cc] = q.front(); q.pop();
                area++;
                for (int k = 0; k < 4; k++) {
                    int nf = cf + df[k], nc = cc + dc[k];
                    if (nf < 0 || nf >= n || nc < 0 || nc >= m) continue;
                    if (g[nf][nc] != '#' || vis[nf][nc]) continue;
                    vis[nf][nc] = true;
                    q.push({nf, nc});
                }
            }
            maxArea = max(maxArea, area);
        }
    }
    cout << maxArea << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 5: Problema de las jarras (Water jug / BFS de estados)
// Dos jarras de capacidad A y B. Desde estado (0,0), hallar el mínimo
// número de operaciones para que alguna jarra tenga exactamente T litros.
// Estado = (a, b). 6 transiciones posibles.
// Límites: A,B <= 100, T <= max(A,B).
// Input: A B T
// Output: mínimo de operaciones o -1
// ------------------------------------------------------------
void problema5() {
    int A, B, T;
    cin >> A >> B >> T;
    vector<vector<int>> dist(A + 1, vector<int>(B + 1, -1));
    queue<pair<int,int>> q;
    dist[0][0] = 0;
    q.push({0, 0});
    while (!q.empty()) {
        auto [a, b] = q.front(); q.pop();
        if (a == T || b == T) { cout << dist[a][b] << "\n"; return; }
        int passAB = min(a, B - b), passBA = min(b, A - a);
        pair<int,int> sig[6] = {
            {A, b}, {a, B},
            {0, b}, {a, 0},
            {a - passAB, b + passAB},
            {a + passBA, b - passBA}
        };
        for (auto [x, y] : sig) {
            if (dist[x][y] == -1) {
                dist[x][y] = dist[a][b] + 1;
                q.push({x, y});
            }
        }
    }
    cout << -1 << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 6: Distancia de Manhattan mínima en grilla con obstáculos
// Grilla n×m, '#' = obstáculo, '.' = libre. Dados múltiples pares (s,t),
// responder la distancia BFS entre ellos. Si hay pocos orígenes distintos,
// precomputar BFS desde cada uno.
// Límites: n,m <= 500, Q <= 1000 (BFS por cada origen único).
// Input: n m Q, grilla, Q consultas "sf sc tf tc" (0-indexed)
// Output: distancia por consulta o -1
// ------------------------------------------------------------
void problema6() {
    int n, m, Q;
    cin >> n >> m >> Q;
    vector<string> g(n);
    for (auto& s : g) cin >> s;

    map<pair<int,int>, vector<vector<int>>> cache;
    auto bfsDesde = [&](int sf, int sc) -> vector<vector<int>> {
        vector<vector<int>> dist(n, vector<int>(m, -1));
        if (g[sf][sc] == '#') return dist;
        queue<pair<int,int>> q;
        dist[sf][sc] = 0;
        q.push({sf, sc});
        while (!q.empty()) {
            auto [f, c] = q.front(); q.pop();
            for (int k = 0; k < 4; k++) {
                int nf = f + df[k], nc = c + dc[k];
                if (nf < 0 || nf >= n || nc < 0 || nc >= m) continue;
                if (g[nf][nc] == '#' || dist[nf][nc] != -1) continue;
                dist[nf][nc] = dist[f][c] + 1;
                q.push({nf, nc});
            }
        }
        return dist;
    };

    while (Q--) {
        int sf, sc, tf, tc;
        cin >> sf >> sc >> tf >> tc;
        auto key = make_pair(sf, sc);
        if (!cache.count(key)) cache[key] = bfsDesde(sf, sc);
        cout << cache[key][tf][tc] << "\n";
    }
}

// ------------------------------------------------------------
// PROBLEMA 7: Puzzle 8 (8-puzzle) - mínimo de movimientos
// Estado = permutación de 9 dígitos (0 = hueco). BFS en espacio de estados.
// Útil: grafo implícito donde el estado es la configuración completa.
// Límites: estado de 9 dígitos → 9! = 362880 estados posibles.
// Input: configuración inicial (9 dígitos, 0 = hueco), fila a fila
// Output: mínimo de movimientos para llegar a "1 2 3 4 5 6 7 8 0" o -1
// ------------------------------------------------------------
void problema7() {
    string inicio = "";
    for (int i = 0; i < 9; i++) {
        int x; cin >> x;
        inicio += (char)('0' + x);
    }
    string meta = "123456780";
    if (inicio == meta) { cout << 0 << "\n"; return; }

    unordered_map<string, int> dist;
    queue<string> q;
    dist[inicio] = 0;
    q.push(inicio);

    int adj8[9][4] = {
        {1,3,-1,-1},{0,2,4,-1},{1,5,-1,-1},
        {0,4,6,-1},{1,3,5,7},{2,4,8,-1},
        {3,7,-1,-1},{4,6,8,-1},{5,7,-1,-1}
    };

    while (!q.empty()) {
        string cur = q.front(); q.pop();
        int d = dist[cur];
        int z = cur.find('0');
        for (int k = 0; k < 4; k++) {
            int nz = adj8[z][k];
            if (nz == -1) continue;
            string nxt = cur;
            swap(nxt[z], nxt[nz]);
            if (!dist.count(nxt)) {
                dist[nxt] = d + 1;
                if (nxt == meta) { cout << dist[nxt] << "\n"; return; }
                q.push(nxt);
            }
        }
    }
    cout << -1 << "\n";
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
