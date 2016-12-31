make:
	gcc -o zpm -g zpm.c

test: make
	zcram ./zpm.t
	
