comp:
	clang++ -std=c++11 -Wall -g main.cpp -v

run:
	./a.out

clean:
	rm -rf a.out* *~

test:
	./a.out
	./a.out -i 0 -f doc.txt
	./a.out -i 1000 -f doc.txt
	./a.out -i -1 -f doc.txt
	./a.out -f nofound.txt
	./a.out -i 5 what the ducky
	./a.out doc.txt
	./a.out -f doc.txt along with some other text
	./a.out here her e her her -f doc.txt lol
	./a.out -f doc.txt
	# Entire Alice in Wonderland book test
	# ./a.out -f alice29.txt 

docker:
	g++ -Wall -std=c++11 main.cpp