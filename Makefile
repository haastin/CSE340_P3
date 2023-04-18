CXXFLAGS =  -c -std=c++11

all: compiler

compiler: compiler.cc parseProgram.o lexer.o inputbuf.o parseProgram.h compiler.h
	g++ -g -o compiler compiler.cc parseProgram.o lexer.o inputbuf.o

parseProgram.o: parseProgram.cc parseProgram.h lexer.o lexer.h
	g++ $(CXXFLAGS) parseProgram.cc lexer.o

lexer.o: lexer.cc inputbuf.o inputbuf.h lexer.h
	g++ $(CXXFLAGS) lexer.cc inputbuf.o

inputbuf.o: inputbuf.cc inputbuf.h
	g++ $(CXXFLAGS) inputbuf.cc

clean:
	rm -f compiler parseProgram.o lexer.o inputbuf.o
