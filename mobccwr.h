#ifndef MOBCCWR_H
#define MOBCCWR_H

#include <bits/stdc++.h>
using namespace std;

#define TCOST double
#define TPROFIT double
#define TRATIO double
#define TPARAM double

//Global ITEM_COUNTER
long long ITEM_COUNTER = 0;

//Global EDGE_COUNTER
long long EDGE_COUNTER = 0;

//Minimum price of the item
TCOST MIN_ITEM_COST = 0;

//Minimum cost of the outfit
TCOST MIN_OUTFIT_COST = 0;

//Items Type
enum TITEM {
	TopWear = 0,
	BottomWear = 1
};

//Stores the information related to the item
typedef struct {
	TITEM type;
	TPROFIT profit;
	TCOST cost;
	string info;
	long long itemHash;
} ITEM_NODE, *PITEM_NODE;

//Stores the edge information
typedef struct {
	PITEM_NODE item1, item2;
	TPROFIT profit;
	long long edgeHash;
} ITEMS_EDGE, *PITEMS_EDGE;

//Maintain the item list with the ITEM_COUNTER
vector<PITEM_NODE> AllItems;

//Maintain the edges with the EDGE_COUNTER
vector<PITEMS_EDGE> AllEdges;

//Maintain map of edges {itemHash1, itemHash2}
map <pair<long long, long long>, long long> hashEdges;

//Maintain information about group of items 
typedef struct {
	vector<PITEM_NODE> returnItems;
	TPROFIT totalProfit;
	TCOST totalCost;
} RETURN_SET, *PRETURN_SET;


//Create the item
void
ITEM_NODE_INIT(PITEM_NODE itemNode, 
	TITEM type, 
	TPROFIT profit,
	TCOST cost, 
	string info = "") 
{
	itemNode->type = type;
	itemNode->cost = cost;
	itemNode->info = info;
	itemNode->profit = profit;

	itemNode->itemHash = ITEM_COUNTER++;
	AllItems.push_back(itemNode);

	return;
}

//Create the edge
void
ITEMS_EDGE_INIT(PITEMS_EDGE itemsEdge,
	PITEM_NODE item1,
	PITEM_NODE item2,
	TPROFIT profit = 1)
{
	if(item1->type == BottomWear) {
		swap(item1, item2);
	}

	itemsEdge->item1 = item1;
	itemsEdge->item2 = item2;
	itemsEdge->profit = profit;

	itemsEdge->edgeHash = EDGE_COUNTER++;
	AllEdges.push_back(itemsEdge);

	hashEdges[{item1->itemHash, item2->itemHash}] = itemsEdge->edgeHash;
	
	return;		
}

//Initialize the return set
void
RETURN_SET_INIT(PRETURN_SET returnSet)
{
	returnSet->returnItems.clear();
	returnSet->totalCost = 0;
	returnSet->totalProfit = 0;
	return;
}

//Add item to the returnset
void
ADD_ITEM_TO_RETURN_SET(PRETURN_SET returnSet,
	PITEM_NODE itemNode)
{
	returnSet->returnItems.push_back(itemNode);
	returnSet->totalCost += itemNode->cost;
	returnSet->totalProfit += itemNode->profit;
	return;
}

//Add item to the returnset, another signature
void
ADD_ITEM_TO_RETURN_SET(PRETURN_SET returnSet,
	PITEM_NODE itemNode,
	TPROFIT profit)
{
	returnSet->returnItems.push_back(itemNode);
	returnSet->totalCost += itemNode->cost;
	returnSet->totalProfit += profit;
	return;
}

//Add edge to the returnset
void
ADD_EDGE_TO_RETURN_SET(PRETURN_SET returnSet,
	PITEMS_EDGE edgeNode)
{
	ADD_ITEM_TO_RETURN_SET(returnSet, edgeNode->item1);
	ADD_ITEM_TO_RETURN_SET(returnSet, edgeNode->item2);
	returnSet->totalProfit += edgeNode->profit;
	return;
}

