PREFIX ?= /usr/local

make:
	gcc -Wall -o zpm -g zpm.c

test: make
	cram ./zpm.t
	
install: make
	cp zpm ${PREFIX}/bin/zpm
	chmod u+x ${PREFIX}/bin/zpm

uninstall:
	rm ${PREFIX}/bin/zpm

