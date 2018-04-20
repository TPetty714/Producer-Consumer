conprod:main.c buffer.h
	gcc -Wall -g -pedantic -std=c99 -o conprod main.c buffer.h -lm -pthread