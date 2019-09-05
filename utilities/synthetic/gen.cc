#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "../../src/Condensation.h"

using namespace std;

int main(int argc, char* argv[]) {

	srand(time(NULL));

	int n, d, c, g;
	n = atoi(argv[1]);
	d = atoi(argv[2]);
	c = atoi(argv[3]);
	g = atoi(argv[4]);

	char fname[100];
	sprintf(fname, "../../dataset/synthetic/v-%d-%d-%d-%d.txt", n, d, c, g);
	FILE *fp = fopen(fname, "w");

	Dataset S(g,d,c,true);
	for (int i=0 ; i<g ; i++)
		S[i].c = i % c;

	NearestNeighbor NN(S);
	NN.all();
	
	Dataset P(n,d,c,true);

	fprintf(fp, "%d %d %d\n", n, d, c);
	int v = n / 10,
		m = n % 10;
	for (int i=0 ; i<10 ; i++)
		fprintf(fp, "%d ", v + (i<m ? 1 : 0));
	fprintf(fp, "\n");
	for (int i=0 ; i<n ; i++) {
		P[i].c = S[NN.of(P[i])].c;
		fprintf(fp, "%d ", i % 10);
		P[i].print(fp);
	}

	fclose(fp);

	return 0;
}
