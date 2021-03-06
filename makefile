CFLAGS = -Wall -std=c++11 -g
OBJS = token.o lexer.o astree.o parser.o intepreter.o
GC = mempool.o collector.o
beheritto: ${OBJS} ${GC} main.o
	g++ -o $@ ${OBJS} ${GC} main.o
test:${OBJS} testmain.o
	g++ -o $@ ${OBJS} testmain.o 
testgc:${OBJS} ${GC} testmain.o
	g++ -o $@ ${OBJS} ${GC} testmain.o
clean:
	rm -f beheritto test testgc ${OBJS} ${GC} testmain.o main.o

