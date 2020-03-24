//
// Created by Shubham on 3/24/2020.
//

#ifndef CODE_TRIOUTFIT_H
#define CODE_TRIOUTFIT_H

#include<bits/stdc++.h>
#include "graph.h"
#include "returnset.h"
using namespace std;

struct trioutfit {
    map <pair<pair<int,int>,int>, int> M;
    map <int,int> type, lazy;

    void addType(int t, int x) {
        if(type.find(t) != type.end())
            assert(type[t] == x);
        type[t] = x;
    }

    int getType(int t) {
        assert(type.find(t) != type.end());
        return type[t];
    }

    void addOutfit(int t, int b, int f, int v) {
        M[{{t, b}, f}] = v;
        addType(t, 0);
        addType(b, 1);
        addType(f, 2);
    }

    int getOutfitProfit(int u, int v, int w) {
        int t[3];
        t[0] = t[1] = t[2] = -1;
        t[getType(u)] = u;
        t[getType(v)] = v;
        t[getType(w)] = w;
        assert(t[0] != -1 && t[1] != -1 && t[2] != -1);
        return M[{{t[0], t[1]}, t[2]}];
    }

    void clear() {
        M.clear();
        type.clear();
        lazy.clear();
    }

    void remove(graph &G, returnSet &U, int ALPHA, int BETA) {
        set<int> s;
        for(int u : U.returnNodes) {
            s.insert(u);
        }
        for(int u : U.returnNodes) {
            int idx = G.indexInNodes[u];
            for(int v : G.adj[idx]) {
                if(G.isActiveIndex(v))
                    continue;
                if(s.find(G.nodes[v].u) != s.end()) {
                    if(idx > v)
                        continue;
                }
                for(int w : G.activeNodes) {
                    if(G.isConnected(u, w) && G.isConnected(G.nodes[v].u, w)) {
                        lazy[w] += ALPHA + BETA * getOutfitProfit(u, G.nodes[v].u, w);
                    }
                }
            }
        }
    }

    int getEdgeProfit(graph &G, int u, int v, int ALPHA, int BETA) {
        int x = G.indexInNodes[u];
        int y = G.indexInNodes[v];
        int profit = 0;
        for(int w : G.adj[x]) {
            if(G.isActiveIndex(w))
                continue;
            if(G.adj[y].find(w) != G.adj[y].end()) {
                profit += BETA * getOutfitProfit(u, v, G.nodes[w].u) + ALPHA;
            }
        }
        return profit;
    }
};

#endif //CODE_TRIOUTFIT_H
