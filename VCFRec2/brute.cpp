#include<bits/stdc++.h>
using namespace std;

#define ALPHA 8
#define BETA 3

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

signed main()
{
    freopen("random_input", "r", stdin);
    freopen("optimal_sol", "w", stdout);

    int tests; cin >> tests;
    for(int cur_test = 1; cur_test <= tests; cur_test++) {
        //cout << "-----------Running test " << cur_test << " -------------\n";
        T.reset();
        int n, m; cin >> n >> m;
        int K; cin >> K;
        int p[n + 1], w[n + 1];
        vector<int> graph[n + 1];
        for(int i = 1; i <= n; i++) {
            cin >> p[i] >> w[i];
        }
        for(int i = 0; i < m; i++) {
            int u, v; cin >> u >> v;
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
        int opt_profit = 0;
        for(int i = 0; i < (1 << n); i++) {
            set<int> s;
            int P = 0, W = 0;
            for(int j = 0; j < n; j++) {
                if((i >> j) & 1) {
                    P += p[j + 1];
                    W += w[j + 1];
                    s.insert(j + 1);
                }
            }
            bool key = true;
            int total = 0;
            for(int u : s) {
                int cnt = 0;
                for(int v : graph[u]) {
                    if(s.find(v) != s.end()) {
                        cnt++;
                    }
                }
                total += cnt;
                if(!graph[u].empty() && !cnt) {
                    key = false;
                    break;
                }
            }
            if(key) {
                if(W <= K) {
                    total /= 2;
                    int profit = ALPHA * total + BETA * P;
                    opt_profit = max(opt_profit, profit);
                }
            }
        }
        cerr << "Done: " << cur_test << " in " << T.elapsed() << ".\n";
        cout << opt_profit << endl;
        //cout << "OPT Solution: " << opt_profit << endl;
    }

    return 0;
}
