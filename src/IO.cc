#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <sys/time.h>
#include <time.h>

using namespace std;

static struct timeval tm1;

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

	gettimeofday(&tm1, NULL);
}

void PrintResult (Dataset &TR, Dataset &TS, Subset &R) {

	struct timeval tm2;
	gettimeofday(&tm2, NULL);
	unsigned long long TimeEnlapsed = 1000*(tm2.tv_sec-tm1.tv_sec)+(tm2.tv_usec-tm1.tv_usec)/1000;

	NearestNeighbor NN(TR);
	NN.use(R);

	//printf("------------------------\n");
	//printf("Size             %6.2lf%%\n", 100.0*(double)R.size()/TR.size());
	//printf("Training Error   %6.2lf%%\n", 100.0*NN.wrong()/TR.size());
	//if (TS.size()>0) {
	//	printf("Test Error 1-NN  %6.2lf%%\n", 100.0*NN.wrong(TS)/TS.size());
	//	printf("Test Error 2-NN  %6.2lf%%\n", 100.0*NN.wrong(TS,2)/TS.size());
	//	printf("Test Error 3-NN  %6.2lf%%\n", 100.0*NN.wrong(TS,3)/TS.size());
	//	printf("Test Error 4-NN  %6.2lf%%\n", 100.0*NN.wrong(TS,4)/TS.size());
	//	printf("Test Error 5-NN  %6.2lf%%\n", 100.0*NN.wrong(TS,5)/TS.size());
	//}
	//printf("Time (ms)  %13llu\n", TimeEnlapsed);
	//printf("Distances  %13llu\n", CountDistances);
	//printf("------------------------\n");

	//R.print();

	printf("%.2lf\t", 100.0*(double)R.size()/TR.size());
	printf("%.2lf\t", 100.0*NN.wrong()/TR.size());
	if (TS.size()>0) {
		printf("%.2lf\t", 100.0*NN.wrong(TS)/TS.size());
		for (int i=3 ; i<=15 ; i+=2)
			printf("%.2lf\t", 100.0*NN.wrong(TS,(i>R.size() ? R.size() : i))/TS.size());
	}
	printf("%llu\t", TimeEnlapsed);
	printf("%llu\n", CountDistances);

	//Point q;
	//unsigned long long NumNNS=0, initCD=CountDistances;
	//clock_t ts = clock();
	//do {
	//	q = Point(TR.dim(),true);
	//	NN.of(q);
	//	NumNNS++;
	//} while (clock()-ts <= CLOCKS_PER_SEC);
	//printf("%llu\t", NumNNS);
	//printf("%llu\n", CountDistances-initCD);
}
