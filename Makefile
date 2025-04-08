build:
	g++ -std=c++11 main.cpp

test:
	g++ lib/translator.test.cpp -o tests.out
	./tests.out
	rm ./tests.out