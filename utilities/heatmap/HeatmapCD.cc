#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "../../src/Condensation.h"

using namespace std;

/*class Heatmap {
	int size;
	double **m;

	public:
	Heatmap () {}
	Heatmap (int _s) : size(_s) {
		m = new double*[size];
		for (int i=0 ; i<size ; i++)
			m[i] = new double[size];
	}
	double*& operator[](const int& i) {
		return m[i];
	}
};*/

void ComputeCD (Dataset &D, Subset &R, int size) {

	NearestEnemy NE(D);
	NE.use(R);
	Point p(2);

	FILE *outf = fopen ("data/tmp.csv","w");

	fprintf(outf, "x,y,cd\n");
	for (int i=0 ; i<size ; i++) {
		p[0] = ((double) 2*i + 1) / ((double) 2*size);
		for (int j=0 ; j<size ; j++) {
			p[1] = ((double) 2*j + 1) / ((double) 2*size);
			fprintf(outf, "%d,%d,%.4f\n", i+1, j+1, NE.chrom_density(p));
		}
	}

	fclose(outf);
}

int main(int argc, char* argv[]) {

	Dataset TR, TS;
	ReadFile(string(argv[3]),atoi(argv[4]),TR,TS);
	
	string alg(argv[2]);
	if (!alg.compare("CNN")) {
		//srand(time(NULL));
		srand(43475);
		TR.shuffle();
	}

	Subset R(TR);
	     if (!alg.compare("CNN"))      R = CNN(TR);
	else if (!alg.compare("RNN"))      R = RNN(TR);
	else if (!alg.compare("FCNN"))     R = FCNN(TR);
	else if (!alg.compare("NET"))      R = NET(TR);
	else if (!alg.compare("MSS"))      R = MSS(TR);
	else if (!alg.compare("AlphaMSS")) R = AlphaMSS(TR,atof(argv[5]));
	
	ComputeCD(TR,R,atoi(argv[1]));

	system("Rscript PlotCD.R");
	char cmd[50];
	sprintf(cmd, "mv data/tmp.csv data/%s%s.csv", alg.c_str(), argc==6 ? argv[5] : "");
	system(cmd);
	sprintf(cmd, "mv img/tmp.png img/%s%s.png", alg.c_str(), argc==6 ? argv[5] : "");
	system(cmd);

	return 0;
}
