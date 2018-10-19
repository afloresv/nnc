all: algnnc

algnnc:
	rm -f NNC
	c++ -O3 src/NNC.cc -o NNC
