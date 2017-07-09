CFLAGS = -Wall
OBJS = token.o lexer.o astree.o parser.o intepreter.o
beheritto: ${OBJS} main.o
	g++ -o $@ ${OBJS} main.o
test:${OBJS} testmain.o
	g++ -o $@ ${OBJS} testmain.o
clean:
	rm -f beheritto test ${OBJS}
