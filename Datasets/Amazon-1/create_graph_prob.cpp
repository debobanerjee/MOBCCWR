#include<bits/stdc++.h>
using namespace std;

double RATE = 1;

signed main()
{
    freopen("preprocess_data_generated.txt", "r", stdin);
    freopen("input_graph_prob.txt", "w", stdout);

    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    int numOutfits; cin >> numOutfits;
    int numItems = 0;
    
    map <string, double> topPrice, bottomPrice;
    map <string, set<int>> topOccasions, bottomOccasions;
    map <pair<string, string>,int> outfits;
    map <pair<string, string>,int> cnt_outfits;

    int t = numOutfits;

    while(t--) {
        string top, bottom;
        cin >> top >> bottom;
        
        double t_price, b_price;
        cin >> t_price >> b_price;
        
        if(topPrice.find(top) != topPrice.end()) {
            assert(abs(topPrice[top] - t_price) <= 1e-9);
        }
        
        if(bottomPrice.find(bottom) != bottomPrice.end()) {
            assert(abs(bottomPrice[bottom] - b_price) <= 1e-9);
        }
        
        topPrice[top] = t_price;
        bottomPrice[bottom] = b_price;

        int m; cin >> m;
        while(m--) {
            int o; cin >> o;
            topOccasions[top].insert(o);
            bottomOccasions[bottom].insert(o);
        }

        int rating;
        cin >> rating;

        outfits[{top, bottom}] += rating;
        cnt_outfits[{top, bottom}]++;
    }

    numItems = (int)topPrice.size() + (int)bottomPrice.size();
    assert(numItems == (int)topOccasions.size() + (int)bottomOccasions.size());

    numOutfits = (int)outfits.size();

    cout << numItems << " " << numOutfits << "\n";
    for(auto it : topPrice) {
        string top = it.first;
        double price = it.second * RATE;
        double vers_score = (double)topOccasions[top].size();
        cout << top << " " << "0 " << vers_score << " " << price << "\n";
    }
    for(auto it : bottomPrice) {
        string bottom = it.first;
        double price = it.second * RATE;
        double vers_score = (double)bottomOccasions[bottom].size();
        cout << bottom << " " << "1 " << vers_score << " " << price << "\n";
    }

    for(auto it : outfits) {
        double rating = it.second;
        assert(cnt_outfits[it.first] > 0);
        rating /= (1.0 * cnt_outfits[it.first]);
        rating /= 5.0;
        cout << it.first.first << " " << it.first.second << " " << rating << "\n";
        //cout << it.first.first << " " << it.first.second << "\n";
    }

    return 0;
}