//Add edge to the returnset, another signature
void
ADD_EDGE_TO_RETURN_SET(PRETURN_SET returnSet,
	PITEMS_EDGE edgeNode,
	TPROFIT profit)
{
	ADD_ITEM_TO_RETURN_SET(returnSet, edgeNode->item1, 0);
	ADD_ITEM_TO_RETURN_SET(returnSet, edgeNode->item2, 0);
	returnSet->totalProfit += profit;
	return;
}

//Add one returnset to other returnset
void
ADD_RETURN_SET_TO_RETURN_SET(PRETURN_SET returnSetTarget,
	PRETURN_SET returnSetSource)
{
	TPROFIT oldProfit = returnSetTarget->totalProfit;

	for(PITEM_NODE item : returnSetSource->returnItems)
		ADD_ITEM_TO_RETURN_SET(returnSetTarget, item);

	returnSetTarget->totalProfit = oldProfit + returnSetSource->totalProfit;
	return;
}

//return other item of the edge
PITEM_NODE
GET_OTHER_ITEM(PITEMS_EDGE edge,
	PITEM_NODE oneNode)
{
	assert(oneNode != NULL);
	if(edge->item1 == oneNode) return edge->item2;
	return edge->item1;
}

//check whether the returnset is empty or not
bool
IS_RETURN_SET_EMPTY(PRETURN_SET returnSet) 
{
	return returnSet->returnItems.empty();
}

//vector of {{cost, itemHash}, edgeHash}
vector < vector < pair < pair < TCOST, long long >, long long > > > PseudoAdjList;

//Do necessary preprocessing here
void
PREPROCESS_INIT()
{
	PseudoAdjList.resize(ITEM_COUNTER);

	for(int i = 0; i < EDGE_COUNTER; i++) {
		long long u = AllEdges[i]->item1->itemHash;
		long long v = AllEdges[i]->item2->itemHash;
		PseudoAdjList[u].push_back({{AllEdges[i]->item2->cost, v}, i});
		PseudoAdjList[v].push_back({{AllEdges[i]->item1->cost, u}, i});
	}

	for(int i = 0; i < ITEM_COUNTER; i++) {
		sort(PseudoAdjList[i].begin(), PseudoAdjList[i].end());
	}
}

//Graph structure necessary for the algorithm
struct GRAPH {
	vector<bool> IsItemSelected;
	vector<bool> IsEdgeSelected;

	vector<TPROFIT> ProfitOfItems;
	vector<TCOST> CostOfItems;
	vector<TPROFIT> ProfitOfEdges;

	vector<long long> RecentTimestampEdge;
	vector<long long> RecentTimestampItem;

	//{{RATIO, edgeHash}, Timestamp}
	priority_queue < pair < pair <TRATIO, long long>, long long > > EdgeQueue;

	//{{RATIO, itemHash}, Timestamp}
	priority_queue < pair < pair <TRATIO, long long>, long long > > ItemQueue;

	long long TIMESTAMP;

	vector < set <long long> > AdjList; 

	TPARAM ALPHA, BETA;

