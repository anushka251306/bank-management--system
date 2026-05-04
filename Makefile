CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99

TARGET  = bank
SRC     = bank.c
HDR     = bank.h

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) accounts.dat accounts.tmp transactions.log
