#ifndef BASELINE_H
#define BASELINE_H

#include <bits/stdc++.h>
#include "mobccwr.h"
using namespace std;


struct RANDOM1 {
	vector<bool> IsItemSelected;
	vector<bool> IsEdgeSelected;
	
	TPARAM ALPHA, BETA;

	RANDOM1(TPARAM alpha, TPARAM beta) {
		ALPHA = alpha;
		BETA = beta;

		IsItemSelected.resize(ITEM_COUNTER, false);
		IsEdgeSelected.resize(EDGE_COUNTER, false);
	}

	void RESET() {
		IsItemSelected.assign(ITEM_COUNTER, false);
		IsEdgeSelected.assign(EDGE_COUNTER, false);	
	}

	TCOST GET_COST_EDGE(long long edgeHash) {
		long long itemHash1 = AllEdges[edgeHash]->item1->itemHash;
		long long itemHash2 = AllEdges[edgeHash]->item2->itemHash;

		TCOST cost = 0;
		if(!IsItemSelected[itemHash1]) cost += AllItems[itemHash1]->cost;
		if(!IsItemSelected[itemHash2]) cost += AllItems[itemHash2]->cost;

		return cost;
	}

	TPROFIT GET_PROFIT_EDGE(long long edgeHash) {
		long long itemHash1 = AllEdges[edgeHash]->item1->itemHash;
		long long itemHash2 = AllEdges[edgeHash]->item2->itemHash;

		TPROFIT profit = 0;
		if(!IsItemSelected[itemHash1]) profit += BETA * AllItems[itemHash1]->profit;
		if(!IsItemSelected[itemHash2]) profit += BETA * AllItems[itemHash2]->profit;

		profit += ALPHA * AllEdges[edgeHash]->profit;
		return profit;
	}

	RETURN_SET RANDOM_ALGO(TCOST BUDGET) {
		TCOST budget = BUDGET;

		RETURN_SET returnSet;
		RETURN_SET_INIT(&returnSet);

		while(true) {
			long long edgeChoose = -1;
			for(long long i = 0; i < EDGE_COUNTER; i++) {
				if(IsEdgeSelected[i]) continue;
				
				TCOST cost = GET_COST_EDGE(i);
			
				if(cost > budget - 1e-9) continue;

				if(edgeChoose == -1) {
					edgeChoose = i;
					continue;
				}

				long long j = rand() % (i + 1);
				if(j == 0) {
					edgeChoose = i;
				}

			}

			if(edgeChoose == -1) break;
			
			long long itemHash1 = AllEdges[edgeChoose]->item1->itemHash;
			long long itemHash2 = AllEdges[edgeChoose]->item2->itemHash;

			if(!IsItemSelected[itemHash1]) ADD_ITEM_TO_RETURN_SET(&returnSet, AllItems[itemHash1], BETA * AllItems[itemHash1]->profit);
			if(!IsItemSelected[itemHash2]) ADD_ITEM_TO_RETURN_SET(&returnSet, AllItems[itemHash2], BETA * AllItems[itemHash2]->profit);			

			budget -= GET_COST_EDGE(edgeChoose);

			IsItemSelected[itemHash1] = true;
			IsItemSelected[itemHash2] = true;
			IsEdgeSelected[edgeChoose] = true;

			returnSet.totalProfit += ALPHA * AllEdges[edgeChoose]->profit;
		}

		return returnSet;
	}

	RETURN_SET GET_RESULT(TCOST BUDGET) {
		RETURN_SET returnSet;
		RETURN_SET_INIT(&returnSet);

		for(int i = 0; i < 10; i++) {
			RESET();
			RETURN_SET curReturnSet = RANDOM_ALGO(BUDGET);
			if(curReturnSet.totalProfit >= returnSet.totalProfit - 1e-9) {
				returnSet = curReturnSet;
			}
		}

		return returnSet;
	}

};


