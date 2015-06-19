all: algnehs algrnehs algcnn algrnn algfcnn

algnehs:
	rm -f NEHS
	c++ -O3 NEHS.cc -o NEHS

algrnehs:
	rm -f RNEHS
	c++ -O3 RNEHS.cc -o RNEHS

algcnn:
	rm -f CNN
	c++ -O3 CNN.cc -o CNN

algrnn:
	rm -f RNN
	c++ -O3 RNN.cc -o RNN

algfcnn:
	rm -f FCNN
	c++ -O3 FCNN.cc -o FCNN
