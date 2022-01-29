# FILE:=export PATH=.:$$PATH
turing: main.o  turing_parser.o turing_simulator.o
#	$(FILE)
	g++ -o turing main.o turing_parser.o turing_simulator.o
	rm -rf *.o
main.o: main.cpp
	g++ -c main.cpp
turing_parser.o: turing_parser.cpp
	g++ -c turing_parser.cpp
turing_simulator.o: turing_simulator.cpp
	g++ -c turing_simulator.cpp
clean:
	rm -rf turing
