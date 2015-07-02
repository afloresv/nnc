#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <climits>
#include <algorithm>
#include "Dataset.cc"
#include "Subset.cc"
#include "KDtree.cc"
#include "NearestNeighbor.cc"
#include "IO.cc"

using namespace std;

Subset FCNN (Dataset &D) {

	Subset R(D);
	vector<int> S = D.centroids(),
		nn(D.size(),-1),
		rep(D.size(),-1);
	vector<double> nnd(D.size(),numeric_limits<double>::max());

	while (S.size() > 0) {
		for (int i=0 ; i<S.size() ; i++) R.set(S[i]);
		for (int i=0 ; i<D.size() ; i++)
			if (R[i]) rep[i] = -1;
		for (int i=0 ; i<D.size() ; i++) {
			if (R[i]) continue;
			for (int j=0 ; j<S.size() ; j++) {
				double d = D.distance(i,S[j]);
				if (d < nnd[i]) {
					nn[i] = S[j];
					nnd[i] = d;
				}
			}
			if (D[i].c != D[nn[i]].c && (rep[nn[i]]==-1
				|| nnd[i] < D.distance(nn[i],rep[nn[i]])))
				rep[nn[i]] = i;
		}
		S.clear();
		for (int i=0 ; i<D.size() ; i++)
			if (R[i] && rep[i]!=-1)
				S.push_back(rep[i]);
	}
	return R;
}

int main(int argc, char* argv[]) {

	Dataset TR, TS;
	ReadFile(string(argv[1]),atoi(argv[2]),TR,TS);
	srand(time(NULL));
	Subset R(TR);

	R = FCNN(TR);

	PrintResult(TR,TS,R);

	return 0;
}
