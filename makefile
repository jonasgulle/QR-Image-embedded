CC = gcc
STRIP = strip
CFLAGS = -I. -Wall -Os 
LFLAGS = -Wl,--gc-sections -ffunction-sections -fdata-sections
DEPS = QR_Encode.h
OBJS = QR_Encode.o QR_Data.o main.o
LIBS = ncurses

%.o: %.c $(DEPS)
	@$(CC) -c -o $@ $< $(CFLAGS)

qrencode: $(OBJS)
	@$(CC) -o $@ $^ $(LFLAGS) -l $(LIBS)
	@$(STRIP) -s -R .comment -R .gnu.version $@

.PHONY: clean
clean:
	@rm -f $(OBJS)
