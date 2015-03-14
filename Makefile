all: algnehs algcnn algfcnn

algnehs:
	rm -f NEHS
	c++ -O3 NEHS.cc -o NEHS

algcnn:
	rm -f CNN
	c++ -O3 CNN.cc -o CNN

algfcnn:
	rm -f FCNN
	c++ -O3 FCNN.cc -o FCNN