	GRAPH(TPARAM alpha, TPARAM beta) {
		ALPHA = alpha; BETA = beta;

		IsItemSelected.resize(ITEM_COUNTER, false);
		IsEdgeSelected.resize(EDGE_COUNTER, false);
		RecentTimestampEdge.resize(EDGE_COUNTER);
		RecentTimestampItem.resize(ITEM_COUNTER);
		AdjList.resize(ITEM_COUNTER);

		TIMESTAMP = 0;

		ProfitOfItems.resize(ITEM_COUNTER);
		CostOfItems.resize(ITEM_COUNTER);
		ProfitOfEdges.resize(EDGE_COUNTER);

		for(long long i = 0; i < ITEM_COUNTER; i++) {
			ProfitOfItems[i] = BETA * AllItems[i]->profit;
			CostOfItems[i] = AllItems[i]->cost;
		}

		for(long long i = 0; i < EDGE_COUNTER; i++) {
			ProfitOfEdges[i] = ALPHA * AllEdges[i]->profit;
		}

		for(long long i = 0; i < EDGE_COUNTER; i++) {
			TPROFIT profit = ProfitOfEdges[i];
			profit += ProfitOfItems[AllEdges[i]->item1->itemHash];
			profit += ProfitOfItems[AllEdges[i]->item2->itemHash];

			TCOST cost = 0;
			cost += CostOfItems[AllEdges[i]->item1->itemHash];
			cost += CostOfItems[AllEdges[i]->item2->itemHash];

			RecentTimestampEdge[i] = 0;

			TRATIO ratio = profit / cost;

			EdgeQueue.push({{ratio, i}, RecentTimestampEdge[i]});

			long long u = AllEdges[i]->item1->itemHash;
			long long v = AllEdges[i]->item2->itemHash;

			AdjList[u].insert(i);
			AdjList[v].insert(i);	
		}
	}

	long long GET_TIMESTAMP() {
		return ++TIMESTAMP;
	}

	//returns best ratio edge
	pair<PITEMS_EDGE, TRATIO> BEST_RATIO_EDGE(TCOST K) {
		PITEMS_EDGE itemsEdge = NULL;
		TRATIO itemsRatio = 0;

		if(K < MIN_OUTFIT_COST - 1e-9) return {itemsEdge, itemsRatio};

		while(!EdgeQueue.empty()) {
			auto it = EdgeQueue.top();

			TRATIO ratio = it.first.first;
			long long edgeHash = it.first.second;
			long long timestamp = it.second;
			TCOST totalCost = CostOfItems[AllEdges[edgeHash]->item1->itemHash] + CostOfItems[AllEdges[edgeHash]->item2->itemHash];

			if(timestamp < RecentTimestampEdge[edgeHash]) {
				//Old Edge
				EdgeQueue.pop();
				continue;
			}

			assert(timestamp == RecentTimestampEdge[edgeHash]);

			if(totalCost > K) {
				//Not good edge
				EdgeQueue.pop();
				continue;
			}

			if(IsEdgeSelected[edgeHash]) {
				//already considered
				EdgeQueue.pop();
				continue;
			}

			itemsEdge = AllEdges[edgeHash];
			itemsRatio = ratio;
			break;
		}

		return {itemsEdge, itemsRatio};
	}

	//returns best ratio Z
	pair<PITEM_NODE, TRATIO> BEST_RATIO_Z(TCOST K) {
		PITEM_NODE itemNode = NULL;
		TRATIO itemRatio = 0.0;

		if(K < MIN_ITEM_COST - 1e-9) return {itemNode, itemRatio};

		while(!ItemQueue.empty()) {
			auto it = ItemQueue.top();

			TRATIO ratio = it.first.first;
			long long itemHash = it.first.second;
			long long timestamp = it.second;
			TCOST totalCost = CostOfItems[itemHash];

			if(timestamp < RecentTimestampItem[itemHash]) {
				//Old Edge
				ItemQueue.pop();
				continue;
			}

			assert(timestamp == RecentTimestampItem[itemHash]);

			if(totalCost > K - 1e-9) {
				//Not good edge
				ItemQueue.pop();
				continue;
			}

			if(IsItemSelected[itemHash]) {
				//already considered
				ItemQueue.pop();
				continue;
			}

			itemNode = AllItems[itemHash];
			itemRatio = ratio;

			break;
		}

		return {itemNode, itemRatio};
	} 

