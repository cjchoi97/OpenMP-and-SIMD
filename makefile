all:
	g++ -o exec.out sphere.cpp

test: all
	./exec.out false 100000 16 20 100
clean:
	rm *.out
