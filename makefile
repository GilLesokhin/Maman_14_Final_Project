all: my_assembler

my_assembler: mylist.o myhash.o my_assembler.o str_util.o  assembler_util.o symbol_table.o cmd_list.o macro_map.o pre_prossesor.o reserv_table.o
	gcc -Wall -ansi -pedantic mylist.o myhash.o my_assembler.o str_util.o  assembler_util.o symbol_table.o cmd_list.o macro_map.o pre_prossesor.o reserv_table.o -o my_assembler

my_assembler.o: my_assembler.c
	gcc -Wall -ansi -pedantic my_assembler.c -c -o  my_assembler.o

pre_prossesor.o: pre_prossesor.c
	gcc -Wall -ansi -pedantic pre_prossesor.c -c -o  pre_prossesor.o



assembler_util.o: assembler_util.c
	cc -Wall -ansi -pedantic assembler_util.c -c -o  assembler_util.o

str_util.o: str_util.c
	gcc -Wall -ansi -pedantic str_util.c -c -o  str_util.o



reserv_table.o: reserv_table.c
	cc -Wall -ansi -pedantic reserv_table.c -c -o  reserv_table.o

symbol_table.o: symbol_table.c
	cc -Wall -ansi -pedantic symbol_table.c -c -o  symbol_table.o

macro_map.o: macro_map.c
	cc -Wall -ansi -pedantic macro_map.c -c -o  macro_map.o

cmd_list.o: cmd_list.c
	cc -Wall -ansi -pedantic cmd_list.c -c -o  cmd_list.o

myhash.o: myhash.c
	gcc -Wall -ansi -pedantic myhash.c -c -o  myhash.o

mylist.o: mylist.c
	gcc -Wall -ansi -pedantic mylist.c -c -o  mylist.o
