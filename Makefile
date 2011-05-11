MCU=msp430x2012
PROGRAMMER=rf2500

CC=msp430-gcc
CFLAGS=-Os -Wall -g -mmcu=$(MCU)
OUT=main.elf
OBJS=main.o


all: $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

main.s: main.c
	$(CC) $(CFLAGS) -S main.c

clean:
	rm -fr $(OUT) $(OBJS)
