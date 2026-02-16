CC      = gcc
CFLAGS  = -O2 -Wall
ASFLAGS = -Wa,--noexecstack

TARGET  = antidisasm
SRCS    = main.c techniques.S
OBJS    = $(SRCS:.c=.o)
OBJS    := $(OBJS:.S=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $(ASFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
