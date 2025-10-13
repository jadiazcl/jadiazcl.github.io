FILE ?= example1.txt
ITERATIONS ?= 100
SIZEBLOCK ?= 256
KERNEL ?= 0

compile:	
	g++ main.cpp -o output -lOpenCL -std=c++11

opencl:
	./OPENCL/output $(FILE) $(ITERATIONS) $(PRINT) $(SIZEBLOCK) $(KERNEL)

clean:
	rm -f output	
