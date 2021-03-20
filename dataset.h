#ifndef DATASET_H
#define DATASET_H

#include <bits/stdc++.h>
#include "mobccwr.h"
using namespace std;

//TO-DO: dataset related configuration

typedef struct {

	TPARAM ALPHA, BETA;
	TCOST BUDGET;
	long long numTop, numBottom, numCompOutfits;

	RETURN_SET returnSetRatio;
	RETURN_SET returnSetPseudo;
	RETURN_SET returnSetAns;

	double executionTimeRatio;
	double executionTimePseudo;
	double executionTimeTotal;

	long long numIterations;

	TPROFIT PS;
	TPROFIT CS;
	TPROFIT VS;

} RESULT, *PRESULT;

int NUM_OUTFITS = 0;

void PRINT_RESULT(PRESULT result, ofstream &out) {
	out << "Total Profit: " << result->returnSetAns.totalProfit << "\n";
	out << "Total Cost: " << result->returnSetAns.totalCost << "\n";

	out << "All Items:\n";
	for(PITEM_NODE itemNode : result->returnSetAns.returnItems) {
		out << itemNode->info << " ";
	}
	out << "\n";
}

void PRINT_RESULT_ROW(PRESULT result, ofstream &out) {
	out << result->BUDGET << "\t" << result->ALPHA << "\t" << result->BETA << "\t";
	out << result->returnSetAns.totalProfit << "\t" << fixed << setprecision(2) << result->executionTimeTotal * 1000 << "\t";
	out << result->numTop << "\t" << result->numBottom << "\t" << result->numCompOutfits << "\t";
	out << result->returnSetRatio.totalProfit << "\t" << result->returnSetPseudo.totalProfit << "\n";
}


void PRINT_RESULT_ROW_BASELINE(PRESULT result, ofstream &out) {
	out << result->BUDGET << "\t" << result->ALPHA << "\t" << result->BETA << "\t";
	out << result->returnSetAns.totalProfit << "\t" << fixed << setprecision(2);
	out << result->numTop << "\t" << result->numBottom << "\t" << result->numCompOutfits << "\n";
}

void PRINT_RESULT_ROW_PSCSVS(PRESULT result, ofstream &out) {
	out << result->BUDGET << "\t" << result->ALPHA << "\t" << result->BETA << "\t";
	out << fixed << setprecision(2) << result->PS << "\t" << result->CS << "\t" << result->VS << "\n";
}

void PRINT_RESULT_RUNTIME(PRESULT result, ofstream &out) {
	out << result->BUDGET << "\t" << fixed << setprecision(2) << result->executionTimeTotal * 1000 << "\n";
}

void ANALYZE_DATASET(string path, ofstream &out) {
	ifstream in(path);

	long long BLK = 100;

	int numTop = 0, numBottom = 0;
	int numItems, numOutfits;
	int numItems2 = 0, numOutfits2 = NUM_OUTFITS;
	
	in >> numItems >> numOutfits;

	map <long long, long long> distTop, distBottom;
	map <long long, TCOST> meanPriceTop, meanPriceBottom;
	map <long long, TPROFIT> meanVersTop, meanVersBottom;
	map <long long, TCOST> stdDevPriceTop, stdDevPriceBottom;
	map <long long, TPROFIT> stdDevVersTop, stdDevVersBottom;

	for(int i = 0; i < numItems; i++) {
		string info;
		long long type;
		TPROFIT profit;
		TCOST cost;

		in >> info >> type >> profit >> cost;

		if(cost >= 2500 - 1e-6) continue;

		numItems2++;

		if(!type) {
			numTop++;
			long long f = (long long)(cost / BLK + 1e-9);
			f = min(f, 24LL);
			distTop[f]++;
			meanPriceTop[f] += cost;
			meanVersTop[f] += profit;
		}
		else {
			numBottom++;
			long long f = (long long)(cost / BLK + 1e-9);
			f = min(f, 24LL);
			distBottom[f]++;	
			meanPriceBottom[f] += cost;
			meanVersBottom[f] += profit;
		}
	}

	for(auto it : distTop) {
		meanPriceTop[it.first] /= (1.0 * it.second);
		meanVersTop[it.first] /= (1.0 * it.second);
	}

	for(auto it : distBottom) {
		meanPriceBottom[it.first] /= (1.0 * it.second);
		meanVersBottom[it.first] /= (1.0 * it.second);
	}

	ifstream in2(path);
	in2 >> numItems >> numOutfits;

	for(int i = 0; i < numItems; i++) {
		string info;
		long long type;
		TPROFIT profit;
		TCOST cost;

		in2 >> info >> type >> profit >> cost;

		if(cost >= 2500 - 1e-6) continue;

		if(!type) {
			long long f = (long long)(cost / BLK + 1e-9);
			f = min(f, 24LL);
			stdDevPriceTop[f] += (cost - meanPriceTop[f]) * (cost - meanPriceTop[f]);
			stdDevVersTop[f] += (profit - meanVersTop[f]) * (profit - meanVersTop[f]);
		}
		else {
			long long f = (long long)(cost / BLK + 1e-9);
			f = min(f, 24LL);
			stdDevPriceBottom[f] += (cost - meanPriceBottom[f]) * (cost - meanPriceBottom[f]);
			stdDevVersBottom[f] += (profit - meanVersBottom[f]) * (profit - meanVersBottom[f]);
		}
	}

	for(auto it : distTop) {
		stdDevPriceTop[it.first] /= (1.0 * it.second);
		stdDevPriceTop[it.first] = sqrt(stdDevPriceTop[it.first]);

		stdDevVersTop[it.first] /= (1.0 * it.second);
		stdDevVersTop[it.first] = sqrt(stdDevVersTop[it.first]);
	}

	for(auto it : distBottom) {
		stdDevPriceBottom[it.first] /= (1.0 * it.second);
		stdDevPriceBottom[it.first] = sqrt(stdDevPriceBottom[it.first]);

		stdDevVersBottom[it.first] /= (1.0 * it.second);
		stdDevVersBottom[it.first] = sqrt(stdDevVersBottom[it.first]);
	}	

	assert(numTop + numBottom == numItems2);

	out << "Data Analysis\n";
	out << "Note: Prices are in USD.\n";
	out << "Total Items: " << numItems2 << "\nTotal Outfits: " << numOutfits2 << "\nTopwears: " << numTop << ", Bottomwears: " << numBottom << "\n\n\n";

	out << "Distribution of Data\n\n";
	out << "TOPWEARS:\n\n";
	out << "Price Range" << "\t" << "No. Of Items" << "\t" << "Perc. Of Items\t" << "Mean Price\t" << "Std. Dev. Price\t" << "Mean Vers\t" << "Std. Dev. Vers\n";
	for(auto it : distTop) {
		double per = 100.0 * it.second / numTop;
		long long f = it.first;
		if(f > 100) continue;
		out << f * BLK << "-" << (f + 1) * BLK - 1 << "\t"; 
		out << it.second << "\t" << fixed << setprecision(2) << per << "\t" << meanPriceTop[f] << "\t" << stdDevPriceTop[f] << "\t" << meanVersTop[f] << "\t" << stdDevVersTop[f] << "\n";
	}

	out << "\nBOTTOMWEARS:\n\n";
	out << "Price Range" << "\t" << "No. Of Items" << "\t" << "Perc. Of Items\t" << "Mean Price\t" << "Std. Dev. Price\t" << "Mean Vers\t" << "Std. Dev. Vers\n";
	for(auto it : distBottom) {
		double per = 100.0 * it.second / numBottom;
		long long f = it.first;
		if(f > 100) continue;
		out << f * BLK << "-" << (f + 1) * BLK - 1 << "\t";
		out << it.second << "\t" << fixed << setprecision(2) << per << "\t" << meanPriceBottom[f] << "\t" << stdDevPriceBottom[f] << "\t" << meanVersBottom[f] << "\t" << stdDevVersBottom[f] << "\n";
	}

	out << "\n\n";
}

#endif //DATASET_H