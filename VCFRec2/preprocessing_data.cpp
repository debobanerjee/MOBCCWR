#include<bits/stdc++.h>
using namespace std;

signed main()
{
    freopen("amazon_data_graph", "r", stdin);
    //freopen("input_graph", "w", stdout);
    freopen("reverse_hashing", "w", stdout);

    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    int t; cin >> t;

    string u, v;
    map<int,set<int>> occasions;
    map<string,int> M;
    int n = 0;
    int m = 0;
    map<int,int> weight;
    vector<pair<int,int>> graph;
    map<int,string> outfit;

    while(t--) {
        cin >> u >> v;
        m++;
        double wd1, wd2; cin >> wd1 >> wd2;
        int w1 = (int)wd1; int w2 = (int)wd2;
        int k; cin >> k;
        if(M.find(u) == M.end()) {
            n++;
            weight[n] = w1;
            M[u] = n;
            outfit[n] = u;
        }
        if(M.find(v) == M.end()) {
            n++;
            weight[n] = w2;
            M[v] = n;
            outfit[n] = v;
        }
        while(k--) {
            int x; cin >> x;
            occasions[M[u]].insert(x);
            occasions[M[v]].insert(x);
        }
        graph.emplace_back(M[u], M[v]);
    }

    /*cout << n << " " << m << "\n";
    for(int i = 1; i <= n; i++)
        cout << i << " " << (int)occasions[i].size() << " " << weight[i] << "\n";

    for(auto it : graph)
        cout << it.first << " " << it.second << "\n";
    */

    for(int i = 1; i <= n; i++)
        cout << outfit[i] << "\n";
    return 0;
}
