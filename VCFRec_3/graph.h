//
// Created by Shubham on 3/24/2020.
//

#ifndef CODE_GRAPH_H
#define CODE_GRAPH_H

#include <bits/stdc++.h>
#include "node3.h"
#include "returnset.h"
using namespace std;

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
        assert(u != v);
        int x = indexInNodes[u];
        int y = indexInNodes[v];
        int tx = nodes[x].type;
        int ty = nodes[y].type;
        if(adj[x].find(y) == adj[x].end()) {
            degree[ty][x]++;
            degree[tx][y]++;
            old_degree[ty][x]++;
            old_degree[tx][y]++;
            adj[x].insert(y);
            adj[y].insert(x);
            numEdges++;
        }
    }

    int getProfit(int node) {
        return nodes[indexInNodes[node]].p;
    }
    int getWeight(int node) {
        return nodes[indexInNodes[node]].w;
    }
    int getType(int node) {
        return nodes[indexInNodes[node]].type;
    }
    bool isConnected(int u, int v) {
        int x = indexInNodes[u];
        int y = indexInNodes[v];
        return (adj[x].find(y) != adj[x].end());
    }

    bool isTriangle(int t, int b, int f) {
        return isConnected(t, b) && isConnected(t, f) && isConnected(b, f);
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


#endif //CODE_GRAPH_H
