all: algrss algmss algcnn algrnn algfcnn

algrss:
	rm -f RSS
	c++ -O3 RSS.cc -o RSS

algmss:
	rm -f MSS
	c++ -O3 MSS.cc -o MSS

algcnn:
	rm -f CNN
	c++ -O3 CNN.cc -o CNN

algrnn:
	rm -f RNN
	c++ -O3 RNN.cc -o RNN

algfcnn:
	rm -f FCNN
	c++ -O3 FCNN.cc -o FCNN
