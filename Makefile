make:
	gcc -o zpm -g zpm.c

test: make
	zsh ~/Projects/zcram/zcram.zsh ./zpm.t
	
