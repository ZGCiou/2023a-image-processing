CC=g++ -std=c++17
CFLAGS=`pkg-config --cflags opencv4`
LDFLAGS=`pkg-config --libs opencv4`
PROG=hw4
OBJS=$(PROG).o 

run: $(PROG)
	./hw4

hw4: hw4.o BmpImage.o
	$(CC) -o $(PROG) $(OBJS) BmpImage.o $(LDFLAGS)

hw4.o: hw4.cpp
	$(CC) -c $(CFLAGS) $(PROG).cpp

BmpImage.o: BmpImage.cpp BmpImage.h
	$(CC) -c $(CFLAGS) BmpImage.cpp

clean:
	rm -rf *.o $(PROG) ../img/output*
