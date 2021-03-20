#include <bits/stdc++.h>
#include "mobccwr.h"
#include "dataset.h"
#include "baseline.h"
using namespace std;

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
}timer;
/** END **/

void readDataset(ifstream &in) {
	int numItems, numOutfits;

	in >> numItems >> numOutfits;

	map <string, PITEM_NODE> M;

	for(int i = 0; i < numItems; i++) {
		string info;
		TPROFIT profit;
		TCOST cost;
		int type;

		in >> info >> type >> profit >> cost;

		if(!(cost > 0)) {
			cout << info << endl;
		}
		assert(cost > 0);
		assert(type == 0 || type == 1);

		if(!i) MIN_ITEM_COST = cost;
		else MIN_ITEM_COST = min(MIN_ITEM_COST, cost);

		TITEM itemType;
		if(!type) itemType = TopWear;
		else itemType = BottomWear;

		PITEM_NODE itemNode = new ITEM_NODE;

		ITEM_NODE_INIT(itemNode, itemType, profit, cost, info);

		M[info] = itemNode;
	}

	for(int i = 0; i < numOutfits; i++) {
		PITEMS_EDGE itemEdge = new ITEMS_EDGE;

		string u, v; in >> u >> v;

		PITEM_NODE item1 = M[u];
		PITEM_NODE item2 = M[v];

		if(item1->cost >= 2500 - 1e-6 || item2->cost >= 2500 - 1e-6) {}
		else NUM_OUTFITS++;

		if(!i) MIN_OUTFIT_COST = item1->cost + item2->cost;
		else MIN_OUTFIT_COST = min(MIN_OUTFIT_COST, item1->cost + item2->cost);

		ITEMS_EDGE_INIT(itemEdge, item1, item2);
	}
}

void readDatasetProb(ifstream &in) {
	int numItems, numOutfits;

	in >> numItems >> numOutfits;

	map <string, PITEM_NODE> M;

	for(int i = 0; i < numItems; i++) {
		string info;
		TPROFIT profit;
		TCOST cost;
		int type;

		in >> info >> type >> profit >> cost;

		if(!(cost > 0)) {
			cout << info << endl;
		}
		assert(cost > 0);
		assert(type == 0 || type == 1);

		if(!i) MIN_ITEM_COST = cost;
		else MIN_ITEM_COST = min(MIN_ITEM_COST, cost);

		TITEM itemType;
		if(!type) itemType = TopWear;
		else itemType = BottomWear;

		PITEM_NODE itemNode = new ITEM_NODE;

		ITEM_NODE_INIT(itemNode, itemType, profit, cost, info);

		M[info] = itemNode;
	}

	for(int i = 0; i < numOutfits; i++) {
		PITEMS_EDGE itemEdge = new ITEMS_EDGE;

		TPROFIT profit;
		string u, v; 
		in >> u >> v >> profit;

		assert(abs(profit) > 1e-9);

		PITEM_NODE item1 = M[u];
		PITEM_NODE item2 = M[v];

		if(item1->cost >= 2500 - 1e-6 || item2->cost >= 2500 - 1e-6) {}
		else NUM_OUTFITS++;

		if(!i) MIN_OUTFIT_COST = item1->cost + item2->cost;
		else MIN_OUTFIT_COST = min(MIN_OUTFIT_COST, item1->cost + item2->cost);

		ITEMS_EDGE_INIT(itemEdge, item1, item2, profit);
	}
}



PRESULT solve(TPARAM alpha, TPARAM beta, TCOST budget) {
	GRAPH G(alpha, beta);
	PRESULT result = new RESULT;

	double executionTimeRatio = 0.0;
	double executionTimePseudo = 0.0;

	timer.reset();
	cout << "Ratio part is running...\n";

	long long numIterations = 0;

	TCOST remBudget = budget;

	RETURN_SET ratioReturnSet;
	RETURN_SET_INIT(&ratioReturnSet);

	while(true) {
		numIterations++;

		RETURN_SET returnSet = G.BEST_RATIO(remBudget);
		
		if(IS_RETURN_SET_EMPTY(&returnSet)) {
			break;
		}

		assert(returnSet.totalCost <= remBudget + 1e-6);
		remBudget -= returnSet.totalCost;

		ADD_RETURN_SET_TO_RETURN_SET(&ratioReturnSet, &returnSet);
	}

	executionTimeRatio = timer.elapsed();

	timer.reset();
	cout << "Ratio calculated in " << fixed << setprecision(2) << executionTimeRatio << "s.\n\n";

	cout << "Pseudo Profit part is running...\n";

	RETURN_SET pseudoReturnSet;
	RETURN_SET_INIT(&pseudoReturnSet);

	// pseudoReturnSet = G.BEST_PSEUDO_PROFIT_EDGE_PROB(budget);
	pseudoReturnSet = G.BEST_PSEUDO_PROFIT_EDGE(budget);

	executionTimePseudo = timer.elapsed();

	cout << "Pseudo Profit calculated in " << fixed << setprecision(2) << executionTimePseudo << "s.\n\n";

	if(abs(ratioReturnSet.totalProfit - pseudoReturnSet.totalProfit) <= 1e-9) {
		if(ratioReturnSet.totalCost < pseudoReturnSet.totalCost) 
			result->returnSetAns = ratioReturnSet;
		else
			result->returnSetAns = pseudoReturnSet;
	}
	else if(ratioReturnSet.totalProfit > pseudoReturnSet.totalProfit) result->returnSetAns = ratioReturnSet;
	else result->returnSetAns = pseudoReturnSet;

	result->ALPHA = alpha;
	result->BETA = beta;
	result->BUDGET = budget;
	result->returnSetRatio = ratioReturnSet;
	result->returnSetPseudo = pseudoReturnSet;
	result->executionTimeRatio = executionTimeRatio;
	result->executionTimePseudo = executionTimePseudo;
	result->executionTimeTotal = executionTimeRatio + executionTimePseudo;

	return result;
}

