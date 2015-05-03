main: main.o lambdaInit.o floodFill.o src/lambdaInit.h src/floodFill.h
	gcc -o bin/main obj/main.o obj/lambdaInit.o obj/floodFill.o -lm

test_floodFill: floodFill.o test_floodFill.o
	gcc -o bin/test_floodFill obj/test_floodFill.o obj/floodFill.o -lm

test_lambOseenShear0: floodFill.o test_lambOseenShear0.o lambdaInit.o
	gcc -o bin/test_lambOseenShear0 obj/test_lambOseenShear0.o obj/floodFill.o obj/lambdaInit.o -lm

test_lambOseenShear1: floodFill.o test_lambOseenShear1.o lambdaInit.o
	gcc -o bin/test_lambOseenShear1 obj/test_lambOseenShear1.o obj/floodFill.o obj/lambdaInit.o -lm

test_lambOseenShear2: floodFill.o test_lambOseenShear2.o lambdaInit.o
	gcc -o bin/test_lambOseenShear2 obj/test_lambOseenShear2.o obj/floodFill.o obj/lambdaInit.o -lm

test_lambOseenShear3: floodFill.o test_lambOseenShear3.o lambdaInit.o
	gcc -o bin/test_lambOseenShear3 obj/test_lambOseenShear3.o obj/floodFill.o obj/lambdaInit.o -lm

test_genLOseenUniformList: test_genLOseenUniformList.o mt64.o vortexGen.o
	gcc -o test_genLOseenUniformList obj/test_genLOseenUniformList.o obj/mt64.o obj/vortexGen.o -lm

test_genLOseenBinaryList: test_genLOseenBinaryList.o mt64.o vortexGen.o
	gcc -o bin/test_genLOseenBinaryList obj/test_genLOseenBinaryList.o obj/mt64.o obj/vortexGen.o -lm

main.o: src/main.c 
	gcc -c src/main.c -o obj/main.o

lambdaInit.o: src/lambdaInit.c src/lambdaInit.h
	gcc -c src/lambdaInit.c -o obj/lambdaInit.o

floodFill.o: src/floodFill.c src/floodFill.h
	gcc -c src/floodFill.c -o obj/floodFill.o

vortexGen.o: src/vortexGen.c
	gcc -c src/vortexGen.c -o obj/vortexGen.o

mt64.o: src/mt19937-64.c src/mt64.h
	gcc -c src/mt19937-64.c -o obj/mt64.o

test_floodFill.o: src/tests/test_floodFill.c src/floodFill.h
	gcc -c src/tests/test_floodFill.c -o obj/test_floodFill.o

test_lambOseenShear0.o: src/tests/test_lambOseenShear0.c src/floodFill.h src/lambdaInit.h
	gcc -c src/tests/test_lambOseenShear0.c -o obj/test_lambOseenShear0.o

test_lambOseenShear1.o: src/tests/test_lambOseenShear1.c src/floodFill.h src/lambdaInit.h
	gcc -c src/tests/test_lambOseenShear1.c -o obj/test_lambOseenShear1.o

test_lambOseenShear2.o: src/tests/test_lambOseenShear2.c src/floodFill.h src/lambdaInit.h
	gcc -c src/tests/test_lambOseenShear2.c -o obj/test_lambOseenShear2.o

test_lambOseenShear3.o: src/tests/test_lambOseenShear3.c src/floodFill.h src/lambdaInit.h
	gcc -c src/tests/test_lambOseenShear3.c -o obj/test_lambOseenShear3.o

test_genLOseenUniformList.o: src/tests/test_genLOseenUniformList.c 
	gcc -c src/tests/test_genLOseenUniformList.c -o obj/test_genLOseenUniformList.o

test_genLOseenBinaryList.o: src/tests/test_genLOseenBinaryList.c 
	gcc -c src/tests/test_genLOseenBinaryList.c -o obj/test_genLOseenBinaryList.o
