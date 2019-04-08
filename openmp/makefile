CPPFLAGS = -Wall -Wextra -pedantic

all:
	g++ $(CPPFLAGS) -o exec.out spheres.cpp -fopenmp

test: all
	./exec.out false 1000 16 20 100
clean:
	rm *.out
