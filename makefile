SOURCES	= $(patsubst %.c,%.o,$(wildcard *.c))
TARGET	= libcc.a
CFLAGS	= -Wall -Werror -g -I.

all: $(SOURCES)
	$(AR) rcs $(TARGET) $(SOURCES)

run: all
	cd test && make run

clean:
	cd test && make clean
	-rm $(SOURCES)
	-rm $(TARGET)
