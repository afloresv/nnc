#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "Dataset.cc"
#include "Subset.cc"
#include "KDtree.cc"
#include "NearestNeighbor.cc"
#include "NearestEnemy.cc"
#include "IO.cc"

using namespace std;

Subset RSS (Dataset &D) {

	// Prepare
	NearestEnemy NE(D);
	NE.all();
	vector<pair<double,int> > neo = NE.order();

	// Algorithm
	Subset R(D);
	vector<int> sel;
	bool add;
	for (int i=0 ; i<D.size() ; i++) {
		add = true;
		for (int j=sel.size()-1 ; add && j>=0 ; j--)
			if (D[neo[i].second].c == D[neo[sel[j]].second].c &&
				D.distance(neo[i].second,neo[sel[j]].second) < neo[sel[j]].first)
				add = false;
		if (add) {
			sel.push_back(i);
			R.set(neo[i].second);
		}
	}

	return R;
}

int main(int argc, char* argv[]) {

	Dataset TR, TS;
	ReadFile(string(argv[1]),atoi(argv[2]),TR,TS);
	srand(time(NULL));
	Subset R(TR);

	R = RSS(TR);

	PrintResult(TR,TS,R);

	return 0;
}
