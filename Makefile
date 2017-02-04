PREFIX ?= /usr/local

make:
	gcc -Wall -o zpm -g zpm.c

test:
	tests/setup.sh
	ZPM_TEST_REPOS=/tmp/zpm-test-suite/repositories cram -v tests/*.t
	
install: make
	cp zpm ${PREFIX}/bin/zpm
	chmod u+x ${PREFIX}/bin/zpm

uninstall:
	rm ${PREFIX}/bin/zpm