struct HIGHPROFIT {
	vector<bool> IsItemSelected;
	vector<bool> IsEdgeSelected;
	
	TPARAM ALPHA, BETA;

	HIGHPROFIT(TPARAM alpha, TPARAM beta) {
		ALPHA = alpha;
		BETA = beta;

		IsItemSelected.resize(ITEM_COUNTER, false);
		IsEdgeSelected.resize(EDGE_COUNTER, false);
	}

	void RESET() {
		IsItemSelected.assign(ITEM_COUNTER, false);
		IsEdgeSelected.assign(EDGE_COUNTER, false);	
	}

	TCOST GET_COST_EDGE(long long edgeHash) {
		long long itemHash1 = AllEdges[edgeHash]->item1->itemHash;
		long long itemHash2 = AllEdges[edgeHash]->item2->itemHash;

		TCOST cost = 0;
		if(!IsItemSelected[itemHash1]) cost += AllItems[itemHash1]->cost;
		if(!IsItemSelected[itemHash2]) cost += AllItems[itemHash2]->cost;

		return cost;
	}

	TPROFIT GET_PROFIT_EDGE(long long edgeHash) {
		long long itemHash1 = AllEdges[edgeHash]->item1->itemHash;
		long long itemHash2 = AllEdges[edgeHash]->item2->itemHash;

		TPROFIT profit = 0;
		if(!IsItemSelected[itemHash1]) profit += BETA * AllItems[itemHash1]->profit;
		if(!IsItemSelected[itemHash2]) profit += BETA * AllItems[itemHash2]->profit;

		profit += ALPHA * AllEdges[edgeHash]->profit;
		return profit;
	}

	RETURN_SET HIGHPROFIT_ALGO(TCOST BUDGET) {
		TCOST budget = BUDGET;

		RETURN_SET returnSet;
		RETURN_SET_INIT(&returnSet);

		while(true) {
			long long edgeChoose = -1;
			TPROFIT curProfit = 0.0;
			for(long long i = 0; i < EDGE_COUNTER; i++) {
				if(IsEdgeSelected[i]) continue;
				
				TCOST cost = GET_COST_EDGE(i);
			
				if(cost > budget - 1e-9) continue;

				if(edgeChoose == -1) {
					edgeChoose = i;
					curProfit = GET_PROFIT_EDGE(i);
					continue;
				}

				TPROFIT profit = GET_PROFIT_EDGE(i);
				if(curProfit <= profit + 1e-9) {
					curProfit = profit;
					edgeChoose = i;
				}
			}

			if(edgeChoose == -1) break;
			
			long long itemHash1 = AllEdges[edgeChoose]->item1->itemHash;
			long long itemHash2 = AllEdges[edgeChoose]->item2->itemHash;

			if(!IsItemSelected[itemHash1]) ADD_ITEM_TO_RETURN_SET(&returnSet, AllItems[itemHash1], BETA * AllItems[itemHash1]->profit);
			if(!IsItemSelected[itemHash2]) ADD_ITEM_TO_RETURN_SET(&returnSet, AllItems[itemHash2], BETA * AllItems[itemHash2]->profit);			

			budget -= GET_COST_EDGE(edgeChoose);

			IsItemSelected[itemHash1] = true;
			IsItemSelected[itemHash2] = true;
			IsEdgeSelected[edgeChoose] = true;

			returnSet.totalProfit += ALPHA * AllEdges[edgeChoose]->profit;
		}

		return returnSet;
	}

	RETURN_SET GET_RESULT(TCOST BUDGET) {
		RETURN_SET returnSet;
		RETURN_SET_INIT(&returnSet);

		for(int i = 0; i < 1; i++) {
			RESET();
			RETURN_SET curReturnSet = HIGHPROFIT_ALGO(BUDGET);
			if(curReturnSet.totalProfit >= returnSet.totalProfit - 1e-9) {
				returnSet = curReturnSet;
			}
		}

		return returnSet;
	}
};

