comp:
	clang++ -std=c++11 -Wall -g -o sorer main.cpp -v

run:
	./sorer

clean:
	rm -rf sorer* *~

test:
	./sorer 
	./sorer -f doc.txt
	./sorer -f "doc.txt"
	./sorer -f doc.txt -from 10
	./sorer -f doc.txt -from 10 -len 100
	./sorer -f doc.txt -from 10 -len 100 -print_col_type 0
	./sorer -f doc.txt -from 30 -print_col_idx 0 1
	./sorer -f doc.txt -from 2 -len 200 -is_missing_idx 2 2
	-./sorer -f doc.txt -is_missing_idx 0
	-./sorer -f nofound
	-./sorer -from 2 what the ducky
	-./sorer doc.txt
	-./sorer -f doc.txt along with some other text
	-./sorer here her e her her -f doc.txt lol
	-./sorer -is_missing_idx ajd 2

docker:
	g++ -O3 -Wall -std=c++11 -o sorer main.cpp 