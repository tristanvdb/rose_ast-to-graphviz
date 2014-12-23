
all:
	make -C lib all
	make -C src all

check:
	make -C src check

clean:
	make -C lib clean
	make -C src clean

