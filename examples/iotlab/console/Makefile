CONTIKI_PROJECT = console_server
CONTIKI_SOURCEFILES += console_server.c myencode.c

CONSOLE_CLIENT_SRCS = client.c myencode.c

#CFLAGS += -DWITH_UIP6

all: $(CONTIKI_PROJECT) client

client: $(CONSOLE_CLIENT_SRCS)
	gcc -std=gnu99 $(CONSOLE_CLIENT_SRCS) -o client -lpthread

CONTIKI = ../../..
include $(CONTIKI)/Makefile.include
