
.SUFFIXES: .c .o
CC = gcc
LD = ld

CFLAGS  = $(INC)
LDFLAGS = -lglib-2.0 -lpthread -lrt
INC = -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include 
OBJS1 = websocket-gmainloop.o
SRCS = $(OBJS1:.o=.c)
TARGET1 = $(OBJS1:.o=)

all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(OBJS1)
	$(CC) -o $(TARGET1) $(OBJS1) $(INC) $(LDFLAGS)

dep:
	gccmakedep $(INC) $(SRCS)

clean:
	rm -rf $(OBJS1) $(OBJS2) $(OBJS3)