	void REMOVE_ITEM(PITEM_NODE itemNode) {
		long long itemHash = itemNode->itemHash;

		vector <long long> UpdNodes;

		for(long long e : AdjList[itemHash]) {
			assert(!IsEdgeSelected[e]);

			IsEdgeSelected[e] = true;
			PITEM_NODE otherNode = GET_OTHER_ITEM(AllEdges[e], itemNode);
			long long otherHash = otherNode->itemHash;

			assert(!IsItemSelected[otherHash]);

			assert(AdjList[otherHash].count(e));

			AdjList[otherHash].erase(AdjList[otherHash].find(e));
			ProfitOfItems[otherHash] += ProfitOfEdges[e];

			UpdNodes.push_back(otherHash);
		}	

		for(long long i : UpdNodes) {
			RECREATE_ITEM(i);
		}

		IsItemSelected[itemHash] = true;
		AdjList[itemHash].clear();

		return;
	}

	void RECREATE_ITEM(long long itemHash) {
		for(long long edgeHash : AdjList[itemHash]) {
			assert(!IsEdgeSelected[edgeHash]);
			
			long long otherHash = GET_OTHER_ITEM(AllEdges[edgeHash], AllItems[itemHash])->itemHash;

			assert(!IsItemSelected[itemHash]);
			
			TPROFIT profit = ProfitOfItems[itemHash] + ProfitOfItems[otherHash] + ProfitOfEdges[edgeHash];
			TCOST cost = CostOfItems[itemHash] + CostOfItems[otherHash];
			TRATIO ratio = profit / cost;

			long long nxtTimestamp = GET_TIMESTAMP();
			RecentTimestampEdge[edgeHash] = nxtTimestamp;

			EdgeQueue.push({{ratio, edgeHash}, nxtTimestamp});
		}

		long long nxtTimestamp = GET_TIMESTAMP();
		TPROFIT profit = ProfitOfItems[itemHash];
		TCOST cost = CostOfItems[itemHash];

		RecentTimestampItem[itemHash] = nxtTimestamp;
		TRATIO ratio = profit / cost;
		ItemQueue.push({{ratio, itemHash}, nxtTimestamp});

		return;
	}

	void REMOVE_EDGE(PITEMS_EDGE itemEdge) {
		long long edgeHash = itemEdge->edgeHash;
		long long toHash = itemEdge->item1->itemHash;
		long long fromHash = itemEdge->item2->itemHash;

		assert(!IsEdgeSelected[itemEdge->edgeHash]);

		IsEdgeSelected[edgeHash] = true;

		assert(AdjList[toHash].count(edgeHash));
		assert(AdjList[fromHash].count(edgeHash));

		AdjList[toHash].erase(AdjList[toHash].find(edgeHash));
		AdjList[fromHash].erase(AdjList[fromHash].find(edgeHash));


		REMOVE_ITEM(AllItems[toHash]);
		REMOVE_ITEM(AllItems[fromHash]);
	}

	//returns best ratio things
	RETURN_SET
	BEST_RATIO(TCOST budget) {
		RETURN_SET returnSet;
		RETURN_SET_INIT(&returnSet);

		pair<PITEMS_EDGE, TRATIO> edge = BEST_RATIO_EDGE(budget);
		pair<PITEM_NODE, TRATIO> node = BEST_RATIO_Z(budget);

		if(edge.first == NULL && node.first == NULL) {
			return returnSet;
		}

		if(edge.first == NULL || (node.first != NULL && edge.second <= node.second)) {
			ADD_ITEM_TO_RETURN_SET(&returnSet, node.first, ProfitOfItems[node.first->itemHash]);
			REMOVE_ITEM(node.first);
			return returnSet;
		}

		if(node.first == NULL || (edge.first != NULL && node.second <= edge.second)) {
			ADD_EDGE_TO_RETURN_SET(&returnSet, edge.first, ProfitOfItems[edge.first->item1->itemHash] + 
			ProfitOfItems[edge.first->item2->itemHash] + ProfitOfEdges[edge.first->edgeHash]);
			REMOVE_EDGE(edge.first);
			return returnSet;	
		}

		assert(1 == 0);

		return returnSet;
	} 

