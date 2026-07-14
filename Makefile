CC=gcc
CFLAGS=-Wall -iquote ./analyzer -O2 -flto -s

global: compile
	sudo mv branal /usr/bin

local: compile
	mv branal ${HOME}/.local/bin

compile: csv structures utils parser capture analyzer
	$(CC) $(CFLAGS) -lpcap *.o main.c -o branal
	rm *.o

csv:
	gcc csv_to_headers.c -o csv_to_headers
	./csv_to_headers
	rm csv_to_headers

structures:
	$(CC) $(CFLAGS) -c analyzer/structures/hashmap.c
	$(CC) $(CFLAGS) -c analyzer/structures/heap.c

utils:
	$(CC) $(CFLAGS) -c analyzer/utils/fs.c
	$(CC) $(CFLAGS) -c analyzer/utils/time.c
	$(CC) $(CFLAGS) -c analyzer/utils/tcp.c
	$(CC) $(CFLAGS) -c analyzer/utils/tcp_seq_math.c

parser:
	$(CC) $(CFLAGS) -c analyzer/parser/parser.c
	$(CC) $(CFLAGS) -c analyzer/parser/fight_results.c

capture:
	$(CC) $(CFLAGS) -c analyzer/capture.c

analyzer:
	$(CC) $(CFLAGS) -c analyzer/analyzer.c

.PHONY: analyzer
.PHONY: csv
