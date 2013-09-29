CC = icc
INCLUDE = -I. -I../include
override CFLAGS := -Wall -Wcheck -O2 -openmp $(CFLAGS) $(INCLUDE)

SRCS = hello.c test.c matmul.c matmul_off.c loop.c fib.c pthreads.c
OBJS = $(SRCS:.c=.o)
PROGS = $(OBJS:.o=)

all: hello matmul matmul_off loop fib pthreads

check: test
	@echo
	@./test | grep "Number"
	@echo

clean:
	-rm -f $(PROGS)
	
.PHONY: all check clean
