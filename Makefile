CC=gcc
CFLAGS=-Wall -Wextra -iquote ./analyzer -O2 -flto -s

global: compile
	sudo mv branal /usr/local/bin

local: compile
	mv branal ${HOME}/.local/bin

debug: CFLAGS=-iquote ./analyzer -Og -g
debug: global

compile: csv_table_gen main_files structures utils parser
	$(CC) $(CFLAGS) -lpcap *.o main.c -o branal
	rm *.o

csv_table_gen:
	$(CC) $(CFLAGS) csv_table_gen.c -o csv_table_gen
	./csv_table_gen
	rm csv_table_gen
	$(CC) $(CFLAGS) -c analyzer/csv/*.c

main_files:
	$(CC) $(CFLAGS) -c analyzer/commands.c
	$(CC) $(CFLAGS) -c analyzer/capture.c
	$(CC) $(CFLAGS) -c analyzer/analyzer.c
	$(CC) $(CFLAGS) -c analyzer/render.c

structures:
	$(CC) $(CFLAGS) -c analyzer/structures/hashmap.c
	$(CC) $(CFLAGS) -c analyzer/structures/heap.c

utils:
	$(CC) $(CFLAGS) -c analyzer/utils/fs.c
	$(CC) $(CFLAGS) -c analyzer/utils/time.c
	$(CC) $(CFLAGS) -c analyzer/utils/tcp.c
	$(CC) $(CFLAGS) -c analyzer/utils/tcp_seq_math.c
	$(CC) $(CFLAGS) -c analyzer/utils/roman.c
	$(CC) $(CFLAGS) -c analyzer/utils/memory.c

parser:
	$(CC) $(CFLAGS) -c analyzer/parser/parser.c
	$(CC) $(CFLAGS) -c analyzer/parser/structs.c
	$(CC) $(CFLAGS) -c analyzer/parser/fight_results/fight_results.c
