memory_manager.out : main.o buddy_mm.o parser.o errors.o commands.o
	gcc -o buddy_mm.out main.o buddy_mm.o parser.o errors.o commands.o

main.o : main.c
	gcc -std=c11 -pedantic -Wall -Wextra -c main.c

buddy_mm.o : buddy_mm/buddy_mm.c
	gcc -std=c11 -pedantic -Wall -Wextra -c buddy_mm/buddy_mm.c
	
parser.o : parser/parser.c
	gcc -std=c11 -pedantic -Wall -Wextra -c parser/parser.c

errors.o : properties/errors.c
	gcc -std=c11 -pedantic -Wall -Wextra -c properties/errors.c

commands.o : properties/commands.c
	gcc -std=c11 -pedantic -Wall -Wextra -c properties/commands.c
