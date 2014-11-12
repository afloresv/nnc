#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>
#include <algorithm>

using namespace std;

class Point {
	double *p;
	public:
	int c, ind;
	Point () {}
	Point (int m) {
		p = new double[m];
	}
	double& operator[](const int& i) {
		return p[i];
	}
};

class Dataset {
	int n, m;
	Point *p;
	double **d;

	public:
	Dataset () {}
	Dataset (int _n, int _m) : n(_n), m(_m) {
		p = new Point[n];
		d = new double*[n];
		for (int i=0 ; i<n ; i++) {
			p[i] = Point(m);
			d[i] = new double[n];
			for (int j=0 ; j<n ; j++)
				d[i][j] = -1.0;
		}
	}
	int size () { return n; }
	int dim ()  { return m; }
	Point& operator[] (const int& i) {
		return p[i];
	}
	double distance (int a, int b) {
		return sqrt(sqdistance(a,b));
	}
	double sqdistance (int a, int b) {
		if (a>b) swap(a,b);
		if (d[a][b]<0.0) d[a][b]=sqdist(a,b);
		return d[a][b];
	}
	double sqdist (int a, int b) {
		double dist = 0.0;
		for (int i=0 ; i<m ; i++)
			dist += pow(p[a][i] - p[b][i],2);
		return dist;
	}
};

void ReadFile (string path, int tfcv, Dataset &TR, Dataset &TS) {

	FILE *src;
	src = fopen(path.c_str(),"r");
	if (src==NULL) {
		cerr << "Error opening file " << path << endl;
		exit(1);
	}
	
	int n,ntr,nts,m,temp;
	fscanf(src,"%d%d",&n,&m);

	ntr = nts = 0;
	for (int i=0 ; i<10 ; i++) {
		fscanf(src,"%d",&temp);
		if (i == tfcv) nts += temp;
		else ntr += temp;
	}

	TR = Dataset(ntr,m);
	TS = Dataset(nts,m);

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
}
