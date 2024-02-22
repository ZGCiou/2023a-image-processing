run: hw3
	./hw3

hw3: hw3.o BmpImage.o
	g++ -o hw3 hw3.o BmpImage.o

hw3.o: hw3.cpp
	g++ -c hw3.cpp

BmpImage.o: BmpImage.cpp BmpImage.h
	g++ -c BmpImage.cpp

clean:
	rm -rf *.o hw3 ../img/output*
