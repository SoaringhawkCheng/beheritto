VFLAGS = -Wall
OBJS = token.o lexer.o astree.o parser.o intepreter.o main.o
beheritto: ${OBJS}
	g++ -o $@ ${OBJS}
clean:
	rm -f beheritto ${OBJS}
