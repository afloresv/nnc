#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <sys/time.h>
#include <ctime>
#include "Dataset.h"
#include "Subset.h"

using namespace std;

#ifndef IO_NNC_H
#define IO_NNC_H

clock_t algtime;

void ReadFile (string path, int tfcv, Dataset &TR, Dataset &TS) {

	FILE *src;
	src = fopen(path.c_str(),"r");
	if (src==NULL) {
		cerr << "Error opening file " << path << endl;
		exit(1);
	}
	
	int n,ntr,nts,m,c,temp;
	fscanf(src,"%d%d%d",&n,&m,&c);

	ntr = nts = 0;
	for (int i=0 ; i<10 ; i++) {
		fscanf(src,"%d",&temp);
		if (i == tfcv) nts += temp;
		else ntr += temp;
	}

	TR = Dataset(ntr,m,c);
	TS = Dataset(nts,m,c);

	Point _p, &p = _p;
	int *clss;
	ntr = nts = 0;
	for (int i=0 ; i<n ; i++) {
		fscanf(src,"%d",&temp);
		if (temp == tfcv) {
			TS[nts].ind = i;
			clss = &(TS[nts].c);
			p = TS[nts++];
		} else {
			TR[ntr].ind = i;
			clss = &(TR[ntr].c);
			p = TR[ntr++];
		}
		for (int j=0 ; j<m ; j++)
			fscanf(src,"%lf",&p[j]);
		fscanf(src,"%d",clss);
	}

	fclose(src);

	algtime = clock();
}

void PrintResult (Dataset &TR, Dataset &TS, Subset &R) {

	if (TS.size()) {

		double elapsed_secs = double(clock() - algtime) / CLOCKS_PER_SEC;

		NearestNeighbor NN(TR);
		NN.use(R);

		printf("%.4lf\t", 100.0*(double)R.size()/TR.size());	// Size
		printf("%.4lf\t", 100.0*NN.wrong(TS)/TS.size());		// Error

		NearestEnemy NE(TR);
		NE.use(R);
		double cd_total;
		vector<double> cd;
		for (int i=0 ; i<TS.size() ; i++) {
			cd.push_back(NE.chrom_density(TS[i]));
			cd_total += cd[cd.size()-1];
		}

		sort(cd.begin(),cd.end());
		printf("%.4lf\t", cd[0]);								// Min CD
		printf("%.4lf\t", cd[cd.size()/4]);						// 1/4 CD
		printf("%.4lf\t", cd[cd.size()/2]);						// Median CD
		printf("%.4lf\t", cd[3*cd.size()/4]);					// 3/4 CD
		printf("%.4lf\t", cd[cd.size()-1]);						// Max CD
		printf("%.4lf\t", cd_total/cd.size());					// Average CD
		printf("%.4lf\n", elapsed_secs);						// Time

	} else {
		R.print();
	}
}

#endif
