TESTDIR=testdir
TIME=/usr/bin/time -f %E

fileoptest : fileoptest.c
	$(CC) -O2 -Wall fileoptest.c -o fileoptest

test : $(SUFFIX)fileoptest
	$(TIME) ./fileoptest create $(TESTDIR) 50000
	$(TIME) ./fileoptest open $(TESTDIR) 50000
	$(TIME) ./fileoptest stat $(TESTDIR) 50000
	$(TIME) ./fileoptest unlink $(TESTDIR) 50000
	$(TIME) ./fileoptest create $(TESTDIR) 500000
	$(TIME) ./fileoptest open $(TESTDIR) 500000
	$(TIME) ./fileoptest stat $(TESTDIR) 500000
	$(TIME) ./fileoptest unlink $(TESTDIR) 500000
