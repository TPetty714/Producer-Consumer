conprod:main.c buffer.h
	gcc -Wall -std=c99 -g -pedantic -o conprod main.c buffer.h -lm -pthread