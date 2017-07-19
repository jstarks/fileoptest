TESTDIR=testdir

fileoptest : fileoptest.c
	$(CC) -O2 -Wall fileoptest.c -o fileoptest

test : fileoptest
	time ./fileoptest create $(TESTDIR) 50000
	time ./fileoptest open $(TESTDIR) 50000
	time ./fileoptest stat $(TESTDIR) 50000
	time ./fileoptest unlink $(TESTDIR) 50000
	time ./fileoptest create $(TESTDIR) 500000
	time ./fileoptest open $(TESTDIR) 500000
	time ./fileoptest stat $(TESTDIR) 500000
	time ./fileoptest unlink $(TESTDIR) 500000
