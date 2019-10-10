all: compile runtest runvar

compile:
	g++ -std=c++14 -Wall -O2 practice_1.cpp -o gurtova

runtest:
	./gurtova md2 "0312110203301120" "3222221003301230"
	./gurtova compress "1322021003301230" "1202310203301120"

runvar:
	./gurtova compress "3203201032233030" "0012232201310331"	

runmd2:
	./gurtova md2 "0312110203301120" "3222221003301230"
runcompress:
	./gurtova compress "1322021003301230" "1202310203301120"
	
clean:
	rm gurtova
