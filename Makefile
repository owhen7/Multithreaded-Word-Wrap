CC=gcc
CFLAGS= -lpthread -Wall -Wvla -fsanitize=address
DEPS = wwheader.h
OBJ = directoryWorker.o main.o  

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mtww: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
