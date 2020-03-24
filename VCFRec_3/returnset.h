//
// Created by Shubham on 3/24/2020.
//

#ifndef CODE_RETURNSET_H
#define CODE_RETURNSET_H

#include <bits/stdc++.h>
using namespace std;

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

#endif //CODE_RETURNSET_H
