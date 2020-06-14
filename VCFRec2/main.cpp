#include<bits/stdc++.h>
using namespace std;

const int N = 1510;

int ALPHA = 1;
int BETA = 1;

/** To check Time Limit **/
class Timer {
private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;
    std::chrono::time_point<clock_t> m_beg;
public:
    Timer() : m_beg(clock_t::now()) {}
    void reset() { m_beg = clock_t::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
}T;
/** END **/

struct returnSet {
    vector<int> returnNodes;
    int totalProfit, totalWeight;

    explicit returnSet() {
        totalProfit = 0;
        totalWeight = 0;
    }

    void addNode(int u, int p, int w) {
        returnNodes.push_back(u);
        totalProfit += p;
        totalWeight += w;
    }

    void addReturnSet(const returnSet &U) {
        totalProfit += U.totalProfit;
        totalWeight += U.totalWeight;
        for(int u : U.returnNodes)
            returnNodes.push_back(u);
    }

    bool isEmpty() {
        return returnNodes.empty();
    }

    void clear() {
        returnNodes.clear();
        totalWeight = 0;
        totalProfit = 0;
    }
};

struct Node {
    int u, p, w;
    explicit Node(int u, int p, int w) : u(u), p(p), w(w) {}
};

struct graph {
    vector<Node> nodes;
    vector<int> degree, old_degree;
    vector<set<int>> adj;
    map<int,int> indexInNodes;
    set<int> activeNodes;
    int numEdges = 0;

    void addNode(int u, int p, int w) {
        Node temp(u, p, w);
        nodes.push_back(temp);
        degree.push_back(0);
        old_degree.push_back(0);
        adj.emplace_back();
        indexInNodes[u] = (int)nodes.size() - 1;
        activeNodes.insert(u);
    }

    void addEdge(int u, int v) {
        int x = indexInNodes[u];
        int y = indexInNodes[v];
        degree[x]++; degree[y]++;
        old_degree[x]++; old_degree[y]++;
        adj[x].insert(y);
        adj[y].insert(x);
        numEdges++;
    }

    int getProfit(int node) {
        return nodes[indexInNodes[node]].p;
    }
    int getWeight(int node) {
        return nodes[indexInNodes[node]].w;
    }

    bool isConnected(int u, int v) {
        int x = indexInNodes[u];
        int y = indexInNodes[v];
        return adj[x].find(y) != adj[x].end();
    }

    bool isActiveIndex(int u) {
        int x = nodes[u].u;
        return activeNodes.find(x) != activeNodes.end();
    }

    bool isActive(int u) {
        return activeNodes.find(u) != activeNodes.end();
    }

    void remove(const returnSet &U) {
        for(int u : U.returnNodes) {
            int idx = indexInNodes[u];
            for(int v : adj[idx]) {
                if(isActiveIndex(v)) {
                    degree[v]--;
                }
            }
        }
        for(int u : U.returnNodes) {
            int idx = indexInNodes[u];
            degree[idx] = 0;
            activeNodes.erase(u);
        }
        numEdges = 0;
        for(int u : activeNodes)
            numEdges += degree[indexInNodes[u]];
        numEdges /= 2;
    }

