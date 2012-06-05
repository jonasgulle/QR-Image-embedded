CC = gcc
CFLAGS = -I. -Wall
DEPS = QR_Encode.h
OBJS = QR_Encode.o main.o
LIBS = ncurses

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

qrencode: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) -l $(LIBS)

.PHONY: clean
clean:
	rm -f $(OBJS)