	//returns best pseudo-profit edge
	RETURN_SET
	BEST_PSEUDO_PROFIT_EDGE(TCOST budget) {
		RETURN_SET returnSet;
		RETURN_SET_INIT(&returnSet);

		for(int e = 0; e < EDGE_COUNTER; e++) {
			RETURN_SET curReturnSet;
			RETURN_SET_INIT(&curReturnSet);

			long long u = AllEdges[e]->item1->itemHash;
			long long v = AllEdges[e]->item2->itemHash;

			if(budget < AllItems[u]->cost + AllItems[v]->cost) {
				continue;
			}

			ADD_EDGE_TO_RETURN_SET(&curReturnSet, AllEdges[e], BETA * (AllItems[u]->profit + AllItems[v]->profit)
				+ ALPHA * AllEdges[e]->profit);

			long long ptr1 = -1, ptr2 = -1;

			TCOST remBudget = budget - AllItems[u]->cost - AllItems[v]->cost;

			while(ptr1 + 1 < (long long)PseudoAdjList[u].size() && ptr2 + 1 < (long long)PseudoAdjList[v].size()) {
				if(PseudoAdjList[u][ptr1 + 1].first.second == v) {
					ptr1++;
					continue;
				}

				if(PseudoAdjList[v][ptr2 + 1].first.second == u) {
					ptr2++;
					continue;
				}

				if(remBudget < min(PseudoAdjList[u][ptr1 + 1].first.first, PseudoAdjList[v][ptr2 + 1].first.first)) {
					break;
				}

				if(PseudoAdjList[u][ptr1 + 1].first.first < PseudoAdjList[v][ptr2 + 1].first.first) {
					TCOST curCost = PseudoAdjList[u][ptr1 + 1].first.first;
					long long curItemHash = PseudoAdjList[u][ptr1 + 1].first.second;
					long long curEdgeHash = PseudoAdjList[u][ptr1 + 1].second;
					remBudget -= curCost;
					ADD_ITEM_TO_RETURN_SET(&curReturnSet, AllItems[curItemHash], ALPHA * AllEdges[curEdgeHash]->profit);
					ptr1++;
					continue;
				}
				else {
					TCOST curCost = PseudoAdjList[v][ptr2 + 1].first.first;
					long long curItemHash = PseudoAdjList[v][ptr2 + 1].first.second;
					long long curEdgeHash = PseudoAdjList[v][ptr2 + 1].second;
					remBudget -= curCost;
					ADD_ITEM_TO_RETURN_SET(&curReturnSet, AllItems[curItemHash], ALPHA * AllEdges[curEdgeHash]->profit);
					ptr2++;
					continue;
				}
			}

			while(ptr1 + 1 < (long long)PseudoAdjList[u].size()) {
				if(PseudoAdjList[u][ptr1 + 1].first.second == v) {
					ptr1++;
					continue;
				}

				if(remBudget < PseudoAdjList[u][ptr1 + 1].first.first) {
					break;
				}

				TCOST curCost = PseudoAdjList[u][ptr1 + 1].first.first;
				long long curItemHash = PseudoAdjList[u][ptr1 + 1].first.second;
				long long curEdgeHash = PseudoAdjList[u][ptr1 + 1].second;
				remBudget -= curCost;
				ADD_ITEM_TO_RETURN_SET(&curReturnSet, AllItems[curItemHash], ALPHA * AllEdges[curEdgeHash]->profit);
				ptr1++;
			}

			while(ptr2 + 1 < (long long)PseudoAdjList[v].size()) {
				if(PseudoAdjList[v][ptr2 + 1].first.second == u) {
					ptr2++;
					continue;
				}

				if(remBudget < PseudoAdjList[v][ptr2 + 1].first.first) {
					break;
				}

				TCOST curCost = PseudoAdjList[v][ptr2 + 1].first.first;
				long long curItemHash = PseudoAdjList[v][ptr2 + 1].first.second;
				long long curEdgeHash = PseudoAdjList[v][ptr2 + 1].second;
				remBudget -= curCost;
				ADD_ITEM_TO_RETURN_SET(&curReturnSet, AllItems[curItemHash], ALPHA * AllEdges[curEdgeHash]->profit);
				ptr2++;
			}

			if(abs(returnSet.totalProfit - curReturnSet.totalProfit) <= 1e-9){
				if(abs(returnSet.totalCost - curReturnSet.totalCost) <= 1e-9)
					continue;

				if(returnSet.totalCost > curReturnSet.totalCost)
					returnSet = curReturnSet;
			}		
			else if(returnSet.totalProfit < curReturnSet.totalProfit) {
				returnSet = curReturnSet;
			}
		}

		UPDATE_PSEUDO_PROFIT_RETURN_SET(&returnSet);

		return returnSet;
	}