struct LOWCOST {
	vector<bool> IsItemSelected;
	vector<bool> IsEdgeSelected;
	
	TPARAM ALPHA, BETA;

	LOWCOST(TPARAM alpha, TPARAM beta) {
		ALPHA = alpha;
		BETA = beta;

		IsItemSelected.resize(ITEM_COUNTER, false);
		IsEdgeSelected.resize(EDGE_COUNTER, false);
	}

	void RESET() {
		IsItemSelected.assign(ITEM_COUNTER, false);
		IsEdgeSelected.assign(EDGE_COUNTER, false);	
	}

	TCOST GET_COST_EDGE(long long edgeHash) {
		long long itemHash1 = AllEdges[edgeHash]->item1->itemHash;
		long long itemHash2 = AllEdges[edgeHash]->item2->itemHash;

		TCOST cost = 0;
		if(!IsItemSelected[itemHash1]) cost += AllItems[itemHash1]->cost;
		if(!IsItemSelected[itemHash2]) cost += AllItems[itemHash2]->cost;

		return cost;
	}

	TPROFIT GET_PROFIT_EDGE(long long edgeHash) {
		long long itemHash1 = AllEdges[edgeHash]->item1->itemHash;
		long long itemHash2 = AllEdges[edgeHash]->item2->itemHash;

		TPROFIT profit = 0;
		if(!IsItemSelected[itemHash1]) profit += BETA * AllItems[itemHash1]->profit;
		if(!IsItemSelected[itemHash2]) profit += BETA * AllItems[itemHash2]->profit;

		profit += ALPHA * AllEdges[edgeHash]->profit;
		return profit;
	}

	RETURN_SET LOWCOST_ALGO(TCOST BUDGET) {
		TCOST budget = BUDGET;

		RETURN_SET returnSet;
		RETURN_SET_INIT(&returnSet);

		while(true) {
			long long edgeChoose = -1;
			TPROFIT curCost = 0.0;
			for(long long i = 0; i < EDGE_COUNTER; i++) {
				if(IsEdgeSelected[i]) continue;
				
				TCOST cost = GET_COST_EDGE(i);
			
				if(cost > budget - 1e-9) continue;

				if(edgeChoose == -1) {
					edgeChoose = i;
					curCost = cost;
					continue;
				}

				if(curCost > cost + 1e-9) {
					curCost = cost;
					edgeChoose = i;
				}
			}

			if(edgeChoose == -1) break;
			
			long long itemHash1 = AllEdges[edgeChoose]->item1->itemHash;
			long long itemHash2 = AllEdges[edgeChoose]->item2->itemHash;

			if(!IsItemSelected[itemHash1]) ADD_ITEM_TO_RETURN_SET(&returnSet, AllItems[itemHash1], BETA * AllItems[itemHash1]->profit);
			if(!IsItemSelected[itemHash2]) ADD_ITEM_TO_RETURN_SET(&returnSet, AllItems[itemHash2], BETA * AllItems[itemHash2]->profit);			

			budget -= GET_COST_EDGE(edgeChoose);

			IsItemSelected[itemHash1] = true;
			IsItemSelected[itemHash2] = true;
			IsEdgeSelected[edgeChoose] = true;

			returnSet.totalProfit += ALPHA * AllEdges[edgeChoose]->profit;
		}

		return returnSet;
	}

	RETURN_SET GET_RESULT(TCOST BUDGET) {
		RETURN_SET returnSet;
		RETURN_SET_INIT(&returnSet);

		for(int i = 0; i < 1; i++) {
			RESET();
			RETURN_SET curReturnSet = LOWCOST_ALGO(BUDGET);
			if(curReturnSet.totalProfit >= returnSet.totalProfit - 1e-9) {
				returnSet = curReturnSet;
			}
		}

		return returnSet;
	}
};

#endif //BASELINE_H