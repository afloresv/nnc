#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>
#include "Condensation.h"

using namespace std;

int main(int argc, char* argv[]) {

	Dataset TR, TS;
	ReadFile(string(argv[2]),atoi(argv[3]),TR,TS);
	
	string alg(argv[1]);
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
	else if (!alg.compare("AlphaMSS")) R = AlphaMSS(TR,atof(argv[4]));
	else if (!alg.compare("RSS"))      R = RSS(TR);
	else if (!alg.compare("AlphaRSS")) R = AlphaRSS(TR,atof(argv[4]));

	R.print();

	return 0;
}
