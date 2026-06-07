run:
	g++ main.cpp -o main -Wall -Wextra -std=c++20 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	./main

clean:
	rm -f main
