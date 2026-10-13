CC=gcc

CFLAGS=-c -Wall

LDFLAGS=-lm

SOURCES= main.c vector_calc.c

OBJECTS=$(SOURCES:.c=.o)

EXECUTABLES= main

all: $(SOURCES) $(EXECUTABLES)

# pull in dependency info for *existing* .o files

-include $(OBJECTS:.o=.d)

main: main.o vector_calc.o
	$(CC) main.o vector_calc.o $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > $*.d

clean:
	rm -rf $(OBJECTS) $(EXECUTABLES) *.d