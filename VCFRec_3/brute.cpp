#include<bits/stdc++.h>
#include "timer.h"
using namespace std;

int ALPHA = 5;
int BETA = 3;
int BUDGET = 800;

Timer T;

const int N = 23;
int totalWeight[(1 << N)], totalComp[(1 << N)], totalVers[(1 << N)];

signed main()
{
    freopen("random_input", "r", stdin);
    freopen("optimal_sol", "w", stdout);

    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    double avg_time = 0.0;

    int tests; cin >> tests;
    for(int cur_test = 1; cur_test <= tests; cur_test++) {
        T.reset();
        int n, m;
        cin >> n >> m;
        int K = BUDGET;

        int type[n + 1];
        int w[n + 1];
        for(int i = 1; i <= n; i++) {
            int x; cin >> x >> w[i] >> type[i];
        }

        map <pair<pair<int,int>,int>, int> M;

        for(int i = 0; i < m; i++) {
            int t, b, f, w; cin >> t >> b >> f >> w;
            M[{{t, b}, f}] = w;
        }

        int opt = 0;
        for(int i = 0; i < (1 << n); i++) {
            vector<int> v;
            int weight = 0;
            for(int j = 0; j < n; j++) {
                if((i >> j) & 1) {
                    v.push_back(j + 1);
                    weight += w[j + 1];
                }
            }
            if(weight > K) continue;
            int profit = 0;
            for(int t : v) {
                for(int b : v) {
                    for(int f : v) {
                        if(type[t] == 0 && type[b] == 1 && type[f] == 2) {
                            profit += ALPHA + BETA * M[{{t, b}, f}];
                        }
                    }
                }
            }
            opt = max(opt, profit);
        }

        cout << opt << "\n";

        cerr << "Done: " << cur_test << " in " << T.elapsed() << ".\n";
        avg_time += T.elapsed();
    }

    cerr << "\nAvg time: " << avg_time / tests << "\n";

    return 0;
}
