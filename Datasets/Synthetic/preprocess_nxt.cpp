#include<bits/stdc++.h>
using namespace std;

double RATE = 74.13;

signed main()
{
    // freopen("real_data", "r", stdin);
    // freopen("dataset_fin.txt", "w", stdout);

    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    int numOutfits;
    int numItems;
    cin >> numItems >> numOutfits;
    
    map <string, int> vers, price;
    map <string, int> type;

    for(int i = 0; i < numItems; i++) {
        string info;
        int v, p;
        cin >> info >> v >> p;
        vers[info] = v;
        price[info] = p;
    }

    set < pair<string,string> > O;

    for(int i = 0; i < numOutfits;i++) {
        string u, v;
        cin >> u >> v;
        type[u] = 0;
        type[v] = 1;
        O.insert({u, v});
    }

    cout << numItems << " " << numOutfits << endl;
    assert(numItems == (int)vers.size() && numItems == (int)price.size() && numItems == (int)type.size());

    for(auto it : vers) {
        cout << it.first << " " << type[it.first] << " " << it.second << " " << price[it.first] << endl;
    }

    assert((int)O.size() == numOutfits);

    for(auto it : O) {
        cout << it.first << " " << it.second << endl;
    }

    return 0;
}