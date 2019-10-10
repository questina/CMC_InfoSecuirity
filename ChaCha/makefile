all: compile run show_out

compile:
	g++ -std=c++14 -Wall -O2 chacha20.cpp -o chacha20

run:
	./chacha20 input key iv output

show_out:
	cat output

show_inp:
	cat input

show_key:
	cat key

show_iv:
	cat iv
	
clean:
	rm a.out
