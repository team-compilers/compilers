CC=g++
HEADERS=./inc
SRC=./src
FLAGS=-O2 -std=c++11

homework: $(SRC)/*.cpp
	$(CC) -I$(HEADERS) $(FLAGS) -o $@ $^

clean:
	rm -f homework
