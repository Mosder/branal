global: compile
	sudo mv branal /usr/bin

local: compile
	mv branal ${HOME}/.local/bin

compile: utils capture
	gcc *.o main.c -o branal
	rm *.o

utils:
	gcc -c analyzer/utils/fs.c
	gcc -c analyzer/utils/time.c

capture:
	gcc -c analyzer/capture.c
