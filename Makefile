fileoptest : fileoptest.c
	cc -O2 -Wall fileoptest.c -o fileoptest

test : fileoptest
	time ./fileoptest create testdir
	time ./fileoptest open testdir
	time ./fileoptest stat testdir
	time ./fileoptest unlink testdir
	time ./fileoptest -nochdir create $(PWD)/testdir
	time ./fileoptest -nochdir open $(PWD)/testdir
	time ./fileoptest -nochdir stat $(PWD)/testdir
	time ./fileoptest -nochdir unlink $(PWD)/testdir
