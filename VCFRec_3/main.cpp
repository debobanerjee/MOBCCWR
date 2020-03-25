#include<bits/stdc++.h>
#include "timer.h"
#include "returnset.h"
#include "graph.h"
#include "trioutfit.h"
#include "qknapsack.h"
using namespace std;

int ALPHA = 5;
int BETA = 3;

Timer timer;
trioutfit To;
QKP qkp;

returnSet BEST_PROFIT_VIABLE(graph &G, int K) {
    returnSet U;
    for(int center : G.activeNodes) {
        if(G.getWeight(center) > K)
            continue;
        vector<int> nodes, profits, weights;
        vector<pair<pair<int,int>,int>> extraProfit;
        int idx = G.indexInNodes[center];
        vector<int> outers;
        for(int u : G.adj[idx]) {
            if(!G.isActiveIndex(u))
                continue;
            outers.push_back(G.nodes[u].u);
            weights.push_back(G.nodes[u].w);
            int profit = To.lazy[G.nodes[u].u];
            profit += To.getEdgeProfit(G, G.nodes[u].u, center, ALPHA, BETA);
            profits.push_back(profit);
            nodes.push_back(G.nodes[u].u);
        }
        for(int i = 0; i < (int)outers.size(); i++) {
            int u = outers[i];
            for(int j = i + 1; j < (int)outers.size(); j++) {
                int v = outers[j];
                if(u == v) continue;
                if(G.isConnected(u, v)) {
                    int profit = To.getEdgeProfit(G, u, v, ALPHA, BETA);
                    profit += ALPHA;
                    profit += To.getOutfitProfit(u, v, center) * BETA;
                    extraProfit.push_back({{u, v}, profit});
                }
            }
        }
        int off_p = To.lazy[center];
        int off_w = G.getWeight(center);
        returnSet Z = qkp.PROFIT_KNAPSACK(nodes, profits, weights, K - off_w, extraProfit);
        Z.addNode(center, off_p, off_w);
        if(Z.totalProfit > U.totalProfit)
            U = Z;
    }
    return U;
}

returnSet BEST_RATIO_VIABLE(graph &G, int K) {
    returnSet U;
    for(int center : G.activeNodes) {
        if(G.getWeight(center) > K)
            continue;
        vector<int> nodes, profits, weights;
        vector<pair<pair<int,int>,int>> extraProfit;
        int idx = G.indexInNodes[center];
        vector<int> outers;
        for(int u : G.adj[idx]) {
            if(!G.isActiveIndex(u))
                continue;
            outers.push_back(G.nodes[u].u);
            weights.push_back(G.nodes[u].w);
            int profit = To.lazy[G.nodes[u].u];
            profit += To.getEdgeProfit(G, G.nodes[u].u, center, ALPHA, BETA);
            profits.push_back(profit);
            nodes.push_back(G.nodes[u].u);
        }
        for(int u : outers) {
            for(int v : outers) {
                if(u == v) continue;
                if(G.isConnected(u, v)) {
                    int profit = To.getEdgeProfit(G, u, v, ALPHA, BETA);
                    profit += ALPHA;
                    profit += To.getOutfitProfit(u, v, center) * BETA;
                    extraProfit.push_back({{u, v}, profit});
                }
            }
        }
        int off_p = To.lazy[center];
        int off_w = G.getWeight(center);
        returnSet Z = qkp.RATIO_KNAPSACK(nodes, profits, weights, K - off_w, off_p, off_w, extraProfit);
        Z.addNode(center, off_p, off_w);
        if(Z.totalProfit) {
            if(U.isEmpty()) U = Z;
            else if(U.totalProfit * Z.totalWeight <= U.totalWeight * Z.totalProfit)
                U = Z;
        }
    }
    return U;
}

returnSet GREEDY_VCFREC3(graph G, int k) {
    returnSet Smax = BEST_PROFIT_VIABLE(G, k);
    int K = k;
    vector<int> Z;
    returnSet U;
    while(true) {
        returnSet Si = BEST_RATIO_VIABLE(G, K);
        int si = -1, pv = -1, wv = -1;
        for(int z : Z) {
            if(G.getWeight(z) <= K) {
                int profit = To.lazy[z];
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
        To.remove(G, Si, ALPHA, BETA);
    }

    if(Smax.totalProfit > U.totalProfit)
        return Smax;
    return U;
}

bool verify(returnSet U, graph G, int budget) {
    int totalWeight = 0, totalProfit = 0;
    for(int u : U.returnNodes) {
        totalWeight += G.getWeight(u);
    }

    if(totalWeight != U.totalWeight || totalWeight > budget)
        return false;

    vector<int> items[3];
    for(int u : U.returnNodes) {
        items[G.getType(u)].push_back(u);
    }

    for(int t : items[0]) {
        for(int b : items[1]) {
            for(int f : items[2]) {
                if(G.isTriangle(t, b, f)) {
                    totalProfit += ALPHA;
                    totalProfit += BETA * To.getOutfitProfit(t, b, f);
                }
            }
        }
    }

    return totalProfit == U.totalProfit;
}

void printSol(graph &G, int budget) {
    returnSet U = GREEDY_VCFREC3(G, budget);
    assert(verify(U, G, budget));
    cout << U.totalProfit << "\n";
    /*cout << "Greedy Solution: ";
    for(int u : U.returnNodes) {
        cout << u << " ";
    }
    cout << "\nTotal Profit: " << U.totalProfit << " Total Weight: " << U.totalWeight << "\n\n";*/
}

void solve() {
    int n, m; cin >> n >> m;
    graph G;
    for(int i = 0; i < n; i++) {
        int u, w, type; cin >> u >> w >> type;
        G.addNode(u, 0, w, type);
    }
    for(int i = 0; i < m; i++) {
        int t, b, f, v; cin >> t >> b >> f >> v;
        G.addEdge(t, b);
        G.addEdge(t, f);
        G.addEdge(b, f);
        To.addOutfit(t, b, f, v);
    }
    int budget = 800; //cin >> budget;
    printSol(G, budget);
    To.clear();
}

void printSolGUI(graph &G, int budget) {
    returnSet U = GREEDY_VCFREC3(G, budget);
    assert(verify(U, G, budget));
    vector<int> items[3];
    for(int u : U.returnNodes) {
        items[G.getType(u)].push_back(u);
    }

    for(int t : items[0]) {
        for(int b : items[1]) {
            for(int f : items[2]) {
                if(G.isTriangle(t, b, f)) {
                    cout << t << " " << b << " " << f << "\n";
                }
            }
        }
    }
}

void solveGUI() {
    int n, m; cin >> n >> m;
    graph G;
    for(int i = 0; i < n; i++) {
        int u, w, type; cin >> u >> w >> type;
        G.addNode(u, 0, w, type);
    }
    for(int i = 0; i < m; i++) {
        int t, b, f, v; cin >> t >> b >> f >> v;
        G.addEdge(t, b);
        G.addEdge(t, f);
        G.addEdge(b, f);
        To.addOutfit(t, b, f, v);
    }
    int alpha, beta;
    int budget = 800; cin >> alpha >> beta >> budget;
    ALPHA = alpha;
    BETA = beta;
    printSolGUI(G, budget);
    To.clear();
}

signed main()
{
    //freopen("real_data", "r", stdin);
    //freopen("greedy_result", "w", stdout);

    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    int test = 1; //cin >> test;
    for(int cur_test = 1; cur_test <= test; cur_test++) {
        timer.reset();
        solveGUI();
        //cerr << "Done: " << cur_test << " in " << timer.elapsed() << ".\n";
    }

    return 0;
}