PRESULT solveRandomBaseline(TPARAM alpha, TPARAM beta, TCOST budget) {
	// RANDOM1 R(alpha, beta);
	// HIGHPROFIT R(alpha, beta);
	LOWCOST R(alpha, beta);

	PRESULT result = new RESULT;

	RETURN_SET returnSet = R.GET_RESULT(budget);

	result->returnSetAns = returnSet;
	result->ALPHA = alpha;
	result->BETA = beta;
	result->BUDGET = budget;
	result->returnSetRatio = returnSet;
	result->returnSetPseudo = returnSet;
	result->executionTimeRatio = 0;
	result->executionTimePseudo = 0;
	result->executionTimeTotal = 0;

	return result;
}

bool VERIFY_RESULT(PRESULT result) {
	TPROFIT totalProfit = 0.0;
	TCOST totalCost = 0.0;

	set<long long> top, bottom;
	long long numOutfits = 0;

	TPROFIT PS = 0, CS = 0, VS = 0;

	if(result->returnSetAns.totalCost > result->BUDGET)
		return false;

	for(PITEM_NODE item : result->returnSetAns.returnItems) {
		assert(top.find(item->itemHash) == top.end());
		assert(bottom.find(item->itemHash) == bottom.end());
		// assert(item->cost <= 2500);

		if(item->type == TopWear) top.insert(item->itemHash);
		else bottom.insert(item->itemHash);

		totalCost += item->cost;
		totalProfit += result->BETA * item->profit;

		VS += item->profit;
	}

	cout << "Topwears:\n";
	for(long long u : top) {
		cout << AllItems[u]->itemHash << " " << AllItems[u]->cost << " " << AllItems[u]->profit << "\n";
	}
	cout << "\n";

	cout << "Bottomwears:\n";
	for(long long u : bottom) {
		cout << AllItems[u]->itemHash << " " << AllItems[u]->cost << " " << AllItems[u]->profit << "\n";
	}
	cout << "\n";

	cout << "Outfits:\n";
	for(long long u : top) {
		for(long long v : bottom) {
			if(hashEdges.find({u, v}) != hashEdges.end()) {
				long long edgeHash = hashEdges[{u, v}];
				totalProfit += result->ALPHA * AllEdges[edgeHash]->profit;
				numOutfits++;
				CS += AllEdges[edgeHash]->profit;
				cout << "[" << AllItems[u]->itemHash << " " << AllItems[v]->itemHash << "]\n";
			}
		}
	}
	// cout << "\n";

	PS = totalProfit;

	if(abs(result->returnSetAns.totalProfit - totalProfit) < 1e-6 && abs(result->returnSetAns.totalCost - totalCost) < 1e-6) {
		result->numTop = (long long)top.size();
		result->numBottom = (long long)bottom.size();
		result->numCompOutfits = numOutfits;
		result->PS = PS;
		result->CS = CS;
		result->VS = VS;
		return true;
	}

	return false;
}

int main()
{
	srand(time(NULL));

	cout << "Enter Dataset File Path: ";
	
	string inPath;
	cin >> inPath;

	ifstream in;

	try {
		in = ifstream(inPath);
	}
	catch(...) {
		cerr << "Invalid path\n";
		return 0;
	}

	cout << "Reading the Dataset....\n";
	readDataset(in);
	// readDatasetProb(in);
	cout << "Done reading...\n\n";

	cout << "Preprocessing started.\n";
	timer.reset();
	PREPROCESS_INIT();
	cout << "Done Preprocessing in " << fixed << setprecision(2) << timer.elapsed() << "s.\n\n";

	string outPath = "./result/result.csv";
	ofstream out(outPath);

	cout << "Analyzing the data...\n";
	ANALYZE_DATASET(inPath, out);
	cout << "Done Analyzing.\n\n";

	TPARAM beta[5] = {0, 0.25, 0.5, 0.75, 1.0};
	TPARAM alpha[5] = {1, 0.75, 0.5, 0.25, 0};

	out << "MAIN RESULT\n";
	out << "Note: Execution time is in milliseconds and prices are in USD.\n\n";
	out << "Budget\t" << "Alpha\t" << "Beta\t" << "TotalProfitReturn\t" << "ExecutionTime (in ms)\t" << "NumTopMain\t" << "NumBottomMain\t" << "NumCompOutfitsMain\t" << "ProfitBestRatioEdge\t" << "ProfitBestPseudoNodes\n";

	cout << "Running algorithm...\n";
	for(int i = 0; i < 5; i++) {
		for(TCOST budget = 1000.0; budget <= 5000.0; budget += 500.0) {
			// PRESULT result = solveRandomBaseline(alpha[i], beta[i], budget);
			PRESULT result = solve(alpha[i], beta[i], budget);
			assert(VERIFY_RESULT(result));
			// PRINT_RESULT_ROW_BASELINE(result, out);
			// PRINT_RESULT_RUNTIME(result, out);
			PRINT_RESULT_ROW(result, out);
			// PRINT_RESULT_ROW_PSCSVS(result, out);
		}
	}

	// PRESULT result = solve(0.75, 0.25, 1000);
	// assert(VERIFY_RESULT(result));
	// PRINT_RESULT_ROW(result, out);

	cout << "Done !!!\n\n";

	return 0;
}