    vector<int> getNeighbours(const returnSet &U) {
        set<int> getNeighbours;
        for(int u : U.returnNodes) {
            int idx = indexInNodes[u];
            for(int v : adj[idx]) {
                if(isActiveIndex(v))
                    getNeighbours.insert(nodes[v].u);
            }
        }
        return vector<int> (getNeighbours.begin(), getNeighbours.end());
    }
};

int dp[N][10 * N], last[N][10 * N];

returnSet PROFIT_KNAPSACK(vector<int> &nodes, vector<int> &profits, vector<int> &weights, int budget) {

    assert((int)nodes.size() == (int)profits.size());
    assert((int)nodes.size() == (int)weights.size());

    int n = (int)nodes.size();
    int M = budget;

    for(int i = 0; i <= n; i++) {
        for(int j = 0; j <= M; j++) {
            dp[i][j] = 0;
            last[i][j] = -1;
        }
    }
    dp[0][0] = 0; last[0][0] = 0;
    for(int i = 1; i <= n; i++) {
        for(int j = 0; j <= M; j++) {
            dp[i][j] = dp[i - 1][j];
        }
        for(int j = 0; j <= M; j++) {
            int x = j + weights[i - 1];
            if(x <= M) {
                if(dp[i][x] < dp[i - 1][j] + profits[i - 1]) {
                    dp[i][x] = dp[i - 1][j] + profits[i - 1];
                    last[i][x] = i;
                }
            }
        }
    }
    pair<int,int> knapsack_sol = {0, 0};
    for(int i = 0; i <= n; i++) {
        for(int j = 0; j <= M; j++) {
            int x = knapsack_sol.first;
            int y = knapsack_sol.second;
            if(dp[x][y] < dp[i][j] && last[i][j] != -1) {
                knapsack_sol = {i, j};
            }
        }
    }

    returnSet U;
    int x = knapsack_sol.first;
    int y = knapsack_sol.second;
    while(true) {
        if(!x && !y) break;
        U.addNode(nodes[x - 1], profits[x - 1], weights[x - 1]);
        y -= weights[x - 1];
        x--;
        while(last[x][y] == -1) x--;
    }

    return U;
}

returnSet RATIO_KNAPSACK(vector<int> &nodes, vector<int> &profits, vector<int> &weights, int budget,
                         int off_p, int off_w) {
    assert((int)nodes.size() == (int)profits.size());
    assert((int)nodes.size() == (int)weights.size());

    int n = (int)nodes.size();
    int M = budget;

    for(int i = 0; i <= n; i++) {
        for(int j = 0; j <= M; j++) {
            dp[i][j] = 0;
            last[i][j] = -1;
        }
    }
    dp[0][0] = 0; last[0][0] = 0;
    for(int i = 1; i <= n; i++) {
        for(int j = 0; j <= M; j++) {
            dp[i][j] = dp[i - 1][j];
        }
        for(int j = 0; j <= M; j++) {
            int x = j + weights[i - 1];
            if(x <= M) {
                if(dp[i][x] < dp[i - 1][j] + profits[i - 1]) {
                    dp[i][x] = dp[i - 1][j] + profits[i - 1];
                    last[i][x] = i;
                }
            }
        }
    }
    pair<int,int> knapsack_sol = {0, 0};
    pair<int,int> knapsack_wt = {0, 0};
    for(int j = 0; j <= M; j++) {
        for(int i = 1; i <= n; i++) {
            if(last[i][j] != -1) {
                if(!knapsack_sol.first && !knapsack_sol.second) {
                    knapsack_sol = {i, j};
                    knapsack_wt = {dp[i][j] + off_p, j + off_w};
                    continue;
                }
                int x = dp[i][j] + off_p;
                int y = j + off_w;
                if(knapsack_wt.first * y <= knapsack_wt.second * x) {
                    knapsack_wt = {x, y};
                    knapsack_sol = {i, j};
                }
            }
        }
    }

    returnSet U;
    int x = knapsack_sol.first;
    int y = knapsack_sol.second;
    while(true) {
        if(!x && !y) break;
        U.addNode(nodes[x - 1], profits[x - 1], weights[x - 1]);
        y -= weights[x - 1];
        x--;
        while(last[x][y] == -1) x--;
    }

    return U;
}

returnSet BEST_PROFIT_VIABLE(graph G, int k) {
    returnSet U;
    for(int center : G.activeNodes) {
        if(G.getWeight(center) > k) continue;
        vector<int> nodes, profits, weights;
        bool isSingle = true;
        for(int u : G.adj[G.indexInNodes[center]]) {
            if(!G.isActiveIndex(u)) continue;
            int c_cap = G.old_degree[u] - G.degree[u] + 1;
            int p_cap = G.nodes[u].p;
            nodes.push_back(G.nodes[u].u);
            profits.push_back(c_cap * ALPHA + p_cap * BETA);
            weights.push_back(G.nodes[u].w);
            isSingle = false;
        }
        int x = G.indexInNodes[center];
        int off_p = (G.old_degree[x] - G.degree[x]) * ALPHA + G.getProfit(center) * BETA;
        int off_w = G.getWeight(center);
        returnSet Z = PROFIT_KNAPSACK(nodes, profits, weights, k - G.getWeight(center));
        if(!isSingle && Z.isEmpty()) continue;
        if(isSingle) continue;
        Z.addNode(center, off_p, off_w);
        if(U.totalProfit < Z.totalProfit)
            U = Z;
    }
    return U;
}

returnSet BEST_RATIO_VIABLE(graph G, int k) {
    returnSet U;
    for(int center : G.activeNodes) {
        if(G.getWeight(center) > k) continue;
        vector<int> nodes, profits, weights;
        bool isSingle = true;
        for(int u : G.adj[G.indexInNodes[center]]) {
            if(!G.isActiveIndex(u)) continue;
            int c_cap = G.old_degree[u] - G.degree[u] + 1;
            int p_cap = G.nodes[u].p;
            nodes.push_back(G.nodes[u].u);
            profits.push_back(c_cap * ALPHA + p_cap * BETA);
            weights.push_back(G.nodes[u].w);
            isSingle = false;
        }
        int x = G.indexInNodes[center];
        int off_p = (G.old_degree[x] - G.degree[x]) * ALPHA + G.getProfit(center) * BETA;
        int off_w = G.getWeight(center);
        returnSet Z = RATIO_KNAPSACK(nodes, profits, weights, k - G.getWeight(center), off_p, off_w);
        if(!isSingle && Z.isEmpty()) continue;
        if(isSingle) continue;
        Z.addNode(center, off_p, off_w);
        if(U.isEmpty()) U = Z;
        else if(U.totalProfit * Z.totalWeight <= U.totalWeight * Z.totalProfit)
            U = Z;
    }
    return U;
}

returnSet GREEDY_1_NEIGHBOUR(graph G, int k) {
    returnSet Smax = BEST_PROFIT_VIABLE(G, k);
    int K = k;
    vector<int> Z;
    returnSet U;
    while(true) {
        returnSet Si = BEST_RATIO_VIABLE(G, K);
        int si = -1, pv = -1, wv = -1;
        for(int z : Z) {
            if(G.getWeight(z) <= K) {
                int c_cap = G.old_degree[G.indexInNodes[z]] - G.degree[G.indexInNodes[z]];
                int p_cap = G.getProfit(z);
                int profit = c_cap * ALPHA + p_cap * BETA;
                if(si == -1) {
                    pv = profit;
                    wv = G.getWeight(z);
                    si = z;
                    continue;
                }
                if(pv * G.getWeight(z) < wv * profit) {
                    si = z;
                    pv = profit;
                    wv = G.getWeight(z);
                }
            }
        }

        if(Si.isEmpty() && si == -1) break;

        if(Si.isEmpty()) Si.addNode(si, pv, wv);
        else if(si != -1) {
            if(Si.totalProfit * wv < Si.totalWeight * pv) {
                Si.clear();
                Si.addNode(si, pv, wv);
            }
        }

        G.remove(Si);
        U.addReturnSet(Si);
        K -= Si.totalWeight;
        Z = G.getNeighbours(U);
    }

    if(Smax.totalProfit > U.totalProfit)
        return Smax;
    return U;
}

bool verifySolution(returnSet &U, graph &G) {
    int totalWeight = 0, totalProfit = 0;
    set<int> curNodes;
    for(int u : U.returnNodes) {
        totalWeight += G.getWeight(u);
        totalProfit += G.getProfit(u) * BETA;
        int x = G.indexInNodes[u];
        for(int v : curNodes) {
            if(G.adj[x].find(v) != G.adj[x].end())
                totalProfit += ALPHA;
        }
        curNodes.insert(x);
    }
    return (U.totalWeight == totalWeight && U.totalProfit == totalProfit);
}

void printSol(graph &G, int budget) {
    T.reset();
    returnSet U = GREEDY_1_NEIGHBOUR(G, budget);
    assert(verifySolution(U, G));
    assert(U.totalWeight <= budget);

    cout << U.totalProfit << "\n";
    cerr << "Done in " << T.elapsed() << ".\n";
}

void readGraph() {
    int n, m, budget; cin >> n >> m >> budget;
    graph G;
    for(int i = 1; i <= n; i++) {
        int p, w; cin >> p >> w;
        G.addNode(i, p, w);
    }
    for(int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        G.addEdge(u, v);
    }
    printSol(G, budget);
}

void printGUISol(graph &G, int budget, set<int> &top, set<int> &bottom) {
    returnSet U = GREEDY_1_NEIGHBOUR(G, budget);
    assert(verifySolution(U, G));
    assert(U.totalWeight <= budget);
    vector<int> Utop, Ubottom;
    for(int u : U.returnNodes) {
        if(top.find(u) != top.end())
            Utop.push_back(u);
        else Ubottom.push_back(u);
    }
    for(int u : Utop) {
        for(int v : Ubottom) {
            if(G.isConnected(u, v)) {
                cout << u << " " << v << "\n";
            }
        }
    }

}

void readAmazonData() {
    int n, m; cin >> n >> m;
    graph G;
    for(int i = 0; i < n; i++) {
        int u, p, w; cin >> u >> p >> w;
        G.addNode(u, p, w);
    }
    for(int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        G.addEdge(u, v);
    }
    int budget = 1000;
    cin >> ALPHA >> BETA >> budget;
    printSol(G, budget);
}

void readGuiData() {
    int n, m; cin >> n >> m;
    graph G;
    for(int i = 0; i < n; i++) {
        int u, p, w; cin >> u >> p >> w;
        G.addNode(u, p, w);
    }
    set<int> top, bottom;
    for(int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        G.addEdge(u, v);
        top.insert(u);
        bottom.insert(v);
    }
    int budget, alpha, beta;
    cin >> budget >> alpha >> beta;
    ALPHA = alpha; BETA = beta;
    printGUISol(G, budget, top, bottom);
}

signed main()
{
    freopen("real_data", "r", stdin);
    //freopen("greedy_result", "w", stdout);

    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    int test = 1; //cin >> test;
    //double avg_time = 0.0;
    for(int cur_test = 1; cur_test <= test; cur_test++) {
        T.reset();
        readGuiData();
        cerr << "Done: " << cur_test << " in " << T.elapsed() << ".\n";
        //avg_time += T.elapsed();
    }
    //cerr << "\nAvg time: " << avg_time / test << "\n";
    return 0;
}
