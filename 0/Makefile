CC := gcc
BINDIR := bin
SRCDIR := src
TESTDIR := $(SRCDIR)/test/
INCDIR := $(SRCDIR)/include/

SERVER := serv
STUCLIENT := stucli
PROFCLIENT := profcli

HELPERS := $(SRCDIR)/helpers/*.c

SERVFILES := $(SRCDIR)/servidor.c
STUCLIFILES := $(SRCDIR)/cliente-aluno.c
PROFCLIFILES := $(SRCDIR)/cliente-prof.c

CFLAGS := -g -Wall -O3 -std=c11 -ggdb3

main:
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -I $(INCDIR) $(HELPERS) $(SERVFILES) -lpthread -o $(BINDIR)/$(SERVER)
	$(CC) $(CFLAGS) -I $(INCDIR) $(HELPERS) $(STUCLIFILES) -o $(BINDIR)/$(STUCLIENT)
	$(CC) $(CFLAGS) -I $(INCDIR) $(HELPERS) $(PROFCLIFILES) -o $(BINDIR)/$(PROFCLIENT)

all: main

clean:
	$(RM) -r $(BINDIR)

# valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose $(BINDIR)/$(PROGRAM) ${ARGS}