CFLAGS = -Wall -std=c++11
OBJS = token.o lexer.o astree.o parser.o intepreter.o
GC = mempool.o collector.o mylist.o
beheritto: ${OBJS} main.o
	g++ -o $@ ${OBJS} main.o
test:${OBJS} testmain.o
	g++ -o $@ ${OBJS} testmain.o 
testgc:${OBJS} ${GC} testmain.o
	g++ -o $@ ${OBJS} ${GC} testmain.o
clean:
	rm -f beheritto test ${OBJS} ${GC} testmain.o main.o beheritto test testgc

