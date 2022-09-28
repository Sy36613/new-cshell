CC=gcc
CFLAGS=-g -Wall

ALL = cshell

all: $(ALL)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(ALL)

