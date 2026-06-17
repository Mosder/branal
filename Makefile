global: compile
	sudo mv branal /usr/bin

local: compile
	mv branal ${HOME}/.local/bin

compile: utils capture analyzer
	gcc *.o main.c -lpcap -o branal
	rm *.o

utils:
	gcc -c analyzer/utils/fs.c
	gcc -c analyzer/utils/time.c
	gcc -c analyzer/utils/pcap.c

capture:
	gcc -c analyzer/capture.c

analyzer:
	gcc -c analyzer/analyzer.c

.PHONY: analyzer
