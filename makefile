# generic makefile for ECE368
# Author: Aditya Biala
# Assignment: PA2

WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla 
GCC = gcc -std=c99 -g $(WARNING) $(ERROR)
VAL = valgrind --tool=memcheck --log-file=memcheck.txt --leak-check=full --verbose
#TESTFALGS = -DTEST_MAIN #s-DTEST_function

#######################################################################

SRCS = main.c huffman.c 
OBJS = $(SRCS:%.c=%.o)

pa2: $(OBJS)
	$(GCC) $(OBJS) -o pa2
	


.c.o:
	$(GCC) $(TESTFALGS) -c $*.c

memory: pa2
	$(VAL) ./pa2 original/gophers gophers.count gophers.tree gophers.code gophers.hbt
	
testall: test1 test2 test3 test4
test1: pa2
	./pa2 original/gophers gophers.count gophers.tree gophers.code gophers.hbt

test2: pa2
	./pa2 original/binary1 binary1.count binary1.tree binary1.code binary1.hbt	#diff output2.b examples/1Ksa.b
	@echo "================================"
	@echo "testing count!, binary file"
	diff binary1.count count/binary1.count
	@echo "================================"
	@echo "testing tree pre-order, binary1"
	diff binary1.tree tree/binary1.tree
	@echo "================================"
	diff binary1.code code/binary1.code
	# @echo "================================"
	# @echo "testing count!, gophers"
	# diff gophers.count count/gophers.count
	# @echo "================================"
	# @echo "testing tree pre-order, gophers"
	# diff gophers.tree tree/gophers.tree
	# @echo "================================"
	# diff gophers.code code/gophers.code

test3: pa2
	./pa2 original/lorum lorum.count lorum.tree lorum.code lorum.hbt	#diff output2.b examples/1Ksa.b
	@echo "================================"
	@echo "testing count!, lorum"
	diff lorum.count count/lorum.count
	@echo "================================"
	@echo "testing tree pre-order, lorum"
	diff lorum.tree tree/lorum.tree
	@echo "================================"
	diff lorum.code code/lorum.code

test4: pa2
	./pa2 original/empty empty.count empty.tree empty.code empty.hbt	#diff output2.b examples/1Ksa.b
	@echo "================================"
	@echo "testing count!, empty file"
	diff empty.count count/empty.count
	@echo "================================"
	@echo "testing tree pre-order, EMPTY"
	diff empty.tree tree/empty.tree
	@echo "================================"
	diff empty.code code/empty.code

test5: pa2
	./pa2 original/woods woods.count woods.tree woods.code woods.hbt	#diff output2.b examples/1Ksa.b
	@echo "================================"
	@echo "testing count!, woods file"
	diff woods.count count/woods.count
	@echo "================================"
	@echo "testing tree pre-order, woods"
	diff woods.tree tree/woods.tree
	@echo "================================"
	diff woods.code code/woods.code

test6: pa2
	./pa2 gbin.bin gbin.count gbin.tree gbin.code gbin.hbt	#diff output2.b examples/1Ksa.b
	diff gbin.count count/gophers.count
# 	@echo "================================"
# 	@echo "testing count!, woods file"
 	#diff gbin.count count/gophers.count
# 	@echo "================================"
# 	@echo "testing tree pre-order, woods"
# 	diff woods.tree tree/woods.tree
# 	@echo "================================"	




clean: # remove all machine generated files
	rm -f pa2 *.o *.count *.tree *.code *.hbt
