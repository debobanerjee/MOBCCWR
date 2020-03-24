//
// Created by Shubham on 3/24/2020.
//

#ifndef CODE_QKNAPSACK_H
#define CODE_QKNAPSACK_H

#include<bits/stdc++.h>
#include "returnset.h"
using namespace std;

struct QKP {
    static returnSet PROFIT_KNAPSACK(vector<int> &nodes, vector<int> &profits, vector<int> &weights, int budget,
                              vector<pair<pair<int, int>,int>> &extraProfit) {
        assert((int) nodes.size() == (int) profits.size());
        assert((int) nodes.size() == (int) weights.size());

        map<int, set<int>> adj;
        map<pair<int,int>,int> edgeProfit;
        for (auto &it : extraProfit) {
            int u = it.first.first;
            int v = it.first.second;
            int w = it.second;
            adj[u].insert(v);
            adj[v].insert(u);
            edgeProfit[{u, v}] = w;
            edgeProfit[{v, u}] = w;
        }
        int n = (int) nodes.size();
        int M = budget;
        int dp[2][M + 1];
        vector<int> last[2][M + 1];
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j <= M; j++) {
                dp[i][j] = 0;
            }
        }
        dp[0][0] = 0;
        int cur = 0;
        for (int i = 1; i <= n; i++) {
            cur = 1 - cur;
            for (int j = 0; j <= M; j++) {
                dp[cur][j] = dp[1 - cur][j];
                last[cur][j] = last[1 - cur][j];
            }
            int v = nodes[i - 1];
            for (int j = 0; j <= M; j++) {
                if(j > 0 && last[1 - cur][j].empty())
                    continue;
                int x = j + weights[i - 1];
                if (x <= M) {
                    int extra = 0;
                    for (int u : last[1 - cur][j]) {
                        if (adj[u].find(v) != adj[u].end())
                            extra += edgeProfit[{u, v}];
                    }
                    if (dp[cur][x] <= dp[1 - cur][j] + profits[i - 1] + extra) {
                        dp[cur][x] = dp[1 - cur][j] + profits[i - 1] + extra;
                        last[cur][x] = last[1 - cur][j];
                        last[cur][x].push_back(v);
                    }
                }
            }
        }
        int knapsack_sol = 0;
        pair<int, int> knapsack_wt = {0, 0};
        for (int j = 0; j <= M; j++) {
            if (!last[cur][j].empty()) {
                if (knapsack_wt.first < dp[cur][j]) {
                    knapsack_wt = {dp[cur][j], j};
                    knapsack_sol = j;
                }
            }
        }

        returnSet U;
        for (int u : last[cur][knapsack_sol]) {
            U.addNode(u, 0, 0);
        }
        U.totalProfit = knapsack_wt.first;
        U.totalWeight = knapsack_wt.second;
        return U;
    }

    static returnSet RATIO_KNAPSACK(vector<int> &nodes, vector<int> &profits, vector<int> &weights, int budget,
                             int off_p, int off_w, vector<pair<pair<int, int>,int>> &extraProfit) {
        assert((int) nodes.size() == (int) profits.size());
        assert((int) nodes.size() == (int) weights.size());

        map<int, set<int>> adj;
        map<pair<int,int>,int> edgeProfit;
        for (auto &it : extraProfit) {
            int u = it.first.first;
            int v = it.first.second;
            int w = it.second;
            adj[u].insert(v);
            adj[v].insert(u);
            edgeProfit[{u, v}] = w;
            edgeProfit[{v, u}] = w;
        }
        int n = (int) nodes.size();
        int M = budget;
        int dp[2][M + 1];
        vector<int> last[2][M + 1];
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j <= M; j++) {
                dp[i][j] = 0;
            }
        }
        dp[0][0] = 0;
        int cur = 0;
        for (int i = 1; i <= n; i++) {
            cur = 1 - cur;
            for (int j = 0; j <= M; j++) {
                dp[cur][j] = dp[1 - cur][j];
                last[cur][j] = last[1 - cur][j];
            }
            int v = nodes[i - 1];
            for (int j = 0; j <= M; j++) {
                if(j > 0 && last[1 - cur][j].empty())
                    continue;
                int x = j + weights[i - 1];
                if (x <= M) {
                    int extra = 0;
                    for (int u : last[1 - cur][j]) {
                        if (adj[u].find(v) != adj[u].end())
                            extra += edgeProfit[{u, v}];
                    }
                    if (dp[cur][x] <= dp[1 - cur][j] + profits[i - 1] + extra) {
                        dp[cur][x] = dp[1 - cur][j] + profits[i - 1] + extra;
                        last[cur][x] = last[1 - cur][j];
                        last[cur][x].push_back(v);
                    }
                }
            }
        }
        int knapsack_sol = 0;
        pair<int, int> knapsack_wt = {0, 0};
        for (int j = 0; j <= M; j++) {
            if (!last[cur][j].empty()) {
                if (!knapsack_sol) {
                    knapsack_sol = j;
                    knapsack_wt = {dp[cur][j], j};
                    continue;
                }
                int x = dp[cur][j] + off_p;
                int y = j + off_w;
                if ((knapsack_wt.first + off_p) * y <= (knapsack_wt.second + off_w) * x) {
                    knapsack_wt = {x - off_p, y - off_w};
                    knapsack_sol = j;
                }
            }
        }

        returnSet U;
        for (int u : last[cur][knapsack_sol]) {
            U.addNode(u, 0, 0);
        }
        U.totalProfit = knapsack_wt.first;
        U.totalWeight = knapsack_wt.second;
        return U;
    }
};

#endif //CODE_QKNAPSACK_H
