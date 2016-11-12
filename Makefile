.PHONE : all

all : liblinklist.so test

liblinklist.so : linkList.o
	gcc -Wall -fPIC -shared $^ -o $@
test: linkListDemo.o
	gcc -Wall -L./ -llinklist $^ -o $@ -Wl,-rpath=.
linkListDemo.o : linkListDemo.c
	gcc -c $<
linkList.o : linkList.c
	gcc -c $<

clean:
	rm -f *.so *.o test
