SOURCES	= $(patsubst %.c,%.o,$(wildcard *.c))
TARGET	= libcc
CFLAGS	= -Wall -Werror -g -I. -I..
LIBCC		= ../libcc.a

all: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LIBCC) -o $(TARGET)

run: all
	cat langtest.txt
	cat langtest.txt | ./$(TARGET)

clean:
	-rm $(SOURCES)
	-rm $(TARGET)
	-rm out *.o *.lst
