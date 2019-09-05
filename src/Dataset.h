#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <map>
#include <algorithm>
#include <set>
#include <utility>
#include <limits>

using namespace std;

#ifndef DATASET_NNC_H
#define DATASET_NNC_H

class Point {
	double *p;
	public:
	int c, ind, d;
	Point () {}
	Point (int _d, bool r=false) : d(_d) {
		p = new double[d];
		if (r) for (int i=0 ; i<d ; i++)
			p[i] = ((double)rand()) / ((double)RAND_MAX);
	}
	double& operator[](const int& i) {
		return p[i];
	}
	Point operator-(Point& b) {
		Point a(d);
		for (int i=0 ; i<d ; i++)
			a[i] = p[i]-b[i];
		return a;
	}
	double operator*(Point& b) {
		double r = 0.0;
		for (int i=0 ; i<d ; i++)
			r += p[i]*b[i];
		return r;
	}
	void norm () {
		double mag = 0.0;
		for (int i=0 ; i<d ; i++)
			mag += p[i]*p[i];
		mag = sqrt(mag);
		for (int i=0 ; i<d ; i++)
			p[i] /= mag;
	}
	void print(FILE *fp=NULL) {
		if (fp == NULL) fp = stdout;
		for (int i=0 ; i<d ; i++)
			fprintf(fp, "%.6f ", p[i]);
		fprintf(fp, "%d\n", c);
	}
};

class Dataset {
	int n, d, c;
	Point *p;

	public:
	Dataset () {}
	Dataset (int _n, int _d, int _c, bool r=false) : n(_n), d(_d), c(_c) {
		p = new Point[n];
		for (int i=0 ; i<n ; i++) {
			p[i] = Point(d,r);
			if (r) p[i].c = rand() % c;
		}
	}
	int size () { return n; }
	int dim  () { return d; }
	int clss () { return c; }
	Point& operator[] (const int& i) {
		return p[i];
	}
	double distance (int a, int b) {
		return sqrt(sqdistance(a,b));
	}
	double distance (Point &a, Point &b) {
		return sqrt(sqdist(a,b));
	}
	double sqdistance (int a, int b) {
		return sqdist(p[a],p[b]);
	}
	double sqdist (Point &a, Point &b) {
		double dist = 0.0;
		for (int i=0 ; i<d ; i++)
			dist += pow(a[i]-b[i], 2);
		return dist;
	}
	double pivotdist (int a, int b, int c) {
		return pivotdist(p[a], p[b], p[c]);
	}
	double pivotdist (Point &a, Point &b, Point &c) {
		Point u, v;
		u = a-b;
		u.norm();
		v = c-b;
		return (v*v) / (2*(u*v));
	}
	void shuffle () {
		for (int i=0 ; i<n ; i++)
			swap(p[i], p[i + (rand() % (n-i))]);
	}
	vector<int> centroids () {
		Point *avg = new Point[c];
		int *num = new int[c];
		for (int i=0 ; i<c ; i++) {
			avg[i] = Point(d);
			for (int j=d-1 ; j>=0 ; j--)
				avg[i][j] = 0.0;
			num[i] = 0;
		}
		for (int i=0 ; i<n ; i++) {
			for (int j=d-1 ; j>=0 ; j--)
				avg[p[i].c][j] += p[i][j];
			num[p[i].c]++;
		}
		for (int i=0 ; i<c ; i++)
			for (int j=0 ; j<d ; j++)
				avg[i][j] /= (double)num[i];
		vector<int> res(c);
		vector<double> d(c,numeric_limits<double>::max());
		for (int i=0 ; i<n ; i++) {
			double r = distance(p[i],avg[p[i].c]);
			if (r < d[p[i].c]) {
				d[p[i].c] = r;
				res[p[i].c] = i;
			}
		}
		return res;
	}
};

#endif
