#include<bits/stdc++.h>
using namespace std;

struct O {
    int t, b, f, v;
};
signed main()
{
    freopen("amazon_data_graph", "r", stdin);
    freopen("input_graph", "w", stdout);
    //freopen("reverse_hashing", "w", stdout);

    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    int t; cin >> t;

    string u, v, w;
    map<int,set<int>> occasions;
    map<string,int> M;
    int n = 0;
    int m = 0;
    map<int,int> weight;
    vector<O> graph;
    map<int,string> outfit;
    map<int,int> type;

    while(t--) {
        cin >> u >> v >> w;
        m++;
        double wd1, wd2, wd3; cin >> wd1 >> wd2 >> wd3;
        int w1 = (int)wd1; int w2 = (int)wd2; int w3 = (int)wd3;
        int k; cin >> k;
        if(M.find(u) == M.end()) {
            n++;
            weight[n] = w1;
            M[u] = n;
            outfit[n] = u;
            type[n] = 0;
        }
        if(M.find(v) == M.end()) {
            n++;
            weight[n] = w2;
            M[v] = n;
            outfit[n] = v;
            type[n] = 1;
        }
        if(M.find(w) == M.end()) {
            n++;
            weight[n] = w3;
            M[w] = n;
            outfit[n] = w;
            type[n] = 2;
        }
        while(k--) {
            int x; cin >> x;
        }
        O temp;
        temp.t = M[u]; temp.b = M[v]; temp.f = M[w]; temp.v = k;
        graph.emplace_back(temp);
    }

    cout << n << " " << m << "\n";
    for(int i = 1; i <= n; i++)
        cout << i << " " << weight[i] << " " << type[i] << "\n";

    for(O it : graph)
        cout << it.t << " " << it.b << " " << it.f << " " << it.v << "\n";

    /*for(int i = 1; i <= n; i++)
        cout << outfit[i] << "\n";*/
    return 0;
}