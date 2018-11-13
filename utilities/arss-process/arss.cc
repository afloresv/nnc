#include <iostream>
#include <cstdio>
#include <vector>
#include "../../src/Dataset.h"
#include "../../src/Subset.h"
#include "../../src/KDtree.h"
#include "../../src/NearestNeighbor.h"
#include "../../src/NearestEnemy.h"
#include "../../src/IO.h"

using namespace std;

void AlphaRSS (Dataset &D, float alpha) {
	NearestEnemy NE(D);
	NE.all();
	vector<pair<double,int> > neo = NE.order();
	vector<int> sel;
	printf("ind,dne\n");
	bool add;
	for (int i=0 ; i<D.size() ; i++) {
		add = true;
		for (int j=sel.size()-1 ; add && j>=0 ; j--)
			if (D[neo[i].second].c == D[neo[sel[j]].second].c &&
				D.distance(neo[i].second,neo[sel[j]].second) * (1.0f + alpha)
					< neo[i].first)
				add = false;
		if (add) {
			sel.push_back(i);
			printf("%d,%f\n", neo[i].second, neo[i].first);
		} else if (sel.size() == 81) {
			fprintf(stderr, "%d,%f\n", neo[i].second, neo[i].first);
		}
	}
}

int main(int argc, char* argv[]) {

	Dataset TR, TS;
	ReadFile(string(argv[1]),atoi(argv[2]),TR,TS);

	AlphaRSS(TR, atof(argv[3]));

	return 0;
}
