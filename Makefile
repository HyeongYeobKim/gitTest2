
CC		= gcc
CFLAGS	= -W -Wall -O2

all : tcp_server tcp_client

tcp_server : tcp_server.c
	$(CC) $(CFLAGS) -o $@ $^

tcp_client : tcp_client.c
	$(CC) $(CFALGS) -o $@ $%
