#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "Dataset.cc"
#include "Subset.cc"
#include "KDtree.cc"
#include "NearestNeighbor.cc"
#include "IO.cc"

using namespace std;

Subset CNN (Dataset &D) {

	// Algorithm
	Subset R(D);
	NearestNeighbor NN(D);
	R.set(0);
	NN.use(R);
	while (true) {
		for (int i=0 ; i<D.size() ; i++) {
			if (!NN.classify(i)) {
				R.set(i);
				NN.use(R);
			}
		}
		if (NN.wrong()==0) break;
	}
	return R;
}

int main(int argc, char* argv[]) {

	Dataset TR, TS;
	ReadFile(string(argv[1]),atoi(argv[2]),TR,TS);
	// 43475
	srand(time(NULL));
	TR.shuffle();
	Subset R(TR);

	R = CNN(TR);

	PrintResult(TR,TS,R);

	return 0;
}
