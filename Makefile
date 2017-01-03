make:
	gcc -Wall -o zpm -g zpm.c

test: make
	zcram ./zpm.t
	
install: make
	cp zpm /usr/local/bin/zpm
	chmod u+x /usr/local/bin/zpm

uninstall:
	rm /usr/local/bin/zpm

