run: hw1
	./hw1

hw1: hw1.o BmpImage.o
	g++ -o hw1 hw1.o BmpImage.o

hw1.o: hw1.cpp
	g++ -c hw1.cpp

BmpImage.o: BmpImage.cpp BmpImage.h
	g++ -c BmpImage.cpp

clean:
	rm -rf *.o hw1 output*