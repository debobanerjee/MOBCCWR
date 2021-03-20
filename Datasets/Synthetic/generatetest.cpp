#include<bits/stdc++.h>
using namespace std;

#define ll long long int
#define LD long double

const int N = 100010;

int inf = 1e9;
int mod = 1e9 + 7;

int blockSizeT[24] = {4, 44, 100, 236, 121, 99, 104, 65, 83, 48, 53, 48, 35, 46, 36, 33, 42, 28, 40,
                      42, 21, 25, 33, 33};
int blockSizeB[12] = {25, 18, 29, 27, 29, 17, 22, 9, 13, 18, 1, 1};


int blockAvgT[24] = {182, 270, 366, 473, 572, 674, 774, 876, 976, 1057, 1167, 1271, 1376, 1484, 1576, 1679, 1790,
                     1888, 1994, 2087, 2183, 2294, 2391, 2496};
int blockAvgB[12] = {554, 667, 755, 857, 964, 1065, 1175, 1244, 1368, 1472, 1799, 2499};

double blockStDevT[24] = {17.088, 25.1595, 28.8791, 28.1603, 28.3019, 27.1662, 27.6043, 28.4253, 30.3315, 30.249,
                          33.2866, 27.4955, 25.318, 28.7924, 33.5708, 28.4605, 20.2237, 21.8861, 13.7113, 27.4044,
                          25.632, 13.2288, 22.6936, 8.7178};
double blockStDevB[12] = {29.5635, 22.8035, 32.5883, 34.3802, 33.4215, 29.6985, 29.7993, 28.1069,
                          23.2594, 30.2985, 0, 0};

int blockSize[24] = {4, 44, 100, 236, 146, 117, 133, 92, 112, 65, 75, 57, 48, 64, 36, 33, 43, 28, 40, 42, 21, 25, 33, 34};
int blockAvg[24] = {182, 270, 366, 473, 569, 673, 770, 871, 972, 1059, 1169, 1267, 1374, 1481, 1576, 1679, 1790, 1888,
                    1994, 2087, 2183, 2294};
double blockStDev[24] = {17.088, 25.1595, 28.8791, 28.1603, 29.2746, 26.6458, 29.7321, 31.4802, 31.607, 30.2985,
                         32.5115, 29.3598, 25.02, 29.7153, 33.5708, 28.4605, 20.025, 21.8861, 13.7113, 27.4044,
                         25.632, 13.2288, 22.6936, 8.48528};

int vers[6] = {964, 437, 176, 48, 3, 0};

int T = 1419;
int B = 209;
int O = 1628;

int getTopPrice() {
    int rn = rand() % T + 1;
    int blk = 0;
    int sum = 0;
    for(int i = 0; i < 24; i++) {
        sum += blockSizeT[i];
        if(rn <= sum) {
            blk = i;
            break;
        }
    }
    normal_distribution<double> distribution(blockAvgT[blk], blockStDevT[blk]);
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    while(true) {
        int val = (int)distribution(generator);
        if(!val) continue;
        return val;
    }
}

int getBottomPrice() {
    int rn = rand() % B + 1;
    int blk = 0;
    int sum = 0;
    for(int i = 0; i < 12; i++) {
        sum += blockSizeB[i];
        if(rn <= sum) {
            blk = i;
            break;
        }
    }
    normal_distribution<double> distribution(blockAvgB[blk], blockStDevB[blk]);
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    while(true) {
        int val = (int)distribution(generator);
        if(!val) continue;
        return val;
    }
}

int getVersatility() {
    int rn = rand() % O + 1;
    int sum = 0;
    for(int i = 0; i < 6; i++) {
        sum += vers[i];
        if(rn <= sum)
            return i + 1;
    }
    return 1;
}

bool is(int ty) {
    int x = rand() % 4;
    if(x <= ty) return true;
    return false;
}

int numtop = 1000;
int numbottom = 1000;
int curTy = 2;


signed main()
{
    //freopen("IN", "r", stdin);
    //freopen("OUT", "w", stdout);

    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    srand(time(0));

   	int val[10] = {2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, 512000, 1000000};
   	for(int _i = 0; _i < 10; _i++) {
   	   	string base = "test-" + to_string(val[_i]) + ".txt";
    	ofstream outfile(base);
       outfile << 2000 << " " << val[_i] << endl;

       for(int i = 1; i <= numtop; i++)
           outfile << i << " " << getVersatility() <<  " " << getTopPrice() << endl;
       for(int i = numtop + 1; i <= numtop + numbottom; i++)
           outfile << i << " " << getVersatility() << " " << getBottomPrice() << endl;

       set<pair<int,int>> E;
       	for(int i = 1; i <= numtop; i++) {
           	while(true) {
               	int x = rand() % numbottom + 1 + numtop;
               	if(E.find({i, x}) != E.end()) continue;
               	E.insert({i, x});
               	break;
           	}
       	}

       	for(int i = numtop + 1; i <= numtop + numbottom; i++) {
           	while(true) {
               	int x = rand() % numtop + 1;
               	if(E.find({x, i}) != E.end()) continue;
               	E.insert({x, i});
               	break;
    	   	}
       	}

       	cerr << (int)E.size() << endl;

       	while((int)E.size() < val[_i]) {
           int a = rand() % numtop + 1;
           int b = rand() % numbottom + 1 + numtop;
           if(E.find({a, b}) != E.end()) continue;
           E.insert({a, b});
       	}

       	for(auto it : E)
    	   outfile << it.first << " " << it.second << endl;

       	cerr << "Done: " << val[_i] << endl;
   	}

    return 0;
}
