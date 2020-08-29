all: test/a test/b test/c libstfu.so

test/a: test/a.c
	gcc -o test/a test/a.c
test/b: test/b.c
	gcc -o test/b test/b.c
test/c: test/c.c
	gcc -o test/c test/c.c

libstfu.so: libstfu.c
	gcc -shared -fPIC -o libstfu.so libstfu.c

.PHONY: clean
clean:
	rm test/a test/b test/c libstfu.so
