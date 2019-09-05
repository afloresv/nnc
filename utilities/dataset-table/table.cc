#include <iostream>
#include <cstdio>
#include <vector>
#include <set>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "../../src/Condensation.h"

using namespace std;

int numne(Dataset &D) {
	NearestEnemy NE(D);
	NE.all();

	Subset R(D);
	for (int i=0 ; i<D.size() ; i++)
		R.set(NE.of(i));
	
	return R.size();
}

int main(int argc, char* argv[]) {

	Dataset TR, TS;
	ReadFile(string(argv[1]),-1,TR,TS);
	
	printf(" & %d & %d & %d & %d \\\\\n", TR.size(), TR.dim(), TR.clss(), numne(TR));

	return 0;
}
