CC= gcc
CFLAGS= -c -Wextra -Wvla -Wall -std=c99 -DNDEBUG
CODEFILES= ex2.tar README CheckDependency.c MyLinkedList.c Makefile valdbg_check_dependency.out valdbg_list_example.out


# Default
default: ListExample
	./ListExample


# All Target
all: CheckDependency ListExample


# Exceutables
CheckDependency: CheckDependency.o
	$(CC) CheckDependency.o -o CheckDependency

ListExample: ListExample.o MyLinkedList.o
	$(CC) ListExample.o MyLinkedList.o -o ListExample


# Object Files
CheckDependency.o: CheckDependency.c
	$(CC) $(CFLAGS) CheckDependency.c -o CheckDependency.o

ListExample.o: ListExample.c MyLinkedList.c MyLinkedList.h
	$(CC) $(CFLAGS) ListExample.c -o ListExample.o

MyLinkedList.o: MyLinkedList.c MyLinkedList.h
	$(CC) $(CFLAGS) MyLinkedList.c -o MyLinkedList.o


# tar
tar:
	tar -cf $(CODEFILES)


# Other Targets
clean:
	-rm -f *.o CheckDependency ListExample
