#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>
#include <boost/algorithm/string.hpp>
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
	     if (!alg.compare("CNN"))    R = CNN(TR);
	else if (!alg.compare("RNN"))    R = RNN(TR);
	else if (!alg.compare("FCNN"))   R = FCNN(TR);
	else if (!alg.compare("SFCNN"))  R = SFCNN(TR);
	else if (!alg.compare("NET"))    R = NET(TR);
	else if (!alg.compare("MSS"))    R = MSS(TR);
	else if (!alg.compare("RSS"))    R = RSS(TR);
	else if (!alg.compare("VSS"))    R = VSS(TR);
	else if (!alg.compare("HSS"))    R = HSS(TR);
	else if (boost::algorithm::ends_with(alg,"-NET"))
		R = AlphaNET(TR,atof(argv[1]));
	else if (boost::algorithm::ends_with(alg,"-MSS"))
		R = AlphaMSS(TR,atof(argv[1]));
	else if (boost::algorithm::ends_with(alg,"-HSS"))
		R = AlphaHSS(TR,atof(argv[1]));
	else if (boost::algorithm::ends_with(alg,"-RSS")) {
		double alpha, epsilon = 0.0;
		string name(argv[1]);
		int pos = name.find(',');
		if (pos != string::npos) {
			argv[1][pos] = '\0';
			epsilon = atof(argv[1]+pos+1);
		}
		alpha = atof(argv[1]);
		R = AlphaRSS(TR,alpha,epsilon);
	}

	PrintResult(TR, TS, R);

	return 0;
}
