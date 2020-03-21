#include<bits/stdc++.h>
using namespace std;

#define ALPHA 2
#define BETA 4

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
    int u, p, w, type;
    explicit Node(int u, int p, int w, int type) : u(u), p(p), w(w), type(type) {}
};

struct graph {
    vector<Node> nodes;
    vector<int> degree[3], old_degree[3];
    vector<set<int>> adj;
    map<int,int> indexInNodes;
    set<int> activeNodes;
    int numEdges = 0;

    void addNode(int u, int p, int w, int type) {
        Node temp(u, p, w, type);
        nodes.push_back(temp);
        for(int i = 0; i < 3; i++) {
            degree[i].push_back(0);
            old_degree[i].push_back(0);
        }
        adj.emplace_back();
        indexInNodes[u] = (int)nodes.size() - 1;
        activeNodes.insert(u);
    }

    void addEdge(int u, int v) {
        int x = indexInNodes[u];
        int y = indexInNodes[v];
        int tx = nodes[x].type;
        int ty = nodes[y].type;
        degree[ty][x]++; degree[tx][y]++;
        old_degree[ty][x]++; old_degree[tx][y]++;
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
            int tx = nodes[idx].type;
            for(int v : adj[idx]) {
                if(isActiveIndex(v)) {
                    degree[tx][v]--;
                }
            }
        }
        for(int u : U.returnNodes) {
            int idx = indexInNodes[u];
            for(int i = 0; i < 3; i++) degree[i][idx] = 0;
            activeNodes.erase(u);
        }
        numEdges = 0;
        for(int u : activeNodes)
            for(int i = 0; i < 3; i++)
                numEdges += degree[i][indexInNodes[u]];
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

returnSet PROFIT_KNAPSACK(vector<int> &nodes, vector<int> &profits, vector<int> &weights, int budget,
        vector<pair<int,int>> &extra_profit) {
    assert((int)nodes.size() == (int)profits.size());
    assert((int)nodes.size() == (int)weights.size());

    map<int,set<int>> adj;
    for(auto &it : extra_profit) {
        adj[it.first].insert(it.second);
        adj[it.second].insert(it.first);
    }
    int n = (int)nodes.size();
    int M = budget;
    int dp[n + 1][M + 1];
    vector<int> last[n + 1][M + 1];
    int NINF = 0;
    for(int i = 0; i <= n; i++) {
        for(int j = 0; j <= M; j++) {
            dp[i][j] = NINF;
        }
    }
    dp[0][0] = 0;
    for(int i = 1; i <= n; i++) {
        for(int j = 0; j <= M; j++) {
            dp[i][j] = dp[i - 1][j];
            last[i][j] = last[i - 1][j];
        }
        for(int j = 0; j <= M; j++) {
            int x = j + weights[i - 1];
            if(x <= M) {
                int extra = 0;
                for(int u : last[i - 1][j]) {
                    if(adj[u].find(i) != adj[u].end())
                        extra += ALPHA;
                }
                if(dp[i][x] < dp[i - 1][j] + profits[i - 1] + extra) {
                    dp[i][x] = dp[i - 1][j] + profits[i - 1] + extra;
                    last[i][x] = last[i - 1][j];
                    last[i][x].push_back(i);
                }
            }
        }
    }
    int knapsack_sol = 0;
    pair<int,int> knapsack_wt = {0, 0};
    for(int j = 0; j <= M; j++) {
        if(!last[n][j].empty()) {
            if(knapsack_wt.first < dp[n][j]) {
                knapsack_wt = {dp[n][j], j};
                knapsack_sol = j;
            }
        }
    }

    returnSet U;
    for(int u : last[n][knapsack_sol]) {
        U.addNode(nodes[u - 1], 0, 0);
    }
    U.totalProfit = knapsack_wt.first;
    U.totalWeight = knapsack_wt.second;
    return U;
}

returnSet RATIO_KNAPSACK(vector<int> &nodes, vector<int> &profits, vector<int> &weights, int budget,
                         int off_p, int off_w, vector<pair<int,int>> &extra_profit) {
    assert((int)nodes.size() == (int)profits.size());
    assert((int)nodes.size() == (int)weights.size());

    map<int,set<int>> adj;
    for(auto &it : extra_profit) {
        adj[it.first].insert(it.second);
        adj[it.second].insert(it.first);
    }
    int n = (int)nodes.size();
    int M = budget;
    int dp[n + 1][M + 1];
    vector<int> last[n + 1][M + 1];
    int NINF = 0;
    for(int i = 0; i <= n; i++) {
        for(int j = 0; j <= M; j++) {
            dp[i][j] = NINF;
        }
    }
    dp[0][0] = 0;
    for(int i = 1; i <= n; i++) {
        for(int j = 0; j <= M; j++) {
            dp[i][j] = dp[i - 1][j];
            last[i][j] = last[i - 1][j];
        }
        for(int j = 0; j <= M; j++) {
            int x = j + weights[i - 1];
            if(x <= M) {
                int extra = 0;
                for(int u : last[i - 1][j]) {
                    if(adj[u].find(i) != adj[u].end())
                        extra += ALPHA;
                }
                if(dp[i][x] < dp[i - 1][j] + profits[i - 1] + extra) {
                    dp[i][x] = dp[i - 1][j] + profits[i - 1] + extra;
                    last[i][x] = last[i - 1][j];
                    last[i][x].push_back(i);
                }
            }
        }
    }
    int knapsack_sol = 0;
    pair<int,int> knapsack_wt = {0, 0};
    for(int j = 0; j <= M; j++) {
        if(!last[n][j].empty()) {
            if(!knapsack_sol) {
                knapsack_sol = j;
                knapsack_wt = {dp[n][j] + off_p, j + off_w};
                continue;
            }
            int x = dp[n][j] + off_p;
            int y = j + off_w;
            if(knapsack_wt.first * y <= knapsack_wt.second * x) {
                knapsack_wt = {x, y};
                knapsack_sol = j;
            }
        }
    }

    returnSet U;
    for(int u : last[n][knapsack_sol]) {
        U.addNode(nodes[u - 1], 0, 0);
    }
    U.totalProfit = knapsack_wt.first;
    U.totalWeight = knapsack_wt.second;
    return U;
}

returnSet BEST_PROFIT_VIABLE(graph G, int k) {
    returnSet U;
    for(int center : G.activeNodes) {
        if(G.getWeight(center) > k) continue;
        vector<int> nodes, profits, weights;
        bool isSingle = true;
        set<int> outerNodes;
        vector<pair<int,int>> extra_profit;
        for(int u : G.adj[G.indexInNodes[center]]) {
            if(!G.isActiveIndex(u)) continue;
            int tu = G.nodes[u].type;
            vector<int> cnt_t;
            for(int i = 0; i < 3; i++)
                if(i != tu)
                    cnt_t.push_back(G.old_degree[i][u] - G.degree[i][u]);
            int c_cap = cnt_t[0] * cnt_t[1];
            int p_cap = G.nodes[u].p;
            nodes.push_back(G.nodes[u].u);
            profits.push_back(c_cap * ALPHA + p_cap * BETA);
            weights.push_back(G.nodes[u].w);
            for(int v : G.adj[u])
                if(outerNodes.find(v) != outerNodes.end())
                    extra_profit.emplace_back(u, v);
            outerNodes.insert(u);
            isSingle = false;
        }
        int x = G.indexInNodes[center];
        int t_center = G.nodes[x].type;
        vector<int> cnt_t;
        for(int i = 0; i < 3; i++)
            if(i != t_center)
                cnt_t.push_back(G.old_degree[i][x] - G.degree[i][x]);

        int off_p = cnt_t[0] * cnt_t[1] * ALPHA + G.getProfit(center) * BETA;
        int off_w = G.getWeight(center);

        returnSet Z = PROFIT_KNAPSACK(nodes, profits, weights, k - G.getWeight(center), extra_profit);
        if(!isSingle && Z.isEmpty()) continue;
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
        set<int> outerNodes;
        vector<pair<int,int>> extra_profit;
        for(int u : G.adj[G.indexInNodes[center]]) {
            if(!G.isActiveIndex(u)) continue;
            int tu = G.nodes[u].type;
            vector<int> cnt_t;
            for(int i = 0; i < 3; i++)
                if(i != tu)
                    cnt_t.push_back(G.old_degree[i][u] - G.degree[i][u]);
            int c_cap = cnt_t[0] * cnt_t[1];
            int p_cap = G.nodes[u].p;
            nodes.push_back(G.nodes[u].u);
            profits.push_back(c_cap * ALPHA + p_cap * BETA);
            weights.push_back(G.nodes[u].w);
            for(int v : G.adj[u])
                if(outerNodes.find(v) != outerNodes.end())
                    extra_profit.emplace_back(u, v);
            outerNodes.insert(u);
            isSingle = false;
        }

        int x = G.indexInNodes[center];
        int t_center = G.nodes[x].type;
        vector<int> cnt_t;
        for(int i = 0; i < 3; i++)
            if(i != t_center)
                cnt_t.push_back(G.old_degree[i][x] - G.degree[i][x]);

        int off_p = cnt_t[0] * cnt_t[1] * ALPHA + G.getProfit(center) * BETA;
        int off_w = G.getWeight(center);
        returnSet Z = RATIO_KNAPSACK(nodes, profits, weights, k - G.getWeight(center),
                                     off_p, off_w, extra_profit);
        if(!isSingle && Z.isEmpty()) continue;
        Z.addNode(center, 0, 0);
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
                int x = G.indexInNodes[z];
                int tx = G.nodes[x].type;
                vector<int> cnt_t;
                for(int i = 0; i < 3; i++)
                    if(i != tx)
                        cnt_t.push_back(G.old_degree[i][x] - G.degree[i][x]);

                int profit = cnt_t[0] * cnt_t[1] * ALPHA + G.getProfit(z) * BETA;
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

void printSol(graph &G, int budget) {
    returnSet U = GREEDY_1_NEIGHBOUR(G, budget);
    //cout << U.totalProfit << "\n";
    cout << "Greedy Solution: ";
    for(int u : U.returnNodes) {
        cout << u << " ";
    }
    cout << "\nTotal Profit: " << U.totalProfit << " Total Weight: " << U.totalWeight << "\n\n";
}

void readGraph() {
    int n, m, budget; cin >> n >> m >> budget;
    graph G;
    for(int i = 1; i <= n; i++) {
        int p, w, type; cin >> p >> w >> type;
        G.addNode(i, p, w, type);
    }
    for(int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        G.addEdge(u, v);
    }
    printSol(G, budget);
}

void readAmazonData() {
    int n, m; cin >> n >> m;
    graph G;
    for(int i = 0; i < n; i++) {
        int u, p, w, type; cin >> u >> p >> w >> type;
        G.addNode(u, p, w, type);
    }
    for(int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        G.addEdge(u, v);
    }
    int budget = 1000;  //Change budget here
    printSol(G, budget);
}

signed main()
{
    //freopen("random_input", "r", stdin);
    //freopen("greedy_result", "w", stdout);

    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    int test = 1; cin >> test;
    for(int cur_test = 1; cur_test <= test; cur_test++) {
        T.reset();
        readGraph();
        cerr << "Done: " << cur_test << " in " << T.elapsed() << ".\n";
    }

    return 0;
}
