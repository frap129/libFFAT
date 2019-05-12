all:
	gcc -Wall -g -std=c99 -O0 -fpic -c -D_FILE_OFFSET_BITS=64 ffat.c -lfuse
	ld -shared ffat.o -o libFFAT.so
	echo "Successfull compiled!"

clean:
	rm -f libFFAT.so ffat.o
