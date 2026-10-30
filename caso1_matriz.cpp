// ============================================================
// CASO 1: MATRIZ DE ADYACENCIA
// Usar cuando: n <= 3000, grafo denso, muchas consultas ady(u,v) O(1),
// o algoritmos que requieren acceso directo (Floyd-Warshall).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ------------------------------------------------------------
// PROBLEMA 1: Floyd-Warshall - Todos los caminos más cortos
// Dado un grafo dirigido con pesos (pueden ser negativos), hallar
// la distancia mínima entre todo par de vértices.
// Útil: matriz porque Floyd accede W[i][j] y W[i][k] + W[k][j] en O(1).
// Límites: n <= 300 (n^3 = 2.7e7 ok), aristas con pesos posiblemente negativos.
// Input: n m, luego m líneas "u v w"
// Output: matriz n×n de distancias (INF si no hay camino)
// ------------------------------------------------------------
const long long INF = 1e18;

void floyd(int n, vector<vector<long long>>& W) {
    for (int k = 1; k <= n; k++)
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                if (W[i][k] < INF && W[k][j] < INF)
                    W[i][j] = min(W[i][j], W[i][k] + W[k][j]);
}

void problema1() {
    int n, m;
    cin >> n >> m;
    vector<vector<long long>> W(n + 1, vector<long long>(n + 1, INF));
    for (int i = 1; i <= n; i++) W[i][i] = 0;
    for (int i = 0; i < m; i++) {
        int u, v; long long w;
        cin >> u >> v >> w;
        W[u][v] = min(W[u][v], w);
    }
    floyd(n, W);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (W[i][j] == INF) cout << "INF";
            else cout << W[i][j];
            cout << " \n"[j == n];
        }
    }
}

// ------------------------------------------------------------
// PROBLEMA 2: Contar triángulos en grafo no dirigido
// Dado un grafo simple no dirigido, contar cuántos triángulos (u,v,w)
// existen (cada trío se cuenta una sola vez).
// Útil: matriz porque ady(u,v) en O(1) → triple for en O(n^3).
// Límites: n <= 500 (n^3 = 1.25e8, justo), m arbitrario.
// Input: n m, luego m líneas "u v"
// Output: número de triángulos
// ------------------------------------------------------------
void problema2() {
    int n, m;
    cin >> n >> m;
    vector<vector<char>> A(n + 1, vector<char>(n + 1, 0));
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        A[u][v] = A[v][u] = 1;
    }
    long long cnt = 0;
    for (int u = 1; u <= n; u++)
        for (int v = u + 1; v <= n; v++)
            if (A[u][v])
                for (int w = v + 1; w <= n; w++)
                    if (A[v][w] && A[u][w]) cnt++;
    cout << cnt << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 3: ¿Es el grafo completo? (todas las aristas presentes)
// Dado n y m aristas, verificar si el grafo es completo (K_n).
// Útil: matriz para verificar que todas las casillas off-diagonal son 1.
// Límites: n <= 1000.
// Input: n m, luego m líneas "u v"
// Output: "SI" o "NO"
// ------------------------------------------------------------
void problema3() {
    int n, m;
    cin >> n >> m;
    vector<vector<char>> A(n + 1, vector<char>(n + 1, 0));
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        A[u][v] = A[v][u] = 1;
    }
    bool completo = true;
    for (int u = 1; u <= n && completo; u++)
        for (int v = 1; v <= n && completo; v++)
            if (u != v && !A[u][v]) completo = false;
    cout << (completo ? "SI" : "NO") << "\n";
}

// ------------------------------------------------------------
// PROBLEMA 4: Clausura transitiva (¿v es alcanzable desde u?)
// Dado un grafo dirigido, responder q consultas "¿existe camino de u a v?".
// Útil: Floyd sobre matriz booleana precalcula alcanzabilidad en O(n^3),
// luego cada consulta es O(1).
// Límites: n <= 500, q grande.
// Input: n m q, m aristas "u v", q consultas "u v"
// Output: "SI" o "NO" por consulta
// ------------------------------------------------------------
void problema4() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<vector<char>> R(n + 1, vector<char>(n + 1, 0));
    for (int i = 1; i <= n; i++) R[i][i] = 1;
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        R[u][v] = 1;
    }
    for (int k = 1; k <= n; k++)
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                if (R[i][k] && R[k][j]) R[i][j] = 1;
    while (q--) {
        int u, v; cin >> u >> v;
        cout << (R[u][v] ? "SI" : "NO") << "\n";
    }
}

// ------------------------------------------------------------
// PROBLEMA 5: Grado de cada vértice y detectar vértice universal
// Un vértice universal está conectado a todos los demás.
// En un grafo no dirigido: grado(u) == n-1.
// Útil: matriz para sumar filas en Θ(n) y detectar grados.
// Límites: n <= 2000.
// Input: n m, luego m líneas "u v"
// Output: IDs de vértices universales (o -1 si ninguno)
// ------------------------------------------------------------
void problema5() {
    int n, m;
    cin >> n >> m;
    vector<vector<char>> A(n + 1, vector<char>(n + 1, 0));
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        A[u][v] = A[v][u] = 1;
    }
    bool found = false;
    for (int u = 1; u <= n; u++) {
        int g = 0;
        for (int v = 1; v <= n; v++) g += A[u][v];
        if (g == n - 1) { cout << u << "\n"; found = true; }
    }
    if (!found) cout << -1 << "\n";
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
