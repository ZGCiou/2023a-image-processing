run: hw2
	./hw2 ../img/input1.bmp ../img/input2.bmp ../img/input3.bmp

hw2: hw2.o BmpImage.o
	g++ -o hw2 hw2.o BmpImage.o

hw2.o: hw2.cpp
	g++ -c hw2.cpp

BmpImage.o: BmpImage.cpp BmpImage.h
	g++ -c BmpImage.cpp

clean:
	rm -rf *.o hw2 output*

demo: hw2
	./hw2 ../img/test1.bmp ../img/test2.bmp ../img/test3.bmp