	//returns best pseudo-profit edge with probability on edges
	RETURN_SET
	BEST_PSEUDO_PROFIT_EDGE_PROB(TCOST budget) {
		RETURN_SET returnSet;
		RETURN_SET_INIT(&returnSet);

		for(int e = 0; e < EDGE_COUNTER; e++) {
			RETURN_SET curReturnSet;
			RETURN_SET_INIT(&curReturnSet);

			long long u = AllEdges[e]->item1->itemHash;
			long long v = AllEdges[e]->item2->itemHash;

			if(budget < AllItems[u]->cost + AllItems[v]->cost) {
				continue;
			}

			ADD_EDGE_TO_RETURN_SET(&curReturnSet, AllEdges[e], BETA * (AllItems[u]->profit + AllItems[v]->profit)
				+ ALPHA * AllEdges[e]->profit);


			TCOST remBudget = budget - AllItems[u]->cost - AllItems[v]->cost;

			/**START: MAIN KNAPSACK LOGIC**/

			//Knaspack Items with {cost, edgeHash}
			vector < pair<TCOST, long long> > kpItems;
			
			long long MAXI = 0;
			TPROFIT MULTIPLIER = 10.0;
			int CNT = 0;

			for(int i = 0; i < (int)PseudoAdjList[u].size(); i++) {
				if(PseudoAdjList[u][i].first.second == v) continue;
				
				long long edgeHash = PseudoAdjList[u][i].second;
				TCOST cost = PseudoAdjList[u][i].first.first;
				kpItems.push_back({cost, edgeHash});

				TPROFIT profit = AllEdges[edgeHash]->profit;
				MAXI += (long long)(profit * MULTIPLIER + 1e-9);
				CNT++;
			}

			for(int i = 0; i < (int)PseudoAdjList[v].size(); i++) {
				if(PseudoAdjList[v][i].first.second == u) continue;
				
				long long edgeHash = PseudoAdjList[v][i].second;
				TCOST cost = PseudoAdjList[v][i].first.first;
				kpItems.push_back({cost, edgeHash});

				TPROFIT profit = AllEdges[edgeHash]->profit;
				MAXI += (long long)(profit * MULTIPLIER + 1e-9);
				CNT++;
			}			

			TCOST dp[CNT + 1][MAXI + 1];
			int last[CNT + 1][MAXI + 1];
			
			int NOT_ASSIGNED = -1, ZERO = -2, UP = -3;

			memset(last, -1, sizeof(last));

			dp[0][0] = 0;
			last[0][0] = ZERO;

			for(int i = 1; i <= CNT; i++) {
				long long edgeHash = kpItems[i - 1].second;
				long long profit = (long long)(AllEdges[edgeHash]->profit * MULTIPLIER + 1e-9);
				TCOST cost = kpItems[i - 1].first;

				for(int j = 0; j <= MAXI; j++) {
					dp[i][j] = dp[i - 1][j];
					if(last[i - 1][j] != NOT_ASSIGNED) {
						last[i][j] = UP;
					}
				}

				for(int j = 0; j <= MAXI; j++) {
					if(j >= profit) {
						int k = j - profit;
						if(last[i - 1][k] != NOT_ASSIGNED) {
							if(last[i][j] == NOT_ASSIGNED) {
								dp[i][j] = dp[i - 1][k] + cost;
								last[i][j] = i - 1;
							}
							else {
								if(dp[i][j] > dp[i - 1][k] + cost) {
									dp[i][j] = dp[i - 1][k] + cost;
									last[i][j] = i - 1;
								}
							}
						}
					}
				}
			}

			vector <int> selectedItems;

			for(int i = MAXI; i >= 0; i--) {
				if(last[CNT][i] != NOT_ASSIGNED && dp[CNT][i] <= remBudget + 1e-9) {
					int curProfit = i;
					for(int j = CNT; j >= 0; j--) {
						assert(curProfit >= 0 && curProfit <= MAXI);
						assert(last[j][curProfit] != NOT_ASSIGNED);
						if(last[j][curProfit] == ZERO) {
							assert(!j);
							continue;
						}

						if(last[j][curProfit] == UP) continue;
						
						assert(last[j][curProfit] >= 0);

						int x = last[j][curProfit];
						long long edgeHash = kpItems[x].second;
						long long profit = (long long)(AllEdges[edgeHash]->profit * MULTIPLIER + 1e-9);
						
						selectedItems.push_back(x);
						curProfit -= profit;

						assert(curProfit >= 0 && curProfit <= MAXI);	
					}
					assert(!curProfit);
					break;
				}
			}

			for(int i : selectedItems) {
				long long edgeHash = kpItems[i].second;
				long long itemHash1 = AllEdges[edgeHash]->item1->itemHash;
				long long itemHash2 = AllEdges[edgeHash]->item2->itemHash;
				
				if(itemHash1 == u || itemHash2 == u) {
					ADD_ITEM_TO_RETURN_SET(&curReturnSet, GET_OTHER_ITEM(AllEdges[edgeHash], AllItems[u]), ALPHA * AllEdges[edgeHash]->profit);	
				}
				else if(itemHash1 == v || itemHash2 == v) {
					ADD_ITEM_TO_RETURN_SET(&curReturnSet, GET_OTHER_ITEM(AllEdges[edgeHash], AllItems[v]), ALPHA * AllEdges[edgeHash]->profit);	
				}
				else {
					assert(1 == 0);
				}
			}

			if(abs(returnSet.totalProfit - curReturnSet.totalProfit) <= 1e-9){
				if(abs(returnSet.totalCost - curReturnSet.totalCost) <= 1e-9)
					continue;

				if(returnSet.totalCost > curReturnSet.totalCost)
					returnSet = curReturnSet;
			}		
			else if(returnSet.totalProfit < curReturnSet.totalProfit) {
				returnSet = curReturnSet;
			}

		}

		UPDATE_PSEUDO_PROFIT_RETURN_SET(&returnSet);

		return returnSet;
	}

	//Find the new complete profit of the Pseduo set
	void
	UPDATE_PSEUDO_PROFIT_RETURN_SET (PRETURN_SET pseudoReturnSet)
	{
		TPROFIT totalProfit = 0.0;
		TCOST totalCost = 0.0;

		vector<long long> top, bottom;

		for(PITEM_NODE item : pseudoReturnSet->returnItems) {
			if(item->type == TopWear) top.push_back(item->itemHash);
			else bottom.push_back(item->itemHash);

			totalCost += item->cost;
			totalProfit += BETA * item->profit;
		}

		for(long long u : top) {
			for(long long v : bottom) {
				if(hashEdges.find({u, v}) != hashEdges.end()) {
					long long edgeHash = hashEdges[{u, v}];
					totalProfit += ALPHA * AllEdges[edgeHash]->profit;
				}
			}
		}

		pseudoReturnSet->totalProfit = totalProfit;
		pseudoReturnSet->totalCost = totalCost;

		return;
	}
};

#endif //MOBCCWR_H