global: compile
	sudo mv branal /usr/bin

local: compile
	mv branal ${HOME}/.local/bin

compile: utils
	gcc *.o main.c -o branal
	rm *.o

utils:
	gcc -c analyzer/utils/fs.c
