global: compile
	sudo mv branal /usr/bin

local: compile
	mv branal ${HOME}/.local/bin

compile: utils capture analyzer parser
	gcc *.o main.c -lpcap -o branal
	rm *.o

utils: structures
	gcc -c analyzer/utils/fs.c
	gcc -c analyzer/utils/time.c
	gcc -c analyzer/utils/tcp.c
	gcc -c analyzer/utils/tcp_seq_math.c

structures:
	gcc -c analyzer/utils/structures/hashmap.c
	gcc -c analyzer/utils/structures/heap.c

capture:
	gcc -c analyzer/capture.c

analyzer:
	gcc -c analyzer/analyzer.c

parser:
	gcc -c analyzer/parser/parser.c
	gcc -c analyzer/parser/fight_results.c

.PHONY: analyzer